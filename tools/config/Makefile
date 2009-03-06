
subdir-y = lxdialog

target.host-y := conf
target.host-y += mconf

conf_files-y = conf.c zconf.tab.c
conf_ldflags-y = -lintl

mconf_files-y = mconf.c zconf.tab.c lxdialog/lxdialog.o
mconf_ldflags-y = -lncurses -lintl

include ../../Makefile.lib
