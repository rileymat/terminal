CC=g++
CFLAGS=-Wl,--no-as-needed -std=c++11 
LIBS=-lpthread
SRC=$(PWD)/src
BUILDDIR=$(PWD)/build
OUTDIR=$(BUILDDIR)/bin
TEMPDIR=$(BUILDDIR)/tmp
MKDIR_P = mkdir -p

.PHONY: directories clean run

all: directories $(OUTDIR)/terminal

$(OUTDIR)/terminal: $(TEMPDIR)/main.o $(TEMPDIR)/terminal.o
	$(CC) $(CFLAGS) $(TEMPDIR)/terminal.o $(TEMPDIR)/main.o $(LIBS) -o $(OUTDIR)/terminal

$(TEMPDIR)/main.o: $(SRC)/main.cpp $(SRC)/terminal.hpp
	$(CC) -c $(CFLAGS) $(SRC)/main.cpp -o $(TEMPDIR)/main.o

$(TEMPDIR)/terminal.o: $(SRC)/terminal.cpp $(SRC)/terminal.hpp
	$(CC) -c $(CFLAGS) $(SRC)/terminal.cpp -o $(TEMPDIR)/terminal.o

directories: ${OUTDIR} $(TEMPDIR)

$(OUTDIR): 
	${MKDIR_P} $(OUTDIR)

$(TEMPDIR):
	${MKDIR_P} $(TEMPDIR)

clean:
	rm -rf $(BUILDDIR)

run:
	$(OUTDIR)/terminal
