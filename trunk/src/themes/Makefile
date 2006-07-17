
include ../../Makefile.cfg
include ../../Makefile.parse

all:
	$(MAKE) -C ../../tools/theme
	$(MAKE) -C keramik
	$(MAKE) -C microgui
	$(MAKE) -C silverado

dist: all
	$(MAKE) dist -C keramik
	$(MAKE) dist -C microgui
	$(MAKE) dist -C silverado

clean:
	$(MAKE) clean -C ../../tools/theme
	$(MAKE) clean -C keramik
	$(MAKE) clean -C microgui
	$(MAKE) clean -C silverado
