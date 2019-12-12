CC=gcc
AR =ar
PWD=$(shell pwd)

OBJDIR=objdir
INCLUDE= ./include
CFLAGS= -Wall -g
 
LIBSRC=$(wildcard *.c)
TESTSRC=$(wildcard test/*.c)
ALLSRCS=$(LIBSRC) $(TESTSRC)

NETBIN=$(OBJDIR)/testnet
FILEBIN=$(OBJDIR)/testfile
LIBCOMM=$(OBJDIR)/libcommfun.a
LIBCOMMSO=$(OBJDIR)/libcommfun.so

FILEOBJ=$(OBJDIR)/commfile.o $(OBJDIR)/testfile.o
NETOBJ=$(OBJDIR)/commnet.o $(OBJDIR)/testnet.o
COMMOBJ=$(OBJDIR)/commnet.o


default: allobj $(LIBCOMM) $(NETBIN) $(FILEBIN) $(LIBCOMMSO)

allobj:$(ALLSRCS)
	@rm -rf $(OBJDIR)
	@mkdir $(OBJDIR)
	$(CC) -c  $^  -I$(INCLUDE) $(CFLAGS)
	@mv *.o $(OBJDIR)
$(LIBCOMMSO):$(LIBSRC)
	$(CC) -fPIC --shared $^ -I$(INCLUDE) -o $@

$(FILEBIN):$(FILEOBJ)
	$(CC) $^ -o $@

$(NETBIN):$(NETOBJ)
	$(CC) $^ -o  $@

$(LIBCOMM):$(COMMOBJ)
	$(AR) -rcs $@  $^

clean:
	@rm -rf $(OBJDIR)
