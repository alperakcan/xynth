
-include ../../../.config

#WIDGET_SCRIPT_ENGINE_JS = Y

ifdef WIDGET_SCRIPT_ENGINE_JS
SPIDER_MONKEY = ./mozilla/js/
SPIDER_MONKEY_CFLAGS = -DWIDGET_SCRIPT_ENGINE_JS -Wno-format -DGCC_OPT_BUG -g -DXP_UNIX -DSVR4 -DSYSV -D_BSD_SOURCE -DPOSIX_SOURCE -DHAVE_LOCALTIME_R -DX86_LINUX  -DDEBUG -DDEBUG_root -DEDITLINE
SPIDER_MONKEY_INC = $(SPIDER_MONKEY)/src $(SPIDER_MONKEY)/src/Linux_All_DBG.OBJ
SPIDER_MONKEY_LIB = $(SPIDER_MONKEY)/src/Linux_All_DBG.OBJ/libjs.a  -lm  $(SPIDER_MONKEY)/src/editline/Linux_All_DBG.OBJ/libedit.a
endif

target-$(CONFIG_WIDGET_WIDGETR) = widgetr

widgetr_cflags-y    += $(SPIDER_MONKEY_CFLAGS)
widgetr_includes-y  += . .. ../../lib $(SPIDER_MONKEY_INC)
widgetr_libraries-y += . ../ ../../lib/
widgetr_ldflags-y   += -lwidget -lxynth $(SPIDER_MONKEY_LIB)
widgetr_files-y     += widgetr.c \
                       code.c \
                       js.c

include ../../../Makefile.lib

dist:
	$(CP) widgetr ../../../dist/$(CONFIG_PATH_BIN)/widgetr
