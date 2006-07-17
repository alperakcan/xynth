
include ../Makefile.cfg
include ../Makefile.parse

all :
	$(MAKE) -C lib
	$(MAKE) -C widget
	$(MAKE) -C ../demo
ifeq ($(THEME_PLUGIN), Y)
	$(MAKE) -C themes
endif
	$(MAKE) -C fonts
	$(MAKE) -C configs
#	$(MAKE) -C python
	$(MAKE) -C server

dist:
	$(MAKE) dist -C lib
	$(MAKE) dist -C widget
	$(MAKE) dist -C ../demo
ifeq ($(THEME_PLUGIN), Y)
	$(MAKE) dist -C themes
endif
	$(MAKE) dist -C fonts
	$(MAKE) dist -C configs
#	$(MAKE) dist -C python
	$(MAKE) dist -C server

clean :
	$(MAKE) clean -C lib
	$(MAKE) clean -C widget
	$(MAKE) clean -C ../demo
	$(MAKE) clean -C themes
	$(MAKE) clean -C fonts
	$(MAKE) clean -C configs
	$(MAKE) clean -C python
	$(MAKE) clean -C server
