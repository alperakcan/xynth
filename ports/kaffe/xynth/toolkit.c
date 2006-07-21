
#include "toolkit.h"

jint Java_java_awt_Toolkit_tlkProperties (JNIEnv *env UNUSED, jclass clazz UNUSED)
{
	jint props;
	DEBUGF("Enter");
	props = TLK_EXTERNAL_DECO;
	DEBUGF("Leave");
	return props;
}

jboolean Java_java_awt_Toolkit_tlkInit (JNIEnv *env UNUSED, jclass clazz UNUSED, jstring name UNUSED)
{
	DEBUGF("Enter");
	xynth = AWT_MALLOC(sizeof(xynth_toolkit_t));
	s_client_init(&(xynth->root));
	xynth->nwindows = XYNTH_NWINDOWS;
	xynth->windows = AWT_CALLOC(xynth->nwindows, sizeof(window_rec_t));
	DEBUGF("Leave");
	return JNI_TRUE;
}

jint Java_java_awt_Toolkit_tlkGetScreenWidth (JNIEnv *env UNUSED, jclass clazz UNUSED)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return xynth->root->surface->linear_buf_width;
}

jint Java_java_awt_Toolkit_tlkGetScreenHeight (JNIEnv *env UNUSED, jclass clazz UNUSED)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return xynth->root->surface->linear_buf_height;
}

jint Java_java_awt_Toolkit_tlkGetResolution (JNIEnv *env UNUSED, jclass clazz UNUSED)
{
	DEBUGF("Enter");
	NYI();
	DEBUGF("Leave");
	return 0;
}

jstring Java_java_awt_Toolkit_tlkVersion (JNIEnv *env, jclass clazz UNUSED)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return (*env)->NewStringUTF(env, "Xynth Windowing System");
}

void Java_java_awt_Toolkit_tlkDisplayBanner (JNIEnv* env UNUSED, jclass clazz UNUSED, jstring banner UNUSED)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
}
