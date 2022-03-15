BINDIR := bin
BUILDDIR := build
SOURCEDIR := src

TARGET_NAME := GTKEditor
TARGET := $(BINDIR)/$(TARGET_NAME)

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
