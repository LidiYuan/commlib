AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libmyfcp.la
libmyfcp_la_SOURCES=commproc.c  commnet.c  commos.c commfile.c  general.c 
libmyfcp_la_CFLAGS=-I$(top_srcdir)/src/
libmyfcp_la_LDFLAGS = -version-info $(LT_VERSION)

pkginclude_HEADERS=commnet.h commos.h commproc.h commfile.h basecomm.h

