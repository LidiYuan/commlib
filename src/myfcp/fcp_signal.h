#ifndef __FCP_SIGNAL_H_
#define __FCP_SIGNAL_H_


typedef void (*pf_sig_handler)(int);
extern int sigutil_sig_add(int sig,pf_sig_handler handler);
extern int sigutil_sig_del(int sig);
extern int sigutil_sig_delall(void);


#endif
