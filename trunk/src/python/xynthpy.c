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

extern PyTypeObject p_event_type;
extern PyTypeObject p_window_type;

PyMethodDef xynth_methods[] = {
	{"window", wrap_window, 1},
//	{"event", wrap_event, 1},
	{NULL, NULL}
};

void initxynth (void)
{
	PyObject *m;

	if (PyType_Ready(&p_window_type) < 0) {
		return;
	}
	p_window_type.ob_type = &PyType_Type;
	m = Py_InitModule3("xynth", xynth_methods, "Xynth Windowing System");
	PyModule_AddIntConstant(m, "WINDOW_NOFORM", WINDOW_NOFORM);
	PyModule_AddIntConstant(m, "WINDOW_MAIN", WINDOW_MAIN);
	Py_INCREF(&p_window_type);
	PyModule_AddObject(m, "window", (PyObject *) ((void *) (&p_window_type)));
//	PyModule_AddObject(m, "event", (PyObject *) ((void *) (&p_event_type)));
}
