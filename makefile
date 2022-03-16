BINDIR := bin
BUILDDIR := build
SOURCEDIR := src
INSTALLDIR := /usr/bin
ASSETSDIR := $(SOURCEDIR)/assets
ICONSDIR := /usr/share/pixmaps

TARGETNAME := gtkeditor
TARGET := $(BINDIR)/$(TARGETNAME)

ICON := $(ASSETSDIR)/$(TARGETNAME).png

DESKTOPFILE := /usr/share/applications/$(TARGETNAME).desktop

SRC := $(foreach x, $(SOURCEDIR), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

CC := gcc
LDFLAGS :=`pkg-config --cflags --libs gtk4`
CFLAGS := -Wall

default: makedir all

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

.PHONY: makedir
makedir:
	mkdir -p $(BUILDDIR) $(BINDIR)

.PHONY: all
all: $(TARGET)

.PHONY: cleanbuild
cleanbuild:
	rm -r $(BUILDDIR)

.PHONY: clean
clean:
	rm -r $(BUILDDIR) $(BINDIR)

.PHONY: install
install:
	install -m 644 $(ICON) $(ICONSDIR)
	echo "[Desktop Entry]" > $(DESKTOPFILE)
	echo "Version=1.0" >> $(DESKTOPFILE)
	echo "Name=GTKEditor" >> $(DESKTOPFILE)
	echo "Comment=GTKEditor is a simple text editor based on Window's Notepad.exe." >> $(DESKTOPFILE)
	echo "Exec=gtkeditor %U" >> $(DESKTOPFILE)
	echo "Path=$(INSTALLDIR)/" >> $(DESKTOPFILE)
	echo "Icon=$(ICONSDIR)/$(TARGETNAME).png" >> $(DESKTOPFILE)
	echo "Terminal=false" >> $(DESKTOPFILE)
	echo "Type=Application" >> $(DESKTOPFILE)
	echo "MimeType=text/plain" >> $(DESKTOPFILE) 
	echo "Categories=Utility;TextTools;TextEditor;GTK;" >> $(DESKTOPFILE)
	sudo update-desktop-database
	install -m 755 $(TARGET) $(INSTALLDIR)
