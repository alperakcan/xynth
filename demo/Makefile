include ../Makefile.cfg

all:
	$(MAKE) -C child
	$(MAKE) -C desktop
	$(MAKE) -C simple
	$(MAKE) -C temp
	$(MAKE) -C hide
	$(MAKE) -C object
	$(MAKE) -C timer
	$(MAKE) -C xynthlogout
ifeq ($(PLATFORM_LINUX), Y)
	$(MAKE) -C load
	$(MAKE) -C img
	$(MAKE) -C mines
	$(MAKE) -C widget
	$(MAKE) -C term
else
ifeq ($(PLATFORM_GP2X), Y)
	$(MAKE) -C load
	$(MAKE) -C term
endif
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
