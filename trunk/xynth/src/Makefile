include ../Makefile.cfg

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

clean :
	$(MAKE) clean -C lib
	$(MAKE) clean -C widget
	$(MAKE) clean -C ../demo
	$(MAKE) clean -C themes
	$(MAKE) clean -C fonts
	$(MAKE) clean -C configs
	$(MAKE) clean -C python
	$(MAKE) clean -C server
