#ifndef _GEN_LOG_H_
#define _GEN_LOG_H_


#include "fcp_base.h"

extern void genlog_print(int loglevel, const char *fmt, ...);
#define print_err(fmt,args...)   genlog_print(LOG_ERR,"%s:L[%d] "fmt,__FUNCTION__,__LINE__,##args)
#define print_debug(fmt,args...) genlog_print(LOG_DEBUG,"%s:L[%d] "fmt,__FUNCTION__,__LINE__,##args)

extern void genlog_set_cb(genlog_cb errcb, genlog_cb debugcb);

#endif
