
#include "toolkit.h"

jclass     ComponentEvent;
jclass     MouseEvent;
jclass     FocusEvent;
jclass     WindowEvent;
jclass     KeyEvent;
jclass     PaintEvent;
jclass     WMEvent;

jmethodID  getComponentEvent;
jmethodID  getMouseEvent;
jmethodID  getFocusEvent;
jmethodID  getWindowEvent;
jmethodID  getKeyEvent;
jmethodID  getPaintEvent;
jmethodID  getWMEvent;

jobject Java_java_awt_Toolkit_evtInit (JNIEnv* env, jclass clazz UNUSED)
{
	jclass Component;
	
	DEBUGF("Enter");
	
	if (ComponentEvent != NULL) {
		DEBUGF("evtInit called twice");
		return NULL;
	}
	
	ComponentEvent = (*env)->FindClass( env, "java/awt/ComponentEvt");
	assert(ComponentEvent != NULL);
	MouseEvent     = (*env)->FindClass( env, "java/awt/MouseEvt");
	assert(MouseEvent != NULL);
	FocusEvent     = (*env)->FindClass( env, "java/awt/FocusEvt");
	assert(FocusEvent != NULL);
	WindowEvent    = (*env)->FindClass( env, "java/awt/WindowEvt");
	assert(WindowEvent != NULL);  
	KeyEvent       = (*env)->FindClass( env, "java/awt/KeyEvt");
	assert(KeyEvent != NULL);
	PaintEvent     = (*env)->FindClass( env, "java/awt/PaintEvt");
	assert(PaintEvent != NULL);
	WMEvent        = (*env)->FindClass( env, "java/awt/WMEvent");
	assert(WMEvent != NULL);
	
	getComponentEvent = (*env)->GetStaticMethodID( env, ComponentEvent, "getEvent", "(IIIIII)Ljava/awt/ComponentEvt;");
	getMouseEvent     = (*env)->GetStaticMethodID( env, MouseEvent, "getEvent", "(IIIII)Ljava/awt/MouseEvt;");
	getFocusEvent     = (*env)->GetStaticMethodID( env, FocusEvent, "getEvent", "(IIZ)Ljava/awt/FocusEvt;");
	getWindowEvent    = (*env)->GetStaticMethodID( env, WindowEvent, "getEvent", "(II)Ljava/awt/WindowEvt;");
	getKeyEvent       = (*env)->GetStaticMethodID( env, KeyEvent, "getEvent", "(IIIII)Ljava/awt/KeyEvt;");
	getPaintEvent     = (*env)->GetStaticMethodID( env, PaintEvent, "getEvent", "(IIIIII)Ljava/awt/PaintEvt;");
	getWMEvent        = (*env)->GetStaticMethodID( env, WMEvent, "getEvent", "(II)Ljava/awt/WMEvent;");
	
	Component = (*env)->FindClass( env, "java/awt/Component");

	DEBUGF("Leave");

	return (*env)->NewObjectArray( env, xynth->nwindows, Component, NULL);
}

jint Java_java_awt_Toolkit_evtRegisterSource (JNIEnv* env UNUSED, jclass clazz UNUSED, jobject nativeWnd)
{
	int id;
	DEBUGF("Enter");
	s_window_t *window;
	window = UNVEIL_WINDOW(nativeWnd);
	id = source_idx_get(xynth, window);
	DEBUGF("Leave");
	return id;
}

jobject Java_java_awt_Toolkit_evtGetNextEvent (JNIEnv* env, jclass clazz)
{
	sleep(1);
//	DEBUGF("Enter");
//	while (1) {
//		sleep(1);
//	}
//	DEBUGF("Leave");
	return NULL;
}
