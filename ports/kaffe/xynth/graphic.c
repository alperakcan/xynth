
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
			case 1: /* image */
			case 2: /* graphics */
			default:
				break;
		}
	} else {
		DEBUGF("tgt == 0");
	}
	if (gr != NULL) {
		DEBUGF("gr != NULL");
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
