LIBDIR ?= /usr/local/lib
INCDIR ?= /usr/local/include

CFLAGS = -g -Wall

sources = lrmi.c
objects = lrmi.o
pic_objects = lrmi.lo
all = liblrmi.a liblrmi.so vbetest

MAJOR = 0
MINOR = 10
VERSION = $(MAJOR).$(MINOR)
LIBNAME = liblrmi

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

%.lo: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fPIC -o $@ $<

all: $(all)

liblrmi.a: $(objects)
	$(AR) -rs $@ $^

liblrmi.so: $(pic_objects)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -o $@ $^
	$(CC) $(CPPFLAGS) $(CFLAGS) -Wl,-soname,$(LIBNAME).so.$(MAJOR) -fPIC -shared -o $(LIBNAME).so.$(VERSION) $^
	ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so.$(MAJOR)
	ln -sf $(LIBNAME).so.$(MAJOR) $(LIBNAME).so

vbetest: vbetest.c liblrmi.a
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

install:
	mkdir -p $(LIBDIR)
	install -m 755 -s -p $(LIBNAME).so.$(VERSION) $(LIBDIR)/$(LIBNAME).so.$(VERSION)
	rm -f $(LIBDIR)/$(LIBNAME).so
	ln -sf $(LIBNAME).so.$(VERSION) $(LIBDIR)/$(LIBNAME).so.$(MAJOR)
	ln -sf $(LIBNAME).so.$(MAJOR) $(LIBDIR)/$(LIBNAME).so
	install -m 644 -s -p lrmi.h $(INCDIR)/lrmi.h
	ldconfig

.PHONY: clean
clean:
	rm -f $(objects) $(pic_objects) $(all) core
	rm -f liblrmi.so liblrmi.so.$(MAJOR) liblrmi.so.$(VERSION)

.PHONY: distclean
distclean: clean
	rm -f .depend

.PHONY: depend
depend: $(sources)
	-$(CC) -M $(CPPFLAGS) $^ >.depend
