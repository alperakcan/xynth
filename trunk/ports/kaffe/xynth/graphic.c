
#include "toolkit.h"

jobject Java_java_awt_Toolkit_graInitGraphics (JNIEnv *env, jclass clazz UNUSED,
                                               jobject ngr, jobject tgt, jint tgtType,
					       jint xOff, jint yOff,
					       jint xClip, jint yClip, jint wClip, jint hClip,
					       jobject fnt, jint fg, jint bg, jboolean blank)
{
	s_rect_t coor;
	s_rect_t inter;
	s_surface_t *srf;
	graphics_t *gr = NULL;
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
			case 2: /* graphics */
				srf = ((graphics_t *) tgtPtr)->surface;
				break;
			case 1: /* image */
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
		srf = gr->surface;
	} else {
		gr = (graphics_t *) AWT_MALLOC(sizeof(graphics_t));
		ngr = JCL_NewRawDataObject(env, gr);
	}
	wClip = (wClip > 0) ? wClip : 0;
	hClip = (hClip > 0) ? hClip : 0;
	gr->font = UNVEIL_FONT(fnt);
	gr->surface = srf;
	gr->fg = fg;
	gr->bg = bg;
	gr->x0 = xOff;
	gr->y0 = yOff;
	gr->clip.x = gr->x0 + xClip;
	gr->clip.y = gr->y0 + yClip;
	gr->clip.w = wClip;
	gr->clip.h = hClip;
	if (blank) {
		coor.x = gr->x0 + xClip;
		coor.y = gr->y0 + yClip;
		coor.w = wClip;
		coor.h = hClip;
		if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
			s_fillbox(gr->surface, inter.x, inter.y, inter.w, inter.h, bg);
		}
	}
	DEBUGF("Leave");
	return ngr;
}

void Java_java_awt_Toolkit_graSetClip (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jint xClip, jint yClip, jint wClip, jint hClip)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	DEBUGF("set clip; xClip: %d, yClip: %d, wClip: %d, hClip: %d", xClip, yClip, wClip, hClip);
	gr->clip.x = gr->x0 + xClip;
	gr->clip.y = gr->y0 + yClip;
	gr->clip.w = (wClip > 0) ? wClip : 0;
	gr->clip.h = (hClip > 0) ? hClip : 0;
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
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	if ((width >= 0) && (height >= 0)) {
		coor.x = gr->x0 + x;
		coor.y = gr->y0 + y;
		coor.w = width;
		coor.h = height;
		if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
			s_fillbox(gr->surface, inter.x, inter.y, inter.w, inter.h, gr->fg);
		}
	}
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graFreeGraphics (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	AWT_FREE(gr);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graSetVisible (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jint isVisible)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	DEBUGF("setvisible: %d", isVisible);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graClearRect (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr, jint x, jint y, jint width, jint height)
{
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	coor.x = gr->x0 + x;
	coor.y = gr->y0 + y;
	coor.w = width;
	coor.h = height;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_fillbox(gr->surface, inter.x, inter.y, inter.w, inter.h, gr->bg);
	}
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDrawImageScaled (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jobject nimg,
                                               jint dx0, jint dy0, jint dx1, jint dy1,
					       jint sx0, jint sy0, jint sx1, jint sy1, jint bgval)
{
	int x0;
	int y0;
	int x1;
	int y1;
	int iw;
	char *vbufs;
	char *vbufi;
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	s_image_t *img;
	s_surface_t *srfs;
	s_surface_t *srfi;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	img = UNVEIL_IMG(nimg);
	if (dx1 > dx0) { x0 = dx0; x1 = dx1;
	} else {         x0 = dx1; x1 = dx0;
	}
	if (dy1 > dy0) { y0 = dy0; y1 = dy1;
	} else {         y0 = dy1; y1 = dy0;
	}
	iw = img->w;
	if (sx0 < 0) sx0 = 0;
	if (sx1 < 0) sx1 = 0;
	if (sx0 >= iw) sx0 = iw - 1;
	if (sx1 >= iw) sx1 = iw - 1;
	srfs = (s_surface_t *) AWT_MALLOC(sizeof(s_surface_t));
	vbufs = (char *) AWT_MALLOC((x1 - x0 + 1) * (y1 - y0 + 1) * gr->surface->bytesperpixel);
	s_getsurfacevirtual(srfs, x1 - x0 + 1, y1 - y0 + 1, gr->surface->bitsperpixel, vbufs);
	srfi = (s_surface_t *) AWT_MALLOC(sizeof(s_surface_t));
	vbufi = (char *) AWT_MALLOC(img->w * img->h * gr->surface->bytesperpixel);
	s_getsurfacevirtual(srfi, img->w, img->h, gr->surface->bitsperpixel, vbufi);
	s_fillbox(srfi, 0, 0, img->w, img->h, gr->bg);
	s_putboxrgba(srfi, 0, 0, img->w, img->h, img->rgba);
	s_scalebox(gr->surface, srfi->width, srfi->height, srfi->vbuf, srfs->width, srfs->height, srfs->vbuf);
	coor.x = gr->x0 + x0;
	coor.y = gr->y0 + y0;
	coor.w = srfs->width;
	coor.h = srfs->height;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_putboxpart(gr->surface, inter.x, inter.y, inter.w, inter.h, srfs->width, srfs->height, srfs->vbuf, inter.x - coor.x, inter.y - coor.y);
	}
	AWT_FREE(vbufs);
	AWT_FREE(vbufi);
	AWT_FREE(srfs);
	AWT_FREE(srfi);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDraw3DRect (JNIEnv *env, jclass clazz, jobject ngr,
                                          jint x, jint y, jint width, jint height,
					  jboolean raised, jint rgb)
{
	int xw;
	int yh;
	int dark;
	int bright;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	dark = (int) (Java_java_awt_Toolkit_clrDark(env, clazz, rgb) >> 32);
	bright = (int) (Java_java_awt_Toolkit_clrBright(env, clazz, rgb) >> 32);
	x += gr->x0;
	y += gr->y0;
	xw = x + width;
	yh = y + height;
	s_hline(gr->surface, x, y, xw - 1, (raised) ? bright : dark);
	s_vline(gr->surface, x, y, yh, (raised) ? bright : dark);
	s_hline(gr->surface, x + 1, yh, xw, (raised) ? dark : bright);
	s_vline(gr->surface, xw, y, yh, (raised) ? dark : bright);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graFill3DRect (JNIEnv *env, jclass clazz, jobject ngr,
                                          jint x, jint y, jint width, jint height,
					  jboolean raised, jint rgb)
{
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	coor.x = gr->x0 + x + 1;
	coor.y = gr->y0 + y + 1;
	coor.w = width - 2;
	coor.h = height - 2;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_fillbox(gr->surface, inter.x, inter.y, inter.w, inter.h, gr->fg);
	}
	Java_java_awt_Toolkit_graDraw3DRect(env, clazz, ngr, x, y, width - 1, height - 1, raised, rgb);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDrawString (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jstring jstr, jint x, jint y)
{
	int r;
	int g;
	int b;
	char *str;
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	str = java2CString(env, jstr);
	DEBUGF("string: %s", str);
	s_font_set_str(gr->font, str);
	s_colorrgb(gr->surface, gr->fg, &r, &g, &b);
	s_font_set_rgb(gr->font, r, g, b);
	s_font_get_glyph(gr->font);
	coor.x = gr->x0 + x;
	coor.y = gr->y0 + y - gr->font->yMax;
	coor.w = gr->font->img->w;
	coor.h = gr->font->img->h;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_putboxpartrgba(gr->surface, inter.x, inter.y, inter.w, inter.h, coor.w, coor.h, gr->font->img->rgba, inter.x - coor.x, inter.y - coor.y);
	}
	AWT_FREE(str);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDrawLine (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr, jint x1, jint y1, jint x2, jint y2)
{
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	if ((x1 == x2) && (y1 == y2)) {
		s_setpixel(gr->surface, gr->x0 + x1, gr->y0 + y1, gr->fg);
	} else if (x1 == x2) {
		s_vline(gr->surface, gr->x0 + x1, gr->y0 + y1, gr->y0 + y2, gr->fg);
	} else if (y1 == y2) {
		s_hline(gr->surface, gr->x0 + x1, gr->y0 + y1, gr->x0 + x2, gr->fg);
	} else {
		s_line(gr->surface, gr->x0 + x1, gr->y0 + y1, gr->x0 + x2, gr->y0 + y2, gr->fg);
	}
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDrawChars (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jcharArray jChars, jint offset, jint len, jint x, jint y)
{
	int i;
	int n;
	int r;
	int g;
	int b;
	char *str;
	jchar *jc;
	jchar *jco;
	jint isCopy;
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	if (!jChars) {
		return;
	}
	gr = UNVEIL_GRAP(ngr);
	n = (*env)->GetArrayLength(env, jChars);
	jc = (*env)->GetCharArrayElements(env, jChars, &isCopy);
	jco = jc + offset;
	if (offset + len > n) {
		n = n - offset;
	} else {
		n = len;
	}
	if (n <= 0) {
		return;
	}
	str = (char *) AWT_MALLOC(sizeof(char) * (n + 1));
	for (i = 0; i < n; i++) {
		*(str + i) = *(jco + i);
	}
	*(str + i) = '\0';
	DEBUGF("string: %s", str);
	s_font_set_str(gr->font, str);
	s_colorrgb(gr->surface, gr->fg, &r, &g, &b);
	s_font_set_rgb(gr->font, r, g, b);
	s_font_get_glyph(gr->font);
	coor.x = gr->x0 + x;
	coor.y = gr->y0 + y - gr->font->yMax;
	coor.w = gr->font->img->w;
	coor.h = gr->font->img->h;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_putboxpartrgba(gr->surface, inter.x, inter.y, inter.w, inter.h, coor.w, coor.h, gr->font->img->rgba, inter.x - coor.x, inter.y - coor.y);
	}
	AWT_FREE(str);
	(*env)->ReleaseCharArrayElements(env, jChars, jc, JNI_ABORT);
}

void Java_java_awt_Toolkit_graCopyArea (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr, jint x, jint y, jint width, jint height, jint xDelta, jint yDelta)
{
	char *vbuf;
	s_rect_t coor;
	s_rect_t inter;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	vbuf = (char *) AWT_MALLOC(width * height * gr->surface->bytesperpixel);
	s_getbox(gr->surface, gr->x0 + x, gr->y0 + y, width, height, vbuf);
	coor.x = gr->x0 + x + xDelta;
	coor.y = gr->y0 + y + yDelta;
	coor.w = width;
	coor.h = height;
	if (s_rect_intersect(&(gr->clip), &coor, &inter) == 0) {
		s_putboxpart(gr->surface, inter.x, inter.y, inter.w, inter.h, coor.w, coor.h, vbuf, inter.x - coor.x, inter.y - coor.y);
	}
	AWT_FREE(vbuf);
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graSetFont (JNIEnv *env, jclass clazz UNUSED, jobject ngr, jobject jfnt)
{
	s_font_t *fnt;
	graphics_t *gr;
	DEBUGF("Enter");
	gr = UNVEIL_GRAP(ngr);
	fnt = UNVEIL_FONT(jfnt);
	gr->font = fnt;
	DEBUGF("Leave");
}

void Java_java_awt_Toolkit_graDrawRect (JNIEnv *env UNUSED, jclass clazz UNUSED, jobject ngr, jint x, jint y, jint width, jint height)
{
	DEBUGF("Enter");
	Java_java_awt_Toolkit_graDrawLine(env, clazz, ngr, x, y, x + width - 1, y);
	Java_java_awt_Toolkit_graDrawLine(env, clazz, ngr, x, y + height - 1, x + width - 1, y + height - 1);
	Java_java_awt_Toolkit_graDrawLine(env, clazz, ngr, x, y, x, y + height - 1);
	Java_java_awt_Toolkit_graDrawLine(env, clazz, ngr, x + width - 1, y, x + width - 1, y);
	DEBUGF("Leave");
}