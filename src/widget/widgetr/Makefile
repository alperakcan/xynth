
include ../../../Makefile.cfg
include ../../../Makefile.parse

DIR = src/widget/widgetr/

#WIDGET_SCRIPT_ENGINE_JS = Y

ifdef WIDGET_SCRIPT_ENGINE_JS
SPIDER_MONKEY = ./mozilla/js/
SPIDER_MONKEY_CFLAGS = -DWIDGET_SCRIPT_ENGINE_JS -Wno-format -DGCC_OPT_BUG -g -DXP_UNIX -DSVR4 -DSYSV -D_BSD_SOURCE -DPOSIX_SOURCE -DHAVE_LOCALTIME_R -DX86_LINUX  -DDEBUG -DDEBUG_root -DEDITLINE
SPIDER_MONKEY_INC = $(SPIDER_MONKEY)/src $(SPIDER_MONKEY)/src/Linux_All_DBG.OBJ
SPIDER_MONKEY_LIB = $(SPIDER_MONKEY)/src/Linux_All_DBG.OBJ/libjs.a  -lm  $(SPIDER_MONKEY)/src/editline/Linux_All_DBG.OBJ/libedit.a
endif

CFLAGS += $(SPIDER_MONKEY_CFLAGS)
INCDIR += . .. ../../lib $(SPIDER_MONKEY_INC)
LIBDIR += ../../lib ../../widget

LIBDIR-$(PLATFORM_MINGW) = ../../server/video/gdi/pthread_w32
LDFLAGS-$(PLATFORM_MINGW) = -lpthreadGC2

LDFLAGS-$(PLATFORM_PSPDEV) = -lm -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpsputility -lpspuser -lpspkernel -lpspnet -lpspnet_inet

LDFLAGS += -lwidget -lxynth $(SPIDER_MONKEY_LIB)

OBJS = widgetr.o code.o js.o

TARGET = widgetr

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
