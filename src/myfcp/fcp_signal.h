#ifndef __FCP_SIGNAL_H_
#define __FCP_SIGNAL_H_
#include <signal.h>

typedef void (*pf_sig_handler)(int);

extern int sigutil_sig_add(int sig,pf_sig_handler handler);
extern int sigutil_sig_del(int sig);
extern int sigutil_sig_delall(void);
extern int sigutil_ignore_all_sig(void);
extern int sigutil_unblock_sig(int signum);
extern int sigutil_block_sig(int signum);

#endif
