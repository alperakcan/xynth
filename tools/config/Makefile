
subdir-y = lxdialog

target.host-y := conf
target.host-y += mconf

conf_files-y = conf.c zconf_tab.c

mconf_files-y = mconf.c zconf_tab.c lxdialog/lxdialog.o
mconf_ldflags-y = -lncurses

include ../../Makefile.lib
