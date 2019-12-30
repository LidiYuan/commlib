#AUTOMAKE_OPTIONS=foreign
noinst_PROGRAMS=testfile testnet

testfile_SOURCES=testfile.c 
testfile_LDADD=$(top_srcdir)/src/gencompo4c/libgencomop4c.la
testfile_CFLAGS=-I$(top_srcdir)/src

testnet_SOURCES=testnet.c
testnet_LDADD=$(top_srcdir)/src/gencompo4c/libgencomop4c.la
testnet_CFLAGS=-I$(top_srcdir)/src

