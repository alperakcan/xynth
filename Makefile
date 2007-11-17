
-include .config

help:
	@echo ""
	@echo "make help         - this text"
	@echo "make menuconfig   - open menu configuration"
	@echo "make all          - build everything"
	@echo "make dist         - install everything to dist path"
	@echo "make install      - install everything to install path"
	@echo ""
	@echo "make clean        - clean everything"
	@echo "make distclean    - clean much more"
	@echo ""
	@echo "make linux        - linux default configuration"
	@echo "make linux-single - linux single app configuration"
	@echo "make mingw        - mingw default configuration"
	@echo "make pspdev       - pspdev default configuration"
	@echo "make gp2x         - linux default configuration"
	@echo ""

check_goal := $(filter-out menuconfig, $(MAKECMDGOALS))
check_goal := $(filter-out distclean, $(check_goal))
check_goal := $(filter-out clean, $(check_goal))
check_goal := $(filter-out linux, $(check_goal))
check_goal := $(filter-out linux-single, $(check_goal))
check_goal := $(filter-out mingw, $(check_goal))
check_goal := $(filter-out pspdev, $(check_goal))
check_goal := $(filter-out gp2x, $(check_goal))
check_goal := $(strip $(check_goal))

ifneq ($(check_goal),)
ifneq "$(CONFIG_CONFIGURED)" "y"
    $(error You must first run make menuconfig)
endif
endif

subdir-y := tools/config
subdir-y += src
subdir-n := tools/theme
subdir-n += tools/optimize

include Makefile.lib

.PHONY: linux linux-single mingw pspdev gp2x
linux linux-single mingw pspdev gp2x:
	$(CP) configs/$@.config .config
	$(CP) configs/$@.h src/lib/config.h
	if [ -e "src/configs/$@.conf" ]; then $(CP) src/configs/$@.conf src/configs/xynth.conf; fi

.PHONY: menuconfig
menuconfig: tools/config_all
	tools/config/mconf configs/Config

.PHONY: distclean
distclean: clean
	@$(RM) .config .config.cmd .config.old .kconfig.d .tmpconfig.h .tmpconfig src/lib/config.h dist

.PHONY: distdirs
distdirs: __FORCE
	$(MKDIR) dist
	$(MKDIR) dist/$(CONFIG_PATH_BIN)
	$(MKDIR) dist/$(CONFIG_PATH_LIB)
	$(MKDIR) dist/$(CONFIG_PATH_INCLUDE)
	$(MKDIR) dist/$(CONFIG_PATH_SHARE)
	$(MKDIR) dist/$(CONFIG_PATH_FONTS)
	$(MKDIR) dist/$(CONFIG_PATH_CONFIGS)
	$(MKDIR) dist/$(CONFIG_PATH_THEMES)

dist: distdirs
	@true

install: dist
	$(CP) ./dist/* $(CONFIG_PATH_INSTALL)/
