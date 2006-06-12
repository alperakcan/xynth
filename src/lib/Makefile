include ../../Makefile.cfg

CFLAGS += -fPIC -I./libz -I./libpng -I./freetype2/include

obj = alloc.o child.o client.o config.o debug.o driver.o event.o eventq.o font.o \
      gettime.o grlib.o handler.o image.o image_gif.o image_png.o image_xpm.o \
      list.o object.o pipe_api.o pollfd.o rect.o socket.o socket_api.o surface.o \
      thread.o timer.o window.o

ifeq ($(PLATFORM_LINUX), Y)
lib = freetype2/libfreetype.a libpng/libpng.a libz/libz.a -lpthread -lm
all: 3rdparty $(BINDIR) $(LIBDIR) $(INCDIR) $(INCDIR)/xynth.h $(LIBDIR)/$(TARGET_L).a\
     $(LIBDIR)/$(TARGET_L).so
else
ifeq ($(PLATFORM_PSPDEV), Y)
all: 3rdparty $(BINDIR) $(LIBDIR) $(INCDIR) $(INCDIR)/xynth.h $(LIBDIR)/$(TARGET_L).a
else
ifeq ($(PLATFORM_MINGW), Y)
all: 3rdparty $(BINDIR) $(LIBDIR) $(INCDIR) $(INCDIR)/xynth.h $(LIBDIR)/$(TARGET_L).a
else
ifeq ($(PLATFORM_GP2X), Y)
all: 3rdparty $(BINDIR) $(LIBDIR) $(INCDIR) $(INCDIR)/xynth.h $(LIBDIR)/$(TARGET_L).a
endif
endif
endif
endif

$(obj): xynth.h xynth_.h ../../Makefile.cfg

3rdparty:
	$(MAKE) -C libz
	$(MAKE) -C libpng
	$(MAKE) -C freetype2

driver.o: mem.h
pipe_api.o: pipe_posix.c pipe_mem.c
socket_api.o: socket_bsd.c socket_pipe.c
thread.o: thread_mutex_emul.c thread_pspdev.c thread_pthreads.c thread_win32.c

$(BINDIR):
	mkdir -p $(BINDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(INCDIR):
	mkdir -p $(INCDIR)

$(INCDIR)/xynth.h:
	cp -f xynth.h $(INCDIR)/xynth.h

$(LIBDIR)/$(TARGET_L).a: $(obj)
	$(AR) $(LIBDIR)/$(TARGET_L).a $(obj)
	$(RANLIB) $(LIBDIR)/$(TARGET_L).a

$(LIBDIR)/$(TARGET_L).so: $(obj)
	$(CC) -g -shared -Wl,-soname,$(TARGET_L).so.$(VERSION_M) -o $(LIBDIR)/$(TARGET_L).so.$(VERSION) $(obj) $(lib)
	ln -sf $(TARGET_L).so.$(VERSION) $(LIBDIR)/$(TARGET_L).so.$(VERSION_M)
	ln -sf $(TARGET_L).so.$(VERSION) $(LIBDIR)/$(TARGET_L).so

clean:
	$(MAKE) clean -C libz
	$(MAKE) clean -C libpng
	$(MAKE) clean -C freetype2
	rm -f $(LIBDIR)/$(TARGET_L).* *.o *.tco $(obj) $(INCDIR)/xynth.h
