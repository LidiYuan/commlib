#AUTOMAKE_OPTIONS=foreign
#INCLUDES= -I$(top_srcdir)/src  #old method

AM_CPPFLAGS=-I$(top_srcdir)/src  
noinst_PROGRAMS=testproc testnet testos testfile

testproc_SOURCES=testproc.c 
testproc_LDADD=$(top_srcdir)/src/myfcp/libmyfcp.la
#testproc_CFLAGS=-I$(top_srcdir)/src

testnet_SOURCES=testnet.c
testnet_LDADD=$(top_srcdir)/src/myfcp/libmyfcp.la
#testnet_CFLAGS=-I$(top_srcdir)/src


testos_SOURCES=testos.c 
testos_LDADD=$(top_srcdir)/src/myfcp/libmyfcp.la
#testos_CFLAGS=-I$(top_srcdir)/src

testfile_SOURCES=testfile.c 
testfile_LDADD=$(top_srcdir)/src/myfcp/libmyfcp.la
#testos_CFLAGS=-I$(top_srcdir)/src
