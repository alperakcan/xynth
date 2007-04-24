
include ../Makefile.cfg
include ../Makefile.parse

subdirs-Y += fonts
subdirs-Y += configs
subdirs-Y += lib
subdirs-$(WIDGET_LIB) += widget
subdirs-Y += ../demo
subdirs-$(THEME_PLUGIN)   += themes
subdirs-Y += server
#subdirs-$(PYTHON_BINDING) += python

all :
	@$(foreach F, $(subdirs-Y), $(MAKE) -C $F || exit 1;)

dist:
	@$(foreach F, $(subdirs-Y), $(MAKE) dist -C $F || exit 1;)

clean :
	@$(foreach F, $(subdirs-Y), $(MAKE) clean -C $F || exit 1;)
	@$(foreach F, $(subdirs-N), $(MAKE) clean -C $F || exit 1;)
	@$(foreach F, $(subdirs-), $(MAKE) clean -C $F || exit 1;)
