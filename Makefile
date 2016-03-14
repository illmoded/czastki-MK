CC := g++ -std=c++11
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := main
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -c -Wall
INC := -I include

.PHONY: run clean all

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $<

$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$(TARGET)

all: $(TARGET)

clean:
	-rm -rf $(BUILDDIR)
	-rm -rf $(BINDIR)

run: $(TARGET)
	./$(BINDIR)/$(TARGET)
