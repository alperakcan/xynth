include Makefile.cfg

all: Makefile.cfg
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src
	rm -rf dist
	rm -rf doc
	find . \( -name \*.o -o -name \*~ -o -name \#\*\# -o -name \*.a -name \*.so \) -exec rm '{}' \;

install: all
ifeq ($(PLATFORM_LINUX), Y)
	mkdir -p $(INSTALLDIR)
	cp -Rf dist/* $(INSTALLDIR)
	ldconfig
else
ifeq ($(PLATFORM_MINGW), Y)
	mkdir -p c:/xynth
	cp -Rf dist/* c:/xynth
else
ifeq ($(PLATFORM_PSPDEV), Y)
	rm -rf dist/include
	rm -rf dist/lib
	rm -rf dist/$(_SHAREDIR)/$(_CONFDIR)
	mkdir -p dist/psp/game
	cp -Rf dist/bin/xynth dist/psp/game
ifeq ($(PSP-VERSION), 1.5)
	cp -Rf dist/bin/xynth% dist/psp/game
endif
	cp -Rf dist/share dist/psp/game/xynth
	rm -rf dist/share
	rm -rf dist/bin
else
ifeq ($(PLATFORM_GP2X), Y)
	rm -rf dist/include
	rm -rf dist/lib
	rm -rf dist/share/configs
	gp2x-strip -xsR .note -R .comment dist/bin/xynth
endif
endif
endif
endif

doxy-doc:
	rm -rf doc
	EXCLUDE_PATTERNS='*/dist/*\
	                  */demo/*\
	                  */tools/*\
	                  */ports/*\
	                  */themes/*\
	                  */libz/*\
	                  */libpng/*\
	                  */freetype2/*\
	                  */lrmi-*/*\
	                  */pthread_w32/*'\
	SRCDIR='.'\
	PROJECT='xynth'\
	DOCDIR='doc'\
	VERSION='0.8.20'\
	PERL_PATH='perl'\
	HAVE_DOT='NO'\
	GENERATE_MAN='NO'\
	GENERATE_RTF='NO'\
	GENERATE_XML='NO'\
	GENERATE_HTMLHELP='NO'\
	GENERATE_CHI='NO'\
	GENERATE_HTML='YES'\
	GENERATE_LATEX='NO'\
	doxygen ./doxygen.cfg

test: clean all install

pspdev:
	mount /mnt/sdb1
	cp -Rf dist/psp/game/xynth* /mnt/sdb1/psp/game/
	umount /mnt/sdb1
