# See LICENSE file for copyright and license details
# slstatus - suckless status monitor
.POSIX:

include config.mk

REQ = util
COM =\
	components/battery\
	components/cpu\
	components/datetime\
	components/disk\
	components/ip\
	components/keymap\
	components/mpd\
	components/ram\
	components/run_command\
	components/swap\
	components/temperature\
	components/uptime\
	components/wifi\

all: slstatus

$(COM:=.o): config.mk $(REQ:=.h)
slstatus.o: slstatus.c slstatus.h arg.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

slstatus: slstatus.o $(COM:=.o) $(REQ:=.o)
	$(CC) -o $@ $(LDFLAGS) $(COM:=.o) $(REQ:=.o) slstatus.o $(LDLIBS)

clean:
	rm -f slstatus slstatus.o $(COM:=.o) $(REQ:=.o)

dist:
	mkdir -p "slstatus-$(VERSION)/components"
	cp -R \
		Makefile \
		slstatus.c \
		$(COM:=.c) \
		$(REQ:=.c) \
		config.mk \
	  arg.h \
		$(REQ:=.h) \
		../LICENSE \
		../README.md \
	  ../slstatus.1 \
		"slstatus-$(VERSION)"
	tar -cf - "slstatus-$(VERSION)" | gzip -c > "slstatus-$(VERSION).tar.gz"
	rm -rf "slstatus-$(VERSION)"

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f slstatus "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/slstatus"
	mkdir -p "$(DESTDIR)$(MANPREFIX)/man1"
	cp -f ../slstatus.1 "$(DESTDIR)$(MANPREFIX)/man1"
	chmod 644 "$(DESTDIR)$(MANPREFIX)/man1/slstatus.1"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/slstatus"
	rm -f "$(DESTDIR)$(MANPREFIX)/man1/slstatus.1"
