CC = gcc
CFLAGS = -Wall
LDFLAGS=`pkg-config --cflags --libs gtk4`

BUILDDIR = build
SOURCEDIR = src

SOURCES = $(wildcard $(SOURCEDIR)/*.c)

all:
	$(CC) -o $(BUILDDIR)/text-editor $(SOURCES) $(CFLAGS) $(LDFLAGS)
