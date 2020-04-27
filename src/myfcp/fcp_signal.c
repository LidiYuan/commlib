#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "fcp_signal.h"
#include "genlog.h"
#include <string.h>

struct signal_save{
    int inited;
    struct sigaction oldsig;
};

static struct signal_save g_oldsig[NSIG]={0};

int sigutil_sig_add(int sig,pf_sig_handler handler)
{
    struct sigaction sa={0};

    if(sig < 1 || sig >= NSIG)  
        return -1;

    memset(&sa, 0, sizeof(sig));
    sa.sa_handler = handler; 
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);

    if (sigaction(sig, &sa, &g_oldsig[sig].oldsig) < 0)
        return -1;

    g_oldsig[sig].inited = 1;        

    return 0;
}

int sigutil_sig_del(int sig)
{
    if(sig < 1 || sig >= NSIG) 
        return -1;
    if(0 == g_oldsig[sig].inited)
        return 0;
    if(sigaction(sig, &g_oldsig[sig].oldsig,NULL) < 0)
        return -1;

    g_oldsig[sig].inited = 0;

    return 0;
}

int sigutil_sig_delall(void)
{
    int i = 0;
    for(i = 1; i<NSIG; i++)
    {
        if(sigutil_sig_del(i) < 0)
        {
            print_debug("del signal %d is error\n",i);        
        }    
    }

    return 0;
}




