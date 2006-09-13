
include Makefile.cfg
include Makefile.parse

all:
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C tools/theme
	$(MAKE) clean -C tools/optimize
	rm -rf dist
	rm -rf doc

dist: all
	rm -rf dist
	mkdir -p $(DISTTOPDIR)
	mkdir -p $(DISTINCDIR)
	mkdir -p $(DISTLIBDIR)
	mkdir -p $(DISTBINDIR)
	mkdir -p $(DISTCONFDIR)
	mkdir -p $(DISTFONTDIR)
	mkdir -p $(DISTTHEMEDIR)
	$(MAKE) dist -C src

install: dist
ifeq ($(PLATFORM_LINUX), Y)
	mkdir -p $(INSTALLDIR)
	cp -af dist/* $(INSTALLDIR)
	ldconfig
else
ifeq ($(PLATFORM_MINGW), Y)
	mkdir -p c:/xynth
	cp -af dist/* c:/xynth
else
ifeq ($(PLATFORM_PSPDEV), Y)
	rm -rf $(DISTINCDIR)
	rm -rf $(DISTLIBDIR)
	rm -rf $(DISTCONFDIR)
	rm -rf $(DISTTHEMEDIR)
	mkdir -p dist/psp/game
	cp -af dist/bin/xynth dist/psp/game
ifeq ($(PSP-VERSION), 1.5)
	cp -af dist/bin/xynth% dist/psp/game
endif
	cp -af dist/share dist/psp/game/xynth
	rm -rf dist/share
	rm -rf dist/bin
else
ifeq ($(PLATFORM_GP2X), Y)
	rm -rf $(DISTINCDIR)
	rm -rf $(DISTLIBDIR)
	rm -rf $(DISTCONFDIR)
	rm -rf $(DISTTHEMEDIR)
	gp2x-strip -xsR .note -R .comment dist/bin/xynth
endif
endif
endif
endif

optimize:
	make -C tools/optimize

doxy-doc:
	rm -rf doc
	EXCLUDE_PATTERNS='*/dist/* \
	                  */demo/* \
			  */tools/* \
			  */ports/* \
			  */themes/* \
			  */libz/* \
			  */libpng/* \
			  */freetype2/* \
			  */lrmi-*/* \
			  */pthread_w32/*' \
	SRCDIR='.' \
	PROJECT='xynth' \
	DOCDIR='doc' \
	VERSION='0.8.20' \
	PERL_PATH='perl' \
	HAVE_DOT='NO' \
	GENERATE_MAN='NO' \
	GENERATE_RTF='NO' \
	GENERATE_XML='NO' \
	GENERATE_HTMLHELP='NO' \
	GENERATE_CHI='NO' \
	GENERATE_HTML='YES' \
	GENERATE_LATEX='NO' \
	doxygen ./doxygen.cfg

test: clean all dist install

update: clean
		svn update

pspdev:
	mount /mnt/sda1
	cp -Rf dist/psp/game/xynth* /mnt/sda1/psp/game/
	umount /mnt/sda1
