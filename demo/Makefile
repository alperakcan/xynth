
include ../Makefile.cfg
include ../Makefile.parse

DIR = src/demo/

SUBDIR-$(DEMO_CHILD)    = child
SUBDIR-$(DEMO_DESKTOP) += desktop
SUBDIR-$(DEMO_HIDE)    += hide
SUBDIR-$(DEMO_LOAD)    += load
SUBDIR-$(DEMO_SIMPLE)  += simple
SUBDIR-$(DEMO_OBJECT)  += object
SUBDIR-$(DEMO_TEMP)    += temp
SUBDIR-$(DEMO_TERM)    += term
SUBDIR-$(DEMO_TIMER)   += timer
SUBDIR-$(DEMO_LOGOUT)  += xynthlogout
SUBDIR-$(DEMO_IMAGE)   += img
SUBDIR-$(DEMO_ONSCREENKEYBOARD) += onscreenkeyboard
SUBDIR-$(DEMO_WIDGETX) += widgetx

include ../Makefile.rules

dist:
	@$(foreach F, $(SUBDIR-Y), $(MAKE) dist -C $F || exit 1;)
