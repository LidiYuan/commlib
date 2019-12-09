CC=gcc
AR =ar
CFLAGS= -Wall -g -c

NETBIN=testnet
LIBCOMM=libcommfun.a

COMMSRC=commnet.c
NETOBJ=commnet.o testnet.o
COMMOBJ=commnet.o

default: $(LIBCOMM) $(NETBIN)

$(NETBIN):testnet.c
	$(CC) $(CFLAGS)  -c $^
	$(CC) $(NETOBJ) -o  $@

$(LIBCOMM):$(COMMSRC)
	$(CC) $(CFLAGS) $^
	$(AR) -rcs $@  $(COMMOBJ)

clean:
	rm -rf *.o
	rm -rf $(NETBIN)
	rm -rf $(LIBCOMM)
