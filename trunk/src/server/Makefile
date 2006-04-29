include ../../Makefile.cfg

obj = event.o id.o kbd.o mouse.o priority.o single.o server.o socket.o start.o\
      surface.o theme.o window.o window_handler.o window_move_resize.o

ifeq ($(THEME_PLUGIN), Y)
	LIBS += -ldl
endif

ifeq ($(VIDEO_VESA), Y)
	objl += video/vesa/libdriver_vesa.a
endif
ifeq ($(VIDEO_SVGALib), Y)
	LIBS += -lvga -lm
	objl += video/svga/libdriver_svga.a
endif
ifeq ($(VIDEO_FBDev), Y)
	LIBS += -lm
	objl += video/fb/libdriver_fb.a
endif
ifeq ($(VIDEO_SDL), Y)
	LIBS += $(shell sdl-config --libs)
	objl += video/sdl/libdriver_sdl.a
endif
ifeq ($(VIDEO_GDI), Y)
	LIBS += -lgdi32
	objl += video/gdi/libdriver_gdi.a
endif
ifeq ($(VIDEO_PSPDEV), Y)
	objl += video/pspdev/libdriver_pspdev.a
endif
ifeq ($(VIDEO_GP2X), Y)
	objl += video/gp2x/libdriver_gp2x.a
endif
ifeq ($(VIDEO_NULL), Y)
	objl += video/null/libdriver_null.a
endif
ifeq ($(VIDEO_HELPER), Y)
	objl += video/helper/libdriver_helper.a
endif
ifeq ($(SINGLE_APP), Y)
	ifeq ($(SINGLE_APP_CHILD), Y)
		objl += ../../demo/single_child.a
	endif
	ifeq ($(SINGLE_APP_DESKTOP), Y)
		objl += ../../demo/single_desktop.a
	endif
	ifeq ($(SINGLE_APP_HIDE), Y)
		objl += ../../demo/single_hide.a
	endif
	ifeq ($(SINGLE_APP_LOAD), Y)
		objl += ../../demo/single_load.a
	endif
	ifeq ($(SINGLE_APP_SIMPLE), Y)
		objl += ../../demo/single_simple.a
	endif
	ifeq ($(SINGLE_APP_OBJECT), Y)
		objl += ../../demo/single_object.a
	endif
	ifeq ($(SINGLE_APP_TEMP), Y)
		objl += ../../demo/single_temp.a
	endif
	ifeq ($(SINGLE_APP_TERM), Y)
		objl += ../../demo/single_term.a
	endif
	ifeq ($(SINGLE_APP_TIMER), Y)
		objl += ../../demo/single_timer.a
	endif
	ifeq ($(SINGLE_APP_LOGOUT), Y)
		objl += ../../demo/single_xynthlogout.a
	endif
endif

all: video_lib $(BINDIR) $(obj) $(objl) $(BINDIR)/$(TARGET_S)

video_lib: ../../Makefile.cfg
	$(MAKE) -C video

$(obj): ../lib/xynth_.h ../lib/xynth.h server.h ../../Makefile.cfg

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(TARGET_S): $(obj) $(objl)
ifeq ($(PLATFORM_LINUX), Y)
	$(CC) -rdynamic -o $(BINDIR)/$(TARGET_S) $(obj) $(objl) $(LIBS)
else
ifeq ($(PLATFORM_MINGW), Y)
	$(CC) -o $(BINDIR)/$(TARGET_S) $(obj) $(objl) $(LIBS)
ifeq ($(THREAD_PTHREADS), Y)
	cp ../../src/server/video/gdi/pthread_w32/pthreadGC2.dll $(BINDIR)
endif
else
ifeq ($(PLATFORM_PSPDEV), Y)
	$(CC) -o $(BINDIR)/$(TARGET_S).elf $(obj) $(objl) $(LIBS)
	psp-fixup-imports $(BINDIR)/$(TARGET_S).elf
	mksfo 'Xynth Windowing System' $(BINDIR)/param.sfo
ifeq ($(PSP-VERSION), 1.5)
	mkdir -p $(BINDIR)/xynth
	psp-strip $(BINDIR)/$(TARGET_S).elf -o $(BINDIR)/xynth/eboot.pbp
	mkdir -p $(BINDIR)/xynth%
	pack-pbp $(BINDIR)/xynth%/eboot.pbp \
	         $(BINDIR)/param.sfo \
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
	psp-strip $(BINDIR)/$(TARGET_S).elf -o $(BINDIR)/$(TARGET_S)_strip.elf
	pack-pbp $(BINDIR)/xynth/eboot.pbp \
                 $(BINDIR)/param.sfo \
		 ../../tools/pspdev/icon0.png \
		 NULL \
		 NULL \
		 ../../tools/pspdev/pic1.png \
		 NULL \
		 $(BINDIR)/$(TARGET_S)_strip.elf \
		 NULL
	rm $(BINDIR)/$(TARGET_S)_strip.elf
endif
endif
else
ifeq ($(PLATFORM_GP2X), Y)
	$(CC) -o $(BINDIR)/$(TARGET_S) $(obj) $(objl) $(LIBS)
	echo "#!/bin/bash"                          >  $(TOPDIR)/$(TARGET_S).gpe
	echo "sync"                                 >> $(TOPDIR)/$(TARGET_S).gpe
	echo "mount /mnt/sd -o remount,sync"        >> $(TOPDIR)/$(TARGET_S).gpe
	echo "cd bin"                               >> $(TOPDIR)/$(TARGET_S).gpe
	echo "export PATH=/mnt/sd/dist/bin:\$$PATH" >> $(TOPDIR)/$(TARGET_S).gpe
ifeq ($(SINGLE_APP), Y)
	echo "./xynth &> ../xynth.log"              >> $(TOPDIR)/$(TARGET_S).gpe
else
	echo "./xynth &> ../xynth.log& sleep 2; ./desktop &> ../xynth_desktop.log" >> $(TOPDIR)/$(TARGET_S).gpe
endif
	echo "sync"                                 >> $(TOPDIR)/$(TARGET_S).gpe
	echo "mount /mnt/sd -o remount,async"       >> $(TOPDIR)/$(TARGET_S).gpe
	echo "cd /usr/gp2x"                         >> $(TOPDIR)/$(TARGET_S).gpe
	echo "exec ./gp2xmenu"                      >> $(TOPDIR)/$(TARGET_S).gpe
	chmod +x $(TOPDIR)/$(TARGET_S).gpe
	cp ../../tools/gp2x/xynth.png $(TOPDIR)
endif
endif
endif
endif

clean:
	$(MAKE) clean -C video
	rm -rf $(BINDIR)/$(TARGET_S) *.o *.tco $(obj)
