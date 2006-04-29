include ../../Makefile.cfg

CFLAGS += -fPIC
CPPFLAGS = $(CFLAGS)

obj = button.o frame.o handler.o layout.o object.o rect.o window.o

ifeq ($(PLATFORM_LINUX), Y)
all: $(LIBDIR) $(INCDIR) $(INCDIR)/widget.h $(LIBDIR)/$(TARGET_W).a $(LIBDIR)/$(TARGET_W).so
else
ifeq ($(PLATFORM_PSPDEV), Y)
all: $(LIBDIR) $(INCDIR) $(INCDIR)/widget.h $(LIBDIR)/$(TARGET_W).a
else
ifeq ($(PLATFORM_GP2X), Y)
all: $(LIBDIR) $(INCDIR) $(INCDIR)/widget.h $(LIBDIR)/$(TARGET_W).a
else
all:
endif
endif
endif

$(obj): widget.h

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(INCDIR):
	mkdir -p $(INCDIR)

$(INCDIR)/widget.h: widget.h
	cp -f widget.h $(INCDIR)/widget.h

$(LIBDIR)/$(TARGET_W).a: $(obj)
	$(AR) $(LIBDIR)/$(TARGET_W).a $(obj)
	$(RANLIB) $(LIBDIR)/$(TARGET_W).a

$(LIBDIR)/$(TARGET_W).so: $(obj)
	$(CXX) -g -shared -Wl,-soname,$(TARGET_W).so.$(VERSION_M) -o $(LIBDIR)/$(TARGET_W).so.$(VERSION) $(obj) $(LIBS)
	ln -sf $(TARGET_W).so.$(VERSION) $(LIBDIR)/$(TARGET_W).so.$(VERSION_M)
	ln -sf $(TARGET_W).so.$(VERSION) $(LIBDIR)/$(TARGET_W).so

clean:
	rm -f $(LIBDIR)/$(TARGET_W).* $(obj)
