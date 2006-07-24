#if 0
Java_java_awt_Toolkit_graInitGraphics ( JNIEnv* env, jclass clazz UNUSED,
					jobject ngr, jobject tgt, jint tgtType,
					jint xOff, jint yOff,
					jint xClip, jint yClip, jint wClip, jint hClip,
					jobject fnt, jint fg, jint bg, jboolean blank )
{
  Graphics *gr = NULL;
  Drawable       drw;
  XGCValues      values;
  XRectangle     rect;
#ifdef KAFFE_I18N
  unsigned long  valueMask = GCForeground | GCBackground | GCFunction;
#else  
  unsigned long  valueMask = GCForeground | GCBackground | GCFont | GCFunction;
#endif  

  if (ngr != NULL)
    gr = UNVEIL_GR(ngr);

  DBG( AWT_GRA, printf("initGraphics: %p (%p, %d) %d,%d  %d,%d - %d,%d  %p %x %x\n",
				 gr, tgt,tgtType, xOff,yOff, xClip,yClip,wClip,hClip, fnt,fg,bg));

  if ( tgt ) {
    void *tgtPtr = JCL_GetRawData(env, tgt);
	switch ( tgtType ) {
	case 0:  drw = (Drawable)tgtPtr; break;          /* Window   */
	case 1:  drw = ((Image*)tgtPtr)->pix; break;     /* Image    */
	case 2:  drw = ((Graphics*)tgtPtr)->drw; break;  /* Graphics */
	default: fprintf( stderr, "illegal Graphics target: %d\n", tgtType);
		 drw = 0;
	}
  }
  else {
	if ( gr )
	  drw = gr->drw;
	else {
	  fprintf( stderr, "attempt to set an uninitialized graphics");
	  drw = 0;
	}
  }

  values.foreground = fg;
  values.background = bg;
#ifndef KAFFE_I18N /* !KAFFE_I18N */ 
  values.font       = UNVEIL_FS(fnt)->fid;
#endif  
  values.function   = GXcopy;
  
  if ( gr ) {
	XChangeGC( X->dsp, gr->gc, valueMask, &values);
  }
  else {
	gr = (Graphics*) AWT_MALLOC( sizeof(Graphics));
	gr->gc = XCreateGC( X->dsp, drw, valueMask, &values);

	ngr = JCL_NewRawDataObject(env, gr);

	DBG( AWT_GRA, printf(" ->gr: %p (%p)\n", gr, gr->gc));
  }

  rect.x      = xClip; 
  rect.y      = yClip;
  rect.width  = (wClip > 0) ? wClip : 0;
  rect.height = (hClip > 0) ? hClip : 0;
  XSetClipRectangles( X->dsp, gr->gc, xOff, yOff, &rect, 1, Unsorted);

  gr->fg   = fg;
  gr->bg   = bg;
  gr->drw  = drw;
  gr->x0   = xOff;
  gr->y0   = yOff;
  gr->xor  = 0;
#ifdef KAFFE_I18N
  gr->oc   = UNVEIL_XOC(fnt);
#endif  

  if ( blank ) {
	XSetForeground( X->dsp, gr->gc, gr->bg);
	XFillRectangle( X->dsp, gr->drw, gr->gc, xOff+xClip, yOff+yClip, wClip, hClip);
	XSetForeground( X->dsp, gr->gc, gr->fg);	
  }

  return ngr;
}
#endif


#include "toolkit.h"

jobject Java_java_awt_Toolkit_graInitGraphics (JNIEnv* env, jclass clazz UNUSED,
                                               jobject ngr, jobject tgt, jint tgtType,
					       	jint xOff, jint yOff,
						jint xClip, jint yClip, jint wClip, jint hClip,
						jobject fnt, jint fg, jint bg, jboolean blank)
{
	graphics_t *gr = NULL;
	DEBUGF("Enter");
	if (ngr != NULL) {
		gr = UNVEIL_GRAP(ngr);
	}
	
	if (tgt) {
		void *tgtPtr = JCL_GetRawData(env, tgt);
		DEBUGF("tgttype: %s", (tgtType == 0) ? "window" :
		                      (tgtType == 1) ? "Image" :
				      (tgtType == 2) ? "graphics" : "unknown");
	}
	DEBUGF("Leave");
	return 0;
}
