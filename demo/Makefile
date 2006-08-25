
include ../Makefile.cfg
include ../Makefile.parse

subdirs-$(DEMO_CHILD)    = child
subdirs-$(DEMO_DESKTOP) += desktop
subdirs-$(DEMO_HIDE)    += hide
subdirs-$(DEMO_LOAD)    += load
subdirs-$(DEMO_SIMPLE)  += simple
subdirs-$(DEMO_OBJECT)  += object
subdirs-$(DEMO_TEMP)    += temp
subdirs-$(DEMO_TERM)    += term
subdirs-$(DEMO_TIMER)   += timer
subdirs-$(DEMO_LOGOUT)  += xynthlogout
subdirs-$(DEMO_IMAGE)   += img
subdirs-$(DEMO_MINES)   += mines
subdirs-$(DEMO_WIDGET)  += widget
subdirs-$(DEMO_ONSCREENKEYBOARD) += onscreenkeyboard

all:
	$(foreach F, $(subdirs-Y), $(MAKE) -C $F || exit 1;)

dist:
	$(foreach F, $(subdirs-Y), $(MAKE) dist -C $F || exit 1;)

clean:
	$(foreach F, $(subdirs-Y), $(MAKE) clean -C $F || exit 1;)
	$(foreach F, $(subdirs-N), $(MAKE) clean -C $F || exit 1;)
	$(foreach F, $(subdirs-), $(MAKE) clean -C $F || exit 1;)
