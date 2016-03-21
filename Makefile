CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := main
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -c -Wall
INC := -I include

.PHONY: run clean clean-all all

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $<

$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$(TARGET) -lboost_iostreams -lboost_system -lboost_filesystem

all: $(TARGET)

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(BINDIR)

clean-all:
	rm -rf $(BUILDDIR)
	rm -rf $(BINDIR)
	rm -f data

run: $(TARGET)
	./$(BINDIR)/$(TARGET)
