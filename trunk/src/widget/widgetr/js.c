/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#if defined(WIDGET_SCRIPT_ENGINE_JS)

#include "jsstddef.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jstypes.h"
#include "jsarena.h"
#include "jsutil.h"
#include "jsprf.h"
#include "jsapi.h"
#include "jsatom.h"
#include "jscntxt.h"
#include "jsdbgapi.h"
#include "jsemit.h"
#include "jsfun.h"
#include "jsgc.h"
#include "jslock.h"
#include "jsobj.h"
#include "jsparse.h"
#include "jsscope.h"
#include "jsscript.h"

#include "xynth_.h"
#include "widget.h"

#include "widgetr.h"
#include "code.h"
#include "js.h"

static s_hashtable_t *g_htable;

static JSRuntime *rt;
static JSContext *cx;
static JSObject *global;
static char *script;

static JSClass global_class = {
	"global", 0,
	JS_PropertyStub,JS_PropertyStub,JS_PropertyStub,JS_PropertyStub,
	JS_EnumerateStub,JS_ResolveStub,JS_ConvertStub,JS_FinalizeStub
};
	
static JSFunctionSpec js_functions[] = {
	{"js_getElementById", js_getElementById, 1, 0, 0},
	{"js_objectFillBox", js_objectFillBox, 8, 0, 0},
	{"js_objectHide", js_objectHide, 1, 0, 0},
	{"js_objectShow", js_objectShow, 1, 0, 0},
	{"js_objectMove", js_objectMove, 5, 0, 0},
	{"js_objectGetWidth", js_objectGetWidth, 1, 0, 0},
	{"js_objectGetHeight", js_objectGetHeight, 1, 0, 0},
	{"js_buttonDraw", js_buttonDraw, 1, 0, 0},
	{"js_textboxSetStr", js_textboxSetStr, 2, 0, 0},
	{0,0,0,0,0},
};

code_script_t js_script = {
	.name = "js",
	.object_draw = js_nativeObjectDraw,
	.button_pressed = js_nativeButtonPressed,
	.button_released = js_nativeButtonReleased,
	.button_clicked = js_nativeButtonClicked,
	.init = js_init,
	.uninit = js_uninit
};

JSObject * js_newObjectData (void *data)
{
	JSObject *jobject;
	jobject = JS_NewObject(cx, NULL, NULL, NULL);
	OBJ_GET_CLASS(cx, jobject)->flags |= JSCLASS_HAS_PRIVATE;
	JS_SetPrivate(cx, jobject, data);
	return jobject;
}

void js_nativeObjectDraw (w_object_t *object)
{
	code_priv_t *priv = object->priv; 
	JSBool jok;
	jsval jrval;
	jsval jargv[1];
	JSObject *jobject;
	jobject = js_newObjectData(object);
	jargv[0] = OBJECT_TO_JSVAL(jobject);
	jok = JS_CallFunctionName(cx, global, priv->draw, 1, jargv, &jrval);
}

void js_nativeButtonPressed (w_object_t *object, int button)
{
	code_priv_t *priv = object->priv; 
	JSBool jok;
	jsval jrval;
	jsval jargv[2];
	JSObject *jobject;
	jobject = js_newObjectData(object);
	jargv[0] = OBJECT_TO_JSVAL(jobject);
	jargv[1] = INT_TO_JSVAL(button);
	jok = JS_CallFunctionName(cx, global, priv->pressed, 2, jargv, &jrval);
}

void js_nativeButtonReleased (w_object_t *object, int button)
{
	code_priv_t *priv = object->priv; 
	JSBool jok;
	jsval jrval;
	jsval jargv[2];
	JSObject *jobject;
	jobject = js_newObjectData(object);
	jargv[0] = OBJECT_TO_JSVAL(jobject);
	jargv[1] = INT_TO_JSVAL(button);
	jok = JS_CallFunctionName(cx, global, priv->released, 2, jargv, &jrval);
}

void js_nativeButtonClicked (w_object_t *object, int button, int count)
{
	code_priv_t *priv = object->priv; 
	JSBool jok;
	jsval jrval;
	jsval jargv[3];
	JSObject *jobject;
	jobject = js_newObjectData(object);
	jargv[0] = OBJECT_TO_JSVAL(jobject);
	jargv[1] = INT_TO_JSVAL(button);
	jargv[2] = INT_TO_JSVAL(count);
	jok = JS_CallFunctionName(cx, global, priv->clicked, 3, jargv, &jrval);
}

JSBool js_objectFillBox (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	int wx;
	int wy;
	int ww;
	int wh;
	int wr;
	int wg;
	int wb;
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	jok = JS_ValueToInt32(cx, argv[1], &wx);
	jok = JS_ValueToInt32(cx, argv[2], &wy);
	jok = JS_ValueToInt32(cx, argv[3], &ww);
	jok = JS_ValueToInt32(cx, argv[4], &wh);
	jok = JS_ValueToInt32(cx, argv[5], &wr);
	jok = JS_ValueToInt32(cx, argv[6], &wg);
	jok = JS_ValueToInt32(cx, argv[7], &wb);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	if (wobject == NULL) {
		return 0;
	}
	s_fillbox(wobject->surface, wx, wy, ww, wh, s_rgbcolor(wobject->surface, wr, wg, wb));
	return 1;
}

JSBool js_objectGetWidth (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	*rval = INT_TO_JSVAL(wobject->surface->width);
	return 1;
}

JSBool js_objectGetHeight (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	*rval = INT_TO_JSVAL(wobject->surface->height);
	return 1;
}

JSBool js_objectHide (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	w_object_hide(wobject);
	return 1;
}

JSBool js_objectShow (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	w_object_show(wobject);
	return 1;
}

JSBool js_objectMove (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	int wx;
	int wy;
	int ww;
	int wh;
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	jok = JS_ValueToInt32(cx, argv[1], &wx);
	jok = JS_ValueToInt32(cx, argv[2], &wy);
	jok = JS_ValueToInt32(cx, argv[3], &ww);
	jok = JS_ValueToInt32(cx, argv[4], &wh);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	w_object_move(wobject, wx, wy, ww, wh);
	return 1;
}

JSBool js_buttonDraw (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	w_button_draw(wobject);
	return 1;
}

JSBool js_textboxSetStr (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSBool jok;
	JSString *jstr;
	JSObject *robject;
	w_object_t *wobject;
	jok = JS_ValueToObject(cx, argv[0], &robject);
	jstr = JS_ValueToString(cx, argv[1]);
	wobject = (w_object_t *) JS_GetPrivate(cx, robject);
	if (wobject == NULL) {
		return 0;
	}
	w_textbox_set_str(wobject, JS_GetStringBytes(jstr));
	return 1;
}

JSBool js_getElementById (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	char *str;
	JSObject *jobject;
	JSString *jstr;
	w_object_t *wobject;
	jstr = JS_ValueToString(cx, argv[0]);
	str = JS_GetStringBytes(jstr);
	wobject = s_hashtable_get_data(g_htable, str);
	jobject = js_newObjectData(wobject);
	*rval = OBJECT_TO_JSVAL(jobject);
	return 1;
}

int js_init (s_hashtable_t *htable, char *script_file)
{
	JSBool ok;
	jsval rval;

	FILE *fp;
	struct stat stbuf;

	if (stat(script_file, &stbuf)) {
		return -1;
	}
	script = (char *) s_malloc(stbuf.st_size + 1);
	memset(script, 0, stbuf.st_size + 1);
	fp = fopen(script_file, "r");
	fread(script, 1, stbuf.st_size, fp);
	fclose(fp);

    	g_htable = htable;
	
	rt = JS_NewRuntime(1L * 1024L * 1024L);
	cx = JS_NewContext(rt, 8192);
	global = JS_NewObject(cx, &global_class, NULL, NULL);
	JS_InitStandardClasses(cx, global);

	ok = JS_DefineFunctions(cx, global, js_functions);
	ok = JS_EvaluateScript(cx, global, script, strlen(script), NULL, 0, &rval);
	
	return 0;
}

void js_uninit (void)
{
	JS_DestroyContext(cx);
	JS_DestroyRuntime(rt);
	s_free(script);
}

#endif /* WIDGET_SCRIPT_ENGINE_JS */
