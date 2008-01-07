/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2008 by Alper Akcan
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

#ifndef JS_H_
#define JS_H_

JSObject * js_newObjectData (void *data);
void js_nativeObjectDraw (w_object_t *object);
void js_nativeButtonPressed (w_object_t *object, int button);
void js_nativeButtonReleased (w_object_t *object, int button);
void js_nativeButtonClicked (w_object_t *object, int button, int count);
JSBool js_objectFillBox (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_objectGetWidth (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_objectGetHeight (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_objectHide (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_objectShow (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_objectMove (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_buttonDraw (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_textboxSetStr (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool js_getElementById (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
int js_init (s_hashtable_t *htable, char *script_file);
void js_uninit (void);

#endif /*JS_H_*/
