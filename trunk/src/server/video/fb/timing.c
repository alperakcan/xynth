/***************************************************************************
    begin                : Sun Oct 12 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(VIDEO_FBDev)

#include <math.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

#define gtf_lockVF	1	/* Lock to vertical frequency			*/
#define gtf_lockHF	2	/* Lock to horizontal frequency			*/
#define gtf_lockPF	3	/* Lock to pixel clock frequency		*/

typedef struct {
	double margin;		/* Margin size as percentage of display		*/
	double cellGran;	/* Character cell granularity			*/
	double minPorch;	/* Minimum front porch in lines/chars		*/
	double vSyncRqd;	/* Width of V sync in lines			*/
	double hSync;		/* Width of H sync as percent of total		*/
	double minVSyncBP;	/* Minimum vertical sync + back porch (us)	*/
	double m;		/* Blanking formula gradient			*/
	double c;		/* Blanking formula offset			*/
	double k;		/* Blanking formula scaling factor		*/
	double j;		/* Blanking formula scaling factor weight	*/
} gtf_constants;

typedef struct {
	int hTotal;         	/* Horizontal total				*/
	int hDisp;		/* Horizontal displayed				*/
	int hSyncStart;     	/* Horizontal sync start			*/
	int hSyncEnd;     	/* Horizontal sync end				*/
	int hFrontPorch;	/* Horizontal front porch			*/
	int hSyncWidth;		/* Horizontal sync width			*/
	int hBackPorch;		/* Horizontal back porch			*/
} gtf_hCRTC;

typedef struct {
	int vTotal;         	/* Vertical total				*/
	int vDisp;  		/* Vertical displayed				*/
	int vSyncStart;     	/* Vertical sync start				*/
	int vSyncEnd;       	/* Vertical sync end				*/
	int vFrontPorch;	/* Vertical front porch				*/
	int vSyncWidth;		/* Vertical sync width				*/
	int vBackPorch;		/* Vertical back porch				*/
} gtf_vCRTC;

typedef struct {
	gtf_hCRTC h;		/* Horizontal CRTC paremeters			*/
	gtf_vCRTC v;		/* Vertical CRTC parameters			*/
	char hSyncPol;		/* Horizontal sync polarity			*/
	char vSyncPol;		/* Vertical sync polarity			*/
	char interlace;		/* 'I' for Interlace, 'N' for Non		*/
	double vFreq;		/* Vertical frequency (Hz)			*/
	double hFreq;		/* Horizontal frequency (KHz)			*/
	double dotClock;	/* Pixel clock (Mhz)				*/
} gtf_timings;

static gtf_constants GC = {
	1.8,	/* Margin size as percentage of display		*/
	8,	/* Character cell granularity			*/
	1,	/* Minimum front porch in lines/chars		*/
	3,	/* Width of V sync in lines			*/
	8,	/* Width of H sync as percent of total		*/
	550,	/* Minimum vertical sync + back porch (us)	*/
	600,	/* Blanking formula gradient			*/
	40,	/* Blanking formula offset			*/
	128,	/* Blanking formula scaling factor		*/
	20	/* Blanking formula scaling factor weight	*/
};

static inline double xround (double v)
{
	return floor(v + 0.5);
}

/*
	hPixels		- X resolution
	vLines		- Y resolution
	freq		- Frequency (Hz, KHz or MHz depending on type)
	type  		- 1 - vertical, 2 - horizontal, 3 - dot clock
	margins		- True if margins should be generated
	interlace	- True if interlaced timings to be generated
	t		- Place to store the resulting timings
*/
void gtf_calcTimings (double hPixels, double vLines, double freq, int type, int wantMargins, int wantInterlace, gtf_timings *t)
{
	double interlace;
	double vFieldRate;
	double hPeriod = 0;
	double topMarginLines;
	double botMarginLines;
	double leftMarginPixels;
	double rightMarginPixels;
	double hPeriodEst = 0;
	double vSyncBP = 0;
	double vBackPorch = 0;
	double vTotalLines = 0;
	double vFieldRateEst;
	double hTotalPixels;
	double hTotalActivePixels;
	double hBlankPixels;
	double idealDutyCycle = 0;
	double hSyncWidth;
	double hSyncBP;
	double hBackPorch;
	double idealHPeriod;
	double vFreq;
	double hFreq;
	double dotClock;
	gtf_constants c;

	/* Get rounded gtf constants used for internal calculations */
	c.margin = GC.margin;
	c.cellGran = xround(GC.cellGran);
	c.minPorch = xround(GC.minPorch);
	c.vSyncRqd = xround(GC.vSyncRqd);
	c.hSync = GC.hSync;
	c.minVSyncBP = GC.minVSyncBP;
	if (GC.k == 0) {
		c.k = 0.001;
        } else {
		c.k = GC.k;
	}
	c.m = (c.k / 256) * GC.m;
	c.c = (GC.c - GC.j) * (c.k / 256) + GC.j;
	c.j = GC.j;
	/* Move input parameters into appropriate variables */
	vFreq = hFreq = dotClock = freq;
	/* Round pixels to character cell granularity */
	hPixels = xround(hPixels / c.cellGran) * c.cellGran;
	/* For interlaced mode halve the vertical parameters, and double
	 * the required field refresh rate.
	 */
	if (wantInterlace) {
		vLines = xround(vLines / 2);
		vFieldRate = vFreq * 2;
		dotClock = dotClock * 2;
		interlace = 0.5;
	} else {
		vFieldRate = vFreq;
		interlace = 0;
	}
	/* Determine the lines for margins */
	if (wantMargins) {
		topMarginLines = xround(c.margin / 100 * vLines);
		botMarginLines = xround(c.margin / 100 * vLines);
	} else {
		topMarginLines = 0;
		botMarginLines = 0;
	}
	if (type != gtf_lockPF) {
		if (type == gtf_lockVF) {
			/* Estimate the horizontal period */
			hPeriodEst = ((1/vFieldRate) - (c.minVSyncBP/1000000)) /
				     (vLines + (2*topMarginLines) + c.minPorch + interlace) * 1000000;
			/* Find the number of lines in vSync + back porch */
			vSyncBP = xround(c.minVSyncBP / hPeriodEst);
		} else if (type == gtf_lockHF) {
			/* Find the number of lines in vSync + back porch */
			vSyncBP = xround((c.minVSyncBP * hFreq) / 1000);
		}
		/* Find the number of lines in the V back porch alone */
		vBackPorch = vSyncBP - c.vSyncRqd;
		/* Find the total number of lines in the vertical period */
		vTotalLines = vLines + topMarginLines + botMarginLines + vSyncBP +
		              interlace + c.minPorch;
		if (type == gtf_lockVF) {
			/* Estimate the vertical frequency */
			vFieldRateEst = 1000000 / (hPeriodEst * vTotalLines);
			/* Find the actual horizontal period */
			hPeriod = (hPeriodEst * vFieldRateEst) / vFieldRate;
			/* Find the actual vertical field frequency */
			vFieldRate = 1000000 / (hPeriod * vTotalLines);
		} else if (type == gtf_lockHF) {
			/* Find the actual vertical field frequency */
			vFieldRate = (hFreq / vTotalLines) * 1000;
		}
	}
	/* Find the number of pixels in the left and right margins */
	if (wantMargins) {
		leftMarginPixels = xround(hPixels * c.margin) / (100 * c.cellGran);
		rightMarginPixels = xround(hPixels * c.margin) / (100 * c.cellGran);
	} else {
		leftMarginPixels = 0;
		rightMarginPixels = 0;
	}
	/* Find the total number of active pixels in image + margins */
	hTotalActivePixels = hPixels + leftMarginPixels + rightMarginPixels;
	if (type == gtf_lockVF) {
		/* Find the ideal blanking duty cycle */
		idealDutyCycle = c.c - ((c.m * hPeriod) / 1000);
	} else if (type == gtf_lockHF) {
		/* Find the ideal blanking duty cycle */
		idealDutyCycle = c.c - (c.m / hFreq);
	} else if (type == gtf_lockPF) {
		/* Find ideal horizontal period from blanking duty cycle formula */
		idealHPeriod = (((c.c - 100) + (sqrt((pow(100-c.c,2)) +
			        (0.4 * c.m * (hTotalActivePixels + rightMarginPixels +
			        leftMarginPixels) / dotClock)))) / (2 * c.m)) * 1000;
		/* Find the ideal blanking duty cycle */
		idealDutyCycle = c.c - ((c.m * idealHPeriod) / 1000);
	}
	/* Find the number of pixels in blanking time */
	hBlankPixels = xround((hTotalActivePixels * idealDutyCycle) /
	                     ((100 - idealDutyCycle) * 2 * c.cellGran)) * (2 * c.cellGran);
	/* Find the total number of pixels */
	hTotalPixels = hTotalActivePixels + hBlankPixels;
	/* Find the horizontal back porch */
	hBackPorch = xround((hBlankPixels / 2) / c.cellGran) * c.cellGran;
	/* Find the horizontal sync width */
	hSyncWidth = xround(((c.hSync/100) * hTotalPixels) / c.cellGran) * c.cellGran;
	/* Find the horizontal sync + back porch */
	hSyncBP = hBackPorch + hSyncWidth;
	if (type == gtf_lockPF) {
		/* Find the horizontal frequency */
		hFreq = (dotClock / hTotalPixels) * 1000;
		/* Find the horizontal period */
		hPeriod = 1000 / hFreq;
		/* Find the number of lines in vSync + back porch */
		vSyncBP = xround((c.minVSyncBP * hFreq) / 1000);
		/* Find the number of lines in the V back porch alone */
		vBackPorch = vSyncBP - c.vSyncRqd;
		/* Find the total number of lines in the vertical period */
		vTotalLines = vLines + topMarginLines + botMarginLines + vSyncBP +
		              interlace + c.minPorch;
		/* Find the actual vertical field frequency */
		vFieldRate = (hFreq / vTotalLines) * 1000;
	} else {
		if (type == gtf_lockVF) {
			/* Find the horizontal frequency */
			hFreq = 1000 / hPeriod;
		} else if (type == gtf_lockHF) {
			/* Find the horizontal frequency */
			hPeriod = 1000 / hFreq;
		}
        	/* Find the pixel clock frequency */
		dotClock = hTotalPixels / hPeriod;
	}
	/* Find the vertical frame frequency */
	if (wantInterlace) {
		vFreq = vFieldRate / 2;
		dotClock = dotClock / 2;
	} else {
		vFreq = vFieldRate;
	}
	/* Return the computed frequencies */
	t->vFreq = vFreq;
	t->hFreq = hFreq;
	t->dotClock = dotClock;
	/* Determine the vertical timing parameters */
	t->h.hTotal = hTotalPixels;
	t->h.hDisp = hTotalActivePixels;
	t->h.hSyncStart = t->h.hTotal - hSyncBP;
	t->h.hSyncEnd = t->h.hTotal - hBackPorch;
	t->h.hFrontPorch = t->h.hSyncStart - t->h.hDisp;
	t->h.hSyncWidth = hSyncWidth;
	t->h.hBackPorch = hBackPorch;
	/* Determine the vertical timing parameters */
	t->v.vTotal = vTotalLines;
	t->v.vDisp = vLines;
	t->v.vSyncStart = t->v.vTotal - vSyncBP;
	t->v.vSyncEnd = t->v.vTotal - vBackPorch;
	t->v.vFrontPorch = t->v.vSyncStart - t->v.vDisp;
	t->v.vSyncWidth = c.vSyncRqd;
	t->v.vBackPorch = vBackPorch;
	/* Mark as gtf timing using the sync polarities */
	t->interlace = (wantInterlace) ? 'I' : 'N';
	t->hSyncPol = '-';
	t->vSyncPol = '+';
}

void fb_timing_set (s_server_conf_t *cfg, struct fb_var_screeninfo *v_scr)
{
	gtf_timings t;
	int interlace = 0;
	double freq = 150;
	double xPixels = v_scr->xres;
	double yPixels = v_scr->yres;
	
	if (cfg->monitor.pixclock ||
	    cfg->monitor.left_margin ||
	    cfg->monitor.right_margin ||
	    cfg->monitor.upper_margin ||
	    cfg->monitor.lower_margin ||
	    cfg->monitor.hsync_len ||
	    cfg->monitor.vsync_len ||
	    cfg->monitor.vmode) {
	    	v_scr->pixclock = cfg->monitor.pixclock;
	    	v_scr->left_margin = cfg->monitor.left_margin;
	    	v_scr->right_margin = cfg->monitor.right_margin;
	    	v_scr->upper_margin = cfg->monitor.upper_margin;
	    	v_scr->lower_margin = cfg->monitor.lower_margin;
	    	v_scr->hsync_len = cfg->monitor.hsync_len;
	    	v_scr->vsync_len = cfg->monitor.vsync_len;
	    	v_scr->vmode = cfg->monitor.vmode;
	    	return;
	} else {
		while (1) {
			gtf_calcTimings(xPixels, yPixels, freq, gtf_lockPF, 0, interlace, &t);
			if ((t.vFreq > cfg->monitor.vertrefresh_min) && (t.vFreq < cfg->monitor.vertrefresh_max) &&
			    (t.hFreq > cfg->monitor.horizsync_min) && (t.hFreq < cfg->monitor.horizsync_max)) {
				v_scr->pixclock = 1000000000 / (t.dotClock * 1000);
				v_scr->left_margin = t.h.hBackPorch;
				v_scr->right_margin = t.h.hFrontPorch;
				v_scr->upper_margin = t.v.vBackPorch;
				v_scr->lower_margin = t.v.vFrontPorch;
				v_scr->hsync_len = t.h.hSyncWidth;
				v_scr->vsync_len = t.v.vSyncWidth;
				v_scr->sync = FB_SYNC_VERT_HIGH_ACT;
				v_scr->vmode = (interlace) ? FB_VMODE_INTERLACED : 0;
				return;
			}
			if (freq-- <= 0) {
				debugf(DSER | DFAT, "FBDEV : Unable to get a suitable timing frequency for your monitor");
			}
		}
	}
}

#if defined(TESTING_gtf)
void main(int argc,char *argv[])
{
	double xPixels = w;
	double yPixels = h;
	double freq = 150;
	int interlace = 0;
	gtf_timings t;

	if (argc != 5 && argc != 6) {
		printf("Usage: gtfCALC <xPixels> <yPixels> <freq> [[Hz] [KHz] [MHz]] [I]\n");
		printf("\n");
		printf("where <xPixels> is the horizontal resolution of the mode, <yPixels> is the\n");
		printf("vertical resolution of the mode. The <freq> value will be the frequency to\n");
		printf("drive the calculations, and will be either the vertical frequency (in Hz)\n");
		printf("the horizontal frequency (in KHz) or the dot clock (in MHz). To generate\n");
		printf("timings for an interlaced mode, add 'I' to the end of the command line.\n");
		printf("\n");
		printf("For example to generate timings for 640x480 at 60Hz vertical:\n");
		printf("\n");
		printf("    gtfCALC 640 480 60 Hz\n");
		printf("\n");
		printf("For example to generate timings for 640x480 at 31.5KHz horizontal:\n");
		printf("\n");
		printf("    gtfCALC 640 480 31.5 KHz\n");
		printf("\n");
		printf("For example to generate timings for 640x480 with a 25.175Mhz dot clock:\n");
		printf("\n");
		printf("    gtfCALC 640 480 25.175 MHz\n");
		printf("\n");
		exit(1);
	}
	/* Get values from command line */
	xPixels = atof(argv[1]);
	yPixels = atof(argv[2]);
	freq = atof(argv[3]);
	interlace = ((argc == 6) && (argv[5][0] == 'I'));
	/* Compute the CRTC timings */
	if (toupper(argv[4][0]) == 'H') {
		gtf_calcTimings(xPixels, yPixels, freq, gtf_lockVF, 0, interlace, &t);
	} else if (toupper(argv[4][0]) == 'K') {
		gtf_calcTimings(xPixels, yPixels, freq, gtf_lockHF, 0, interlace, &t);
	} else if (toupper(argv[4][0]) == 'M') {
		gtf_calcTimings(xPixels, yPixels, freq, gtf_lockPF, 0, interlace, &t);
	} else {
		printf("Unknown command line!\n");
		exit(1);
	}
	/* Dump summary info to standard output */
	printf("CRTC values for %.0fx%.0f @ %.2f %s\n", xPixels, yPixels, freq, argv[4]);
	printf("\n");
	printf("  hTotal      = %-4d    vTotal      = %-4d\n",
		t.h.hTotal, t.v.vTotal);
	printf("  hDisp       = %-4d    vDisp       = %-4d\n",
		t.h.hDisp, t.v.vDisp);
	printf("  hSyncStart  = %-4d    vSyncStart  = %-4d\n",
		t.h.hSyncStart, t.v.vSyncStart);
	printf("  hSyncEnd    = %-4d    vSyncEnd    = %-4d\n",
		t.h.hSyncEnd, t.v.vSyncEnd);
	printf("  hFrontPorch = %-4d    vFrontPorch = %-4d\n",
		t.h.hFrontPorch, t.v.vFrontPorch);
	printf("  hSyncWidth  = %-4d    vSyncWidth  = %-4d\n",
		t.h.hSyncWidth, t.v.vSyncWidth);
	printf("  hBackPorch  = %-4d    vBackPorch  = %-4d\n",
		t.h.hBackPorch, t.v.vBackPorch);
	printf("\n");
	printf("  Interlaced  = %s\n", (t.interlace == 'I') ? "Yes" : "No");
	printf("  H sync pol  = %c\n", t.hSyncPol);
	printf("  V sync pol  = %c\n", t.vSyncPol);
	printf("\n");
	printf("  Vert freq   = %.2f Hz\n", t.vFreq);
	printf("  Horiz freq  = %.2f KHz\n", t.hFreq);
	printf("  Dot Clock   = %.2f Mhz\n", t.dotClock);
	fprintf(stderr,"Modeline %c%ix%i@%.0f%c %.3f %i %i %i %i %i %i %i %i %s %chsync %cvsync\n",
		'"',t.h.hDisp, t.v.vDisp, t.vFreq,'"',
		t.dotClock,
		t.h.hDisp,
		t.h.hSyncStart,
		t.h.hSyncEnd,
		t.h.hTotal,
		t.v.vDisp,
		t.v.vSyncStart,
		t.v.vSyncEnd,
		t.v.vTotal,
		(t.interlace == 'I') ? "Interlace" : "",
		t.hSyncPol,
		t.vSyncPol);
}
#endif  /* TESTING */

#endif /* VIDEO_FBDev */
