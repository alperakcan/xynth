
#include "toolkit.h"

#define JRGB(_r,_g,_b)  (_r << 16 | _g << 8 | (_b))
#define JRED(_rgb)      (((_rgb) & 0xff0000) >> 16)
#define JGREEN(_rgb)    (((_rgb) & 0x00ff00) >> 8)
#define JBLUE(_rgb)     ((_rgb) & 0x0000ff)

jint Java_java_awt_Toolkit_clrGetPixelValue (JNIEnv *env, jclass clazz, jint rgb)
{
	jint pix;
	DEBUGF("Enter");
	pix = s_rgbolor(xynth->root->surface, JRED(rgb), JGREEN(rgb), JBLUE(rgb));
	DEBUGF("Leave");
	return pix;
}
