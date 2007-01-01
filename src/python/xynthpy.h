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

#include <Python.h>
#include <structmember.h>
#include "xynth.h"

/* event.c */
void p_event_dealloc (PyObject *obj);
PyObject * p_event_new (PyTypeObject *type, PyObject *args, PyObject *kwds);
PyObject * wrap_event (PyObject *self, PyObject *args);

/* window.c */
void p_window_dealloc (PyObject *obj);
PyObject * p_window_new (PyTypeObject *type, PyObject *args, PyObject *kwds);
PyObject * p_window_set_title (PyObject *self, PyObject *args);
PyObject * p_window_show (PyObject *self, PyObject *args);
PyObject * p_window_main (PyObject *self, PyObject *args);
PyObject * p_window_hide (PyObject *self, PyObject *args);
PyObject * p_window_form_draw (PyObject *self, PyObject *args);
PyObject * p_window_set_coor (PyObject *self, PyObject *args);
PyObject * p_window_set_resizeable (PyObject *self, PyObject *args);
PyObject * p_window_set_alwaysontop (PyObject *self, PyObject *args);
PyObject * p_window_atevent (PyObject *self, PyObject *args);
PyObject * p_window_atexit (PyObject *self, PyObject *args);
PyObject * wrap_window (PyObject *self, PyObject *args);

/* xynthpy.c */
void initxynth (void);
