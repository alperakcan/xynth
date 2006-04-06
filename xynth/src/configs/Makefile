include ../../Makefile.cfg

all: $(CONFDIR) $(CONFDIR)/xynth.conf

$(CONFDIR):
	mkdir -p $(CONFDIR)

$(CONFDIR)/xynth.conf: xynth.conf
	cp xynth.conf $(CONFDIR)

clean:
	rm -rf $(CONFDIR)
