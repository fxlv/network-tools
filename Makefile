CFLAGS=-Wall -O2
#CFLAGS=-Wall -O2

INSTALLDIR=/opt/network-tools


BINDIR=bin
SRCDIR=src

#all: $(BINDIR)/sockstat $(BINDIR)/files

debug: CFLAGS += -g
debug: $(BINDIR)/udpraw $(BINDIR)/udpsender

# arc4random on linux is in lib-bsd
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    CFLAGS += -lbsd
    CFLAGS += -DLinux # define macro to be used for conditional includes
	CFLAGS += -D_BSD_SOURCE
endif

$(BINDIR)/udpraw: $(SRCDIR)/udpraw.c $(SRCDIR)/common.h $(SRCDIR)/net-checksum.h
	gcc -o $(BINDIR)/udpraw $(SRCDIR)/udpraw.c $(SRCDIR)/common.c $(SRCDIR)/net-checksum.c $(CFLAGS)

$(BINDIR)/udpsender: $(SRCDIR)/udpsender.c $(SRCDIR)/common.h
	gcc -o $(BINDIR)/udpsender $(SRCDIR)/udpsender.c $(SRCDIR)/common.c $(CFLAGS)

clean:
	rm -rf $(BINDIR)/udpraw
	rm -rf $(BINDIR)/udpsender

install:
	mkdir -p $(INSTALLDIR)/
	cp  bin/udpraw $(INSTALLDIR)/
	cp  bin/udpsender $(INSTALLDIR)/
