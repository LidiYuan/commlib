AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libmyfcp.la

libmyfcp_la_SOURCES=commproc.c  commnet.c  commos.c commfile.c  basecomm.c  general.c  genlog.c
libmyfcp_la_CPPFLAGS=-I$(top_srcdir)/src/

if LOG_STDERR
libmyfcp_la_CPPFLAGS += -DLOG_STDERR
endif

libmyfcp_la_LDFLAGS = -version-info $(LT_VERSION)

pkginclude_HEADERS=commnet.h commos.h commproc.h commfile.h basecomm.h

