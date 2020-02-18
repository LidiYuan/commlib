AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libmyfcp.la


libmyfcp_la_SOURCES=fcp_proc.c  fcp_net.c  fcp_os.c fcp_file.c fcp_base.c fcp_string.c genstring.c general.c  genlog.c genmd5.c
libmyfcp_la_CPPFLAGS=-I$(top_srcdir)/src/


if LOG_STDERR
libmyfcp_la_CPPFLAGS += -DLOG_STDERR
endif

libmyfcp_la_LDFLAGS = -version-info $(LT_VERSION)

pkginclude_HEADERS=fcp_net.h fcp_os.h fcp_proc.h fcp_file.h fcp_base.h fcp_string.h 

