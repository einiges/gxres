
VERSION ?= 0.0.1

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

LDLIBS  = -lX11
LDFLAGS =

CPPFLAGS = -DVERSION=\"${VERSION}\"

CFLAGS = -std=c99 -Wall -pedantic

DFLAGS = -g

