#AUTOMAKE_OPTIONS=foreign
#INCLUDES= -I$(top_srcdir)/src  #old method


AM_CPPFLAGS=-I$(top_srcdir)/src  
noinst_PROGRAMS=testproc testnet testos testfile teststring
MYFCP_PATH=$(top_srcdir)/src/myfcp/libmyfcp.la

testproc_SOURCES=testproc.c 
testproc_LDADD=$(MYFCP_PATH)
#testproc_CFLAGS=-I$(top_srcdir)/src

testnet_SOURCES=testnet.c
testnet_LDADD=$(MYFCP_PATH)

testos_SOURCES=testos.c 
testos_LDADD=$(MYFCP_PATH)

testfile_SOURCES=testfile.c 
testfile_LDADD=$(MYFCP_PATH)

teststring_SOURCES=teststring.c 
teststring_LDADD=$(MYFCP_PATH)
