
include Makefile.cfg
include Makefile.parse

.PHONY := all clean dist install optimize doxy-doc xynth.pc test strip update pspdev

all:
	@$(MAKE) --no-print-directory -C src

clean:
	@$(MAKE) --no-print-directory clean -C src
	@$(MAKE) --no-print-directory clean -C tools/theme
	@$(MAKE) --no-print-directory clean -C tools/optimize
	rm -f xynth.pc
	rm -rf dist
	rm -rf doc

dist: all xynth.pc
	rm -rf dist
	mkdir -p $(DISTTOPDIR)
	mkdir -p $(DISTINCDIR)
	mkdir -p $(DISTLIBDIR)
	mkdir -p $(DISTBINDIR)
	mkdir -p $(DISTCONFDIR)
	mkdir -p $(DISTFONTDIR)
	mkdir -p $(DISTTHEMEDIR)
	@$(MAKE) --no-print-directory dist -C src
	mkdir -p $(DISTPKGCONFIG)
	cp xynth.pc $(DISTPKGCONFIG)/xynth.pc

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
ifeq ($(PSP_FW_VERSION), 150)
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
	$(MAKE) -C tools/optimize

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
			  */expat/* \
			  */pixman/* \
			  */lrmi-*/* \
			  */pthread_w32/*' \
	SRCDIR='.' \
	PROJECT='xynth' \
	DOCDIR='doc' \
	VERSION='0.8.40' \
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

xynth.pc: Makefile.parse Makefile.cfg
	@rm -rf xynth.pc
	@echo > subs.sed
	@echo "s,@prefix@,${INSTALLDIR},g"     >> subs.sed
	@echo "s,@version@,${XYNTH_VERSION},g" >> subs.sed
ifeq ($(WIDGET_LIB), Y)
	@echo "s,@widget_libs@,-lwidget,g"     >> subs.sed
endif
	@sed -f subs.sed $@.in > $@
	@rm -rf subs.sed

test: clean all dist install

update: clean
	svn update

strip: dist
	@$(foreach F, $(wildcard $(DISTBINDIR)/*), $(STRIP) $(F) > /dev/null 2>&1;)
	@$(foreach F, $(wildcard $(DISTLIBDIR)/*), $(STRIP) $(F) > /dev/null 2>&1;)
	@$(foreach F, $(wildcard $(DISTTHEMEDIR)/*), $(STRIP) $(F) > /dev/null 2>&1;)

pspdev:
	mount /mnt/sdc1
	cp -Rf dist/psp/game/xynth* /mnt/sdc1/psp/game/
	umount /mnt/sdc1
