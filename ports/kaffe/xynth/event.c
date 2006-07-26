
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

typedef jobject (*event_handler) (JNIEnv *, s_event_t *);

typedef enum {
	JNONE_EVENT   = 0x0,
	JQUIT_EVENT   = 0x1,
	JKEYBD_EVENT  = 0x2,
	JMOUSE_EVENT  = 0x3,
	JEXPOSE_EVENT = 0x4
} S_JEVENT;

S_JEVENT event_handler_number (s_event_t *event)
{
	S_EVENT event_type;
	event_type = event->type & (QUIT_EVENT |
	                            KEYBD_EVENT |
				    MOUSE_EVENT |
				    EXPOSE_EVENT);
	switch (event_type) {
		case QUIT_EVENT:  return JQUIT_EVENT;
		case KEYBD_EVENT: return JKEYBD_EVENT;
		case MOUSE_EVENT: return JMOUSE_EVENT;
		case EXPOSE_EVENT:return JEXPOSE_EVENT;
	}
	DEBUGF("unknown event: 0x08x", event->type); 
	return JNONE_EVENT;
}

jobject event_handler_none (JNIEnv *env, s_event_t *event)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return NULL;
}

jobject event_handler_quit (JNIEnv *env, s_event_t *event)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return NULL;
}

jobject event_handler_keybd (JNIEnv *env, s_event_t *event)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return NULL;
}

jobject event_handler_mouse (JNIEnv *env, s_event_t *event)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return NULL;
}

jobject event_handler_expose (JNIEnv *env, s_event_t *event)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return NULL;
}

event_handler process_event[] = {
	event_handler_none,
	event_handler_quit,
	event_handler_keybd,
	event_handler_mouse,
	event_handler_expose
};

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
//	sleep(1);
//	DEBUGF("Enter");
//	while (1) {
//		sleep(1);
//	}
//	DEBUGF("Leave");
	return NULL;
}
