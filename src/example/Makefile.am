#AUTOMAKE_OPTIONS=foreign
noinst_PROGRAMS=testproc testnet testos

testproc_SOURCES=testproc.c 
testproc_LDADD=$(top_srcdir)/src/gencompo4c/libgencomop4c.la
testproc_CFLAGS=-I$(top_srcdir)/src

testnet_SOURCES=testnet.c
testnet_LDADD=$(top_srcdir)/src/gencompo4c/libgencomop4c.la
testnet_CFLAGS=-I$(top_srcdir)/src


testos_SOURCES=testos.c 
testos_LDADD=$(top_srcdir)/src/gencompo4c/libgencomop4c.la
testos_CFLAGS=-I$(top_srcdir)/src

