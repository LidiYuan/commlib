#ifndef _GEN_LOG_H_
#define _GEN_LOG_H_

#include <syslog.h>

typedef void(*pf_genlog_cb)(const char *msg);
extern void genlog_print(int loglevel, const char *fmt, ...);
#define print_err(fmt,args...)   genlog_print(LOG_ERR,"%s:L[%d] "fmt,__FUNCTION__,__LINE__,##args)
#define print_debug(fmt,args...) genlog_print(LOG_DEBUG,"%s:L[%d] "fmt,__FUNCTION__,__LINE__,##args)
extern void genlog_set_cb(pf_genlog_cb errcb, pf_genlog_cb debugcb);

#endif
