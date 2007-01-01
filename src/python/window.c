/***************************************************************************
    begin                : Sat Jul 2 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#include "xynthpy.h"

typedef struct p_window_s {
	PyObject_HEAD
	s_window_t *window;
	PyObject *atevent;
	PyObject *atexit;
} p_window_t;

PyMethodDef p_window_methods[] = {
	{"set_title", p_window_set_title, 1},
	{"show", p_window_show, 1},
	{"main", p_window_main, 1},
	{"form_draw", p_window_form_draw, 1},
	{"hide", p_window_hide, 1},
	{"set_coor", p_window_set_coor, 1},
	{"set_resizeable", p_window_set_resizeable, 1},
	{"set_alwaysontop", p_window_set_alwaysontop, 1},
	{"atevent", p_window_atevent, 1},
	{"atexit", p_window_atexit, 1},
	{NULL, NULL}
};

PyGetSetDef p_window_getseters[] = {
	{NULL}
};

PyTypeObject p_window_type = {
	PyObject_HEAD_INIT(NULL)
	0,                         /* ob_size */
	"p_window",                /* tp_name */
	sizeof(p_window_t),        /* tp_basicsize */
	0,                         /* tp_itemsize */
	p_window_dealloc,          /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_CHECKTYPES |
	Py_TPFLAGS_BASETYPE,       /* tp_flags*/
	"p_window object",         /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	p_window_methods,          /* tp_methods */
	0,                         /* tp_members */
	p_window_getseters,        /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,                         /* tp_init */
	0,                         /* tp_alloc */
	p_window_new,              /* tp_new */
};

void p_window_dealloc (PyObject *obj)
{
	p_window_t *window = (p_window_t *) obj;
	s_window_exit(window->window);
	obj->ob_type->tp_free(obj);
}

PyObject * p_window_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return wrap_window(NULL, args);
}

PyObject * p_window_set_title (PyObject *self, PyObject *args)
{
	char *title;
	p_window_t *window = (p_window_t *) self;
	if (!PyArg_ParseTuple(args, "s:p_window_set_title", &title)) {
		return NULL;
	}
	s_window_set_title(window->window, title);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_show (PyObject *self, PyObject *args)
{
	p_window_t *window = (p_window_t *) self;
	s_window_show(window->window);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_main (PyObject *self, PyObject *args)
{
	p_window_t *window = (p_window_t *) self;
	s_window_main(window->window);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_hide (PyObject *self, PyObject *args)
{
	p_window_t *window = (p_window_t *) self;
	s_window_hide(window->window);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_form_draw (PyObject *self, PyObject *args)
{
	p_window_t *window = (p_window_t *) self;
	s_window_form_draw(window->window);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_set_coor (PyObject *self, PyObject *args)
{
	int i[5];
	p_window_t *window = (p_window_t *) self;
	if (!PyArg_ParseTuple(args, "iiiii:p_window_set_coor", &i[0], &i[1], &i[2], &i[3], &i[4])) {
		return NULL;
	}
	s_window_set_coor(window->window, i[0], i[1], i[2], i[3], i[4]);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_set_resizeable (PyObject *self, PyObject *args)
{
	int i;
	p_window_t *window = (p_window_t *) self;
	if (!PyArg_ParseTuple(args, "i:p_window_set_resizeable", &i)) {
		return NULL;
	}
	s_window_set_resizeable(window->window, i);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_set_alwaysontop (PyObject *self, PyObject *args)
{
	int i;
	p_window_t *window = (p_window_t *) self;
	if (!PyArg_ParseTuple(args, "i:p_window_set_alwaysontop", &i)) {
		return NULL;
	}
	s_window_set_alwaysontop(window->window, i);
	return Py_BuildValue("i", 0);
}

PyObject * p_window_atevent (PyObject *self, PyObject *args)
{
	PyObject *temp;
	p_window_t *window = (p_window_t *) self;

	if (PyArg_ParseTuple(args, "O:p_window_atevent", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return Py_BuildValue("i", -1);
		}
		Py_XINCREF(temp);
		Py_XDECREF(window->atevent);
		window->atevent = temp;
		Py_INCREF(Py_None);
		return Py_BuildValue("i", 0);
	}
	return Py_BuildValue("i", -1);
}

PyObject * p_window_atexit (PyObject *self, PyObject *args)
{
	PyObject *temp;
	p_window_t *window = (p_window_t *) self;

	if (PyArg_ParseTuple(args, "O:p_window_atexit", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return Py_BuildValue("i", -1);
		}
		Py_XINCREF(temp);
		Py_XDECREF(window->atexit);
		window->atexit = temp;
		Py_INCREF(Py_None);
		return Py_BuildValue("i", 0);
	}
	return Py_BuildValue("i", -1);
}

void p_client_atexit (s_window_t *xwin)
{
	PyObject *arg;
	PyObject *res;
	p_window_t *pwi;

	pwi = (p_window_t *) xwin->data;
	arg = Py_BuildValue("(O)", pwi);
	if (pwi->atexit != NULL) {
		res = PyEval_CallObject(pwi->atexit, arg);
	}
	Py_DECREF(arg);
	pwi->window = NULL;
}

void p_client_atevent (s_window_t *xwin, s_event_t *xevn)
{
	PyObject *arg;
	PyObject *res;
	p_window_t *pwi;

	pwi = (p_window_t *) xwin->data;
	arg = Py_BuildValue("(OO)", pwi, xevn);
	if (pwi->atevent != NULL) {
		res = PyEval_CallObject(pwi->atevent, arg);
	}
	Py_DECREF(arg);
}

PyObject * wrap_window (PyObject *self, PyObject *args)
{
	int res;
	S_WINDOW wtype;
	p_window_t *window;

	wtype = WINDOW_MAIN;
	if (!PyArg_ParseTuple(args, "|i:wrap_window", &wtype)) {
		return NULL;
	}

	window = PyObject_New(p_window_t, &p_window_type);
	window->atevent = NULL;
	window->atexit = NULL;
	
	res = s_window_init(&(window->window));
	if (res) {
		PyErr_SetString(PyExc_TypeError, "s_window_init failed.");
		return NULL;
	}
	res = s_window_new(window->window, wtype, NULL);
	if (res) {
		PyErr_SetString(PyExc_TypeError, "s_window_new failed.");
		Py_DECREF((PyObject *) window);
		return NULL;
	}
	window->window->data = window;

	s_window_atexit(window->window, p_client_atexit);
	s_window_atevent(window->window, p_client_atevent);
	
	return (PyObject *) window;
}
