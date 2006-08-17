
include ../Makefile.cfg
include ../Makefile.parse

all:
ifeq ($(DEMO_CHILD), Y)
	$(MAKE) -C child
endif
ifeq ($(DEMO_DESKTOP), Y)
	$(MAKE) -C desktop
endif
ifeq ($(DEMO_HIDE), Y)
	$(MAKE) -C hide
endif
ifeq ($(DEMO_LOAD), Y)
	$(MAKE) -C load
endif
ifeq ($(DEMO_SIMPLE), Y)
	$(MAKE) -C simple
endif
ifeq ($(DEMO_OBJECT), Y)
	$(MAKE) -C object
endif
ifeq ($(DEMO_ONSCREENKEYBOARD), Y)
	$(MAKE) -C onscreenkeyboard
endif
ifeq ($(DEMO_TEMP), Y)
	$(MAKE) -C temp
endif
ifeq ($(DEMO_TERM), Y)
	$(MAKE) -C term
endif
ifeq ($(DEMO_TIMER), Y)
	$(MAKE) -C timer
endif
ifeq ($(DEMO_LOGOUT), Y)
	$(MAKE) -C xynthlogout
endif
ifeq ($(DEMO_IMAGE), Y)
	$(MAKE) -C img
endif
ifeq ($(DEMO_MINES), Y)
	$(MAKE) -C mines
endif
ifeq ($(DEMO_WIDGET), Y)
	$(MAKE) -C widget
endif

dist:
ifeq ($(DEMO_CHILD), Y)
	$(MAKE) dist -C child
endif
ifeq ($(DEMO_DESKTOP), Y)
	$(MAKE) dist -C desktop
endif
ifeq ($(DEMO_HIDE), Y)
	$(MAKE) dist -C hide
endif
ifeq ($(DEMO_LOAD), Y)
	$(MAKE) dist -C load
endif
ifeq ($(DEMO_SIMPLE), Y)
	$(MAKE) dist -C simple
endif
ifeq ($(DEMO_OBJECT), Y)
	$(MAKE) dist -C object
endif
ifeq ($(DEMO_ONSCREENKEYBOARD), Y)
	$(MAKE) dist -C onscreenkeyboard
endif
ifeq ($(DEMO_TEMP), Y)
	$(MAKE) dist -C temp
endif
ifeq ($(DEMO_TERM), Y)
	$(MAKE) dist -C term
endif
ifeq ($(DEMO_TIMER), Y)
	$(MAKE) dist -C timer
endif
ifeq ($(DEMO_LOGOUT), Y)
	$(MAKE) dist -C xynthlogout
endif
ifeq ($(DEMO_IMAGE), Y)
	$(MAKE) dist -C img
endif
ifeq ($(DEMO_MINES), Y)
	$(MAKE) dist -C mines
endif
ifeq ($(DEMO_WIDGET), Y)
	$(MAKE) dist -C widget
endif

clean:
	$(MAKE) clean -C child
	$(MAKE) clean -C load
	$(MAKE) clean -C mines
	$(MAKE) clean -C desktop
	$(MAKE) clean -C simple
	$(MAKE) clean -C temp
	$(MAKE) clean -C term
	$(MAKE) clean -C widget
	$(MAKE) clean -C hide
	$(MAKE) clean -C object
	$(MAKE) clean -C timer
	$(MAKE) clean -C img
	$(MAKE) clean -C xynthlogout
	$(MAKE) clean -C onscreenkeyboard
