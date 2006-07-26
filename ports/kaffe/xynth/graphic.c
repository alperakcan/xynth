
#include "toolkit.h"

jobject Java_java_awt_Toolkit_graInitGraphics (JNIEnv* env, jclass clazz UNUSED,
                                               jobject ngr, jobject tgt, jint tgtType,
					       	jint xOff, jint yOff,
						jint xClip, jint yClip, jint wClip, jint hClip,
						jobject fnt, jint fg, jint bg, jboolean blank)
{
	graphics_t *gr = NULL;
	s_surface_t *srf;
	DEBUGF("Enter");
	if (ngr != NULL) {
		gr = UNVEIL_GRAP(ngr);
	}
	DEBUGF("xoff: %d, yoff: %d, xclip: %d, yclip: %d, wclip: %d, hclip: %d", xOff, yOff, xClip, yClip, wClip, hClip);
	if (tgt) {
		void *tgtPtr = JCL_GetRawData(env, tgt);
		DEBUGF("tgttype: %s", (tgtType == 0) ? "window" :
		                      (tgtType == 1) ? "Image" :
				      (tgtType == 2) ? "graphics" : "unknown");
		switch (tgtType) {
			case 0: /* window */
				srf = ((s_window_t *) tgtPtr)->surface;
				break;
			case 1: /* image */
			case 2: /* graphics */
			default:
				exit(1);
				break;
		}
	} else {
		DEBUGF("tgt == 0");
		if (gr) {
			srf = gr->surface;
		} else {
			DEBUGF("attempt to set an uninitialized graphics");
			exit(1);
		}
	}
	if (gr != NULL) {
		DEBUGF("gr != NULL");
		srf = gr->surface;
	} else {
		gr = (graphics_t *) AWT_MALLOC(sizeof(graphics_t));
		ngr = JCL_NewRawDataObject(env, gr);
	}
	gr->surface = srf;
	gr->fg = fg;
	gr->bg = bg;
	gr->x0 = xOff;
	gr->y0 = yOff;
	if (blank) {
		s_fillbox(gr->surface, xOff + xClip, yOff + yClip, wClip, hClip, bg);
	}
	DEBUGF("Leave");
	return ngr;
}

void Java_java_awt_Toolkit_graSetClip (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jint xClip, jint yClip, jint wClip, jint hClip)
{
	graphics_t *gr;
	s_rect_t rect;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	DEBUGF("set clip; xClip: %d, yClip: %d, wClip: %d, hClip: %d", xClip, yClip, wClip, hClip);
	rect.x = xClip;
	rect.y = yClip;
	rect.w = (wClip > 0) ? wClip : 0;
	rect.h = (hClip > 0) ? hClip : 0;
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graSetColor (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jint clr)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	gr->fg = clr;
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graFillRect (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr, jint x, jint y, jint width, jint height)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	if ((width >= 0) && (height >= 0)) {
		DEBUGF("fill rect; x: %d, y: %d, w: %d, h: %d", x + gr->x0, y + gr->y0, width, height);
		s_fillbox(gr->surface, x + gr->x0, y + gr->y0, width, height, gr->fg);
	}
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graFreeGraphics (JNIEnv* env UNUSED, jclass clazz UNUSED, jobject ngr)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	AWT_FREE(gr);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graSetVisible (JNIEnv* env, jclass clazz UNUSED, jobject ngr, jint isVisible)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	DEBUGF("setvisible: %d", isVisible);
	DEBUGF("Leave");
}
