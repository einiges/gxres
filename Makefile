
include config.mk

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

all: gxres

debug: CFLAGS += $(DFLAGS)
debug: gxres

gxres: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(LDLIBS) $(OBJ)

$(OBJ): %.o : %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	$(RM) gxres $(OBJ) gxres-$(VERSION).tar.gz

dist: clean
	mkdir -p gxres-$(VERSION)
	cp -R Makefile config.mk LICENSE README.md gxres.1 arg.h $(SRC) gxres-$(VERSION)
	tar -cf - gxres-$(VERSION) | gzip > gxres-$(VERSION).tar.gz
	$(RM) -r gxres-$(VERSION)

install: gxres
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f gxres $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/gxres
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < gxres.1 > $(DESTDIR)$(MANPREFIX)/man1/gxres.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/gxres.1

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/gxres
	$(RM) $(DESTDIR)$(MANPREFIX)/man1/gxres.1

.PHONY: all clean

