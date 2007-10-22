
-include ../../../../.config

target_o-$(CONFIG_VIDEO_HELPER) = video_helper.o

video_helper.o_files-y = console.c \
                         helper.h \
                         irman.c \
                         irimaf.c \
                         irair.c \
                         kbd.c \
                         kbd.h \
                         modes.c \
                         mouse.c \
                         mouse.h \
                         mtrr.c \
                         tscreen.c

video_helper.o_includes-y = ../../ \
                            ../../../lib

video_helper.o_includes-$(CONFIG_VIDEO_HELPER_IRIMAF) += $(PRJROOT)/sysapps/imedia/modules/ividio/include \
                                                         $(PRJROOT)/sysapps/imedia/modules/imanage/include \
                                                         $(PRJROOT)/sysapps/imedia/imedia/imaf/include

include ../../../../Makefile.lib
