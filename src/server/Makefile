
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR += ../lib
LIBDIR += ../lib
TARGET = xynth

VLDFLAGS :=
OBJS = event.o id.o kbd.o mouse.o priority.o single.o server.o socket.o start.o \
       surface.o theme.o window.o window_handler.o window_move_resize.o

ifeq ($(VIDEO_FBDev), Y)
OBJS     += video/fb/fb.o video/fb/kbd.o video/fb/server.o video/fb/timing.o
VLDFLAGS += -lm
endif
ifeq ($(VIDEO_SDL), Y)
OBJS     += video/sdl/event.o video/sdl/kbd.o video/sdl/mouse.o video/sdl/server.o
CFLAGS   += $(shell sdl-config --cflags)
VLDFLAGS += $(shell sdl-config --libs)
endif
ifeq ($(VIDEO_SVGALib), Y)
OBJS     += video/svga/kbd.o video/svga/server.o
VLDFLAGS += -lvga -lm
endif
ifeq ($(VIDEO_VESA), Y)
OBJS     += video/vesa/kbd.o video/vesa/server.o video/vesa/lrmi-0.10/lrmi.o
endif
ifeq ($(VIDEO_PSPDEV), Y)
TARGET    = xynth.elf
OBJS     += video/pspdev/event.o video/pspdev/kbd.o video/pspdev/mouse.o video/pspdev/server.o
VLDFLAGS += -lm -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpsputility -lpspuser -lpspkernel
endif
ifeq ($(VIDEO_GDI), Y)
OBJS     += video/gdi/event.o video/gdi/kbd.o video/gdi/mouse.o video/gdi/server.o
LIBDIR   += video/gdi/pthread_w32
VLDFLAGS += -lpthreadGC2 -lgdi32
TARGET    = xynth.exe
endif
ifeq ($(VIDEO_GP2X), Y)
OBJS     += video/gp2x/event.o video/gp2x/kbd.o video/gp2x/mouse.o video/gp2x/server.o
VLDFLAGS += -lpthread -lm
endif
ifeq ($(VIDEO_NULL), Y)
OBJS     += video/null/kbd.o video/null/mouse.o video/null/server.o
endif
ifeq ($(VIDEO_HELPER), Y)
OBJS     += video/helper/console.o video/helper/cursor.o video/helper/kbd.o video/helper/modes.o video/helper/mouse.o video/helper/mtrr.o
endif

ifeq ($(SINGLE_APP), Y)
	ifeq ($(DEMO_CHILD), Y)
		LDFLAGS += ../../demo/child/single_child.a
	endif
	ifeq ($(DEMO_DESKTOP), Y)
		LDFLAGS += ../../demo/desktop/single_desktop.a
	endif
	ifeq ($(DEMO_HIDE), Y)
		LDFLAGS += ../../demo/hide/single_hide.a
	endif
	ifeq ($(DEMO_LOAD), Y)
		LDFLAGS += ../../demo/load/single_load.a
	endif
	ifeq ($(DEMO_SIMPLE), Y)
		LDFLAGS += ../../demo/simple/single_simple.a
	endif
	ifeq ($(DEMO_OBJECT), Y)
		LDFLAGS += ../../demo/object/single_object.a
	endif
	ifeq ($(DEMO_ONSCREENKEYBOARD), Y)
		LDFLAGS += ../../demo/onscreenkeyboard/single_onscreenkeyboard.a
	endif
	ifeq ($(DEMO_TEMP), Y)
		LDFLAGS += ../../demo/temp/single_temp.a
	endif
	ifeq ($(DEMO_TERM), Y)
		LDFLAGS += ../../demo/term/single_term.a
	endif
	ifeq ($(DEMO_TIMER), Y)
		LDFLAGS += ../../demo/timer/single_timer.a
	endif
	ifeq ($(DEMO_LOGOUT), Y)
		LDFLAGS += ../../demo/xynthlogout/single_xynthlogout.a
	endif
endif

ifeq ($(THEME_PLUGIN), Y)
LDFLAGS += -ldl
endif
LDFLAGS += -lxynth $(VLDFLAGS)

EXTRA_CLEAN = xynth xynth.elf xynth.exe video/*/*.o

DEPEND_FILES       = *.c video/*/*.c
EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
	cp -a $(TARGET) ../../$(DISTBINDIR)
ifeq ($(PLATFORM_MINGW), Y)
ifeq ($(THREAD_PTHREADS), Y)
	cp -a video/gdi/pthread_w32/pthreadGC2.dll ../../$(DISTBINDIR)
endif
else
ifeq ($(PLATFORM_PSPDEV), Y)
	psp-fixup-imports ../../$(DISTBINDIR)/$(TARGET)
	mksfo 'Xynth Windowing System' ../../$(DISTBINDIR)/param.sfo
ifeq ($(PSP-VERSION), 1.5)
	mkdir -p ../../$(DISTBINDIR)/xynth
	psp-strip ../../$(DISTBINDIR)/$(TARGET) -o ../../$(DISTBINDIR)/xynth/eboot.pbp
	mkdir -p ../../$(DISTBINDIR)/xynth%
	pack-pbp ../../$(DISTBINDIR)/xynth%/eboot.pbp \
	         ../../$(DISTBINDIR)/param.sfo \
	         ../../tools/pspdev/icon0.png \
	         NULL \
	         NULL \
	         ../../tools/pspdev/pic1.png \
	         NULL \
	         NULL \
	         NULL
else
ifeq ($(PSP-VERSION), 1.0)
	mkdir -p $(BINDIR)/xynth
	psp-strip ../../$(DISTBINDIR)/$(TARGET) -o ../../$(DISTBINDIR)/$(TARGET).strip
	pack-pbp ../../$(DISTBINDIR)/xynth/eboot.pbp \
	         ../../$(DISTBINDIR)/param.sfo \
	         ../../tools/pspdev/icon0.png \
	         NULL \
	         NULL \
	         ../../tools/pspdev/pic1.png \
	         NULL \
	         ../../$(DISTBINDIR)/$(TARGET).strip \
	         NULL
	rm ../../$(DISTBINDIR)/$(TARGET).strip
endif
endif
else
ifeq ($(PLATFORM_GP2X), Y)
	echo "#!/bin/bash"                          >  ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "sync"                                 >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "mount /mnt/sd -o remount,sync"        >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "cd bin"                               >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "export PATH=\`pwd\`:\$$PATH"          >> ../../$(DISTTOPDIR)/$(TARGET).gpe
ifeq ($(SINGLE_APP), Y)
	echo "./xynth &> ../xynth.log"              >> ../../$(DISTTOPDIR)/$(TARGET).gpe
else
	echo "./xynth &> ../xynth.log& sleep 2; ./desktop &> ../xynth_desktop.log" >> ../../$(DISTTOPDIR)/$(TARGET).gpe
endif
	echo "sync"                                 >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "mount /mnt/sd -o remount,async"       >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "cd /usr/gp2x"                         >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	echo "exec ./gp2xmenu"                      >> ../../$(DISTTOPDIR)/$(TARGET).gpe
	chmod +x ../../$(DISTTOPDIR)/$(TARGET).gpe
	cp ../../tools/gp2x/xynth.png ../../$(DISTTOPDIR)
endif
endif
endif
