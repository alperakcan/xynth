
#include "toolkit.h"

void Java_java_awt_Toolkit_wndSetFrameInsets (JNIEnv* env UNUSED, jclass clazz UNUSED, jint top, jint left, jint bottom, jint right)
{
	DEBUGF("Enter");
	xynth->frame_insets.top  = top;
	xynth->frame_insets.left = left;
	xynth->frame_insets.bottom = bottom;
	xynth->frame_insets.right = right;
	xynth->frame_insets.guess = 1;
	DEBUGF("Leave");
}
