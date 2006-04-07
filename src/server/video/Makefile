include ../../../Makefile.cfg

all:
ifeq ($(VIDEO_HELPER), Y)
	$(MAKE) -C helper
endif
ifeq ($(VIDEO_NULL), Y)
	$(MAKE) -C null
endif
ifeq ($(VIDEO_FBDev), Y)
	$(MAKE) -C fb
endif
ifeq ($(VIDEO_SDL), Y)
	$(MAKE) -C sdl
endif
ifeq ($(VIDEO_GDI), Y)
	$(MAKE) -C gdi
endif
ifeq ($(VIDEO_SVGALib), Y)
	$(MAKE) -C svga
endif
ifeq ($(VIDEO_VESA), Y)
	$(MAKE) -C vesa
endif
ifeq ($(VIDEO_PSPDEV), Y)
	$(MAKE) -C pspdev
endif
ifeq ($(VIDEO_GP2X), Y)
	$(MAKE) -C gp2x
endif

clean:
	$(MAKE) clean -C helper
	$(MAKE) clean -C null
	$(MAKE) clean -C fb
	$(MAKE) clean -C sdl
	$(MAKE) clean -C svga
	$(MAKE) clean -C vesa
	$(MAKE) clean -C pspdev
	$(MAKE) clean -C gp2x
	$(MAKE) clean -C gdi
