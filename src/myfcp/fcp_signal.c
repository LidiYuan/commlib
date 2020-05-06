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
struct signal_promask{
    int inited; 
    sigset_t  promasksig;
};

static struct signal_save g_oldsig[NSIG]={0};
static struct signal_promask  g_promasksig={0};

/*
 *功能: 忽略所有的信号
  参数:无
  返回值:始终返回0
 * */
int sigutil_ignore_all_sig(void)
{
    int i;    
    struct sigaction sa;

    sa.sa_flags = 0 ;
    sigemptyset( &sa.sa_mask ) ;
    sa.sa_handler = SIG_IGN;
    for (i=1; i<NSIG; i++)
        sigaction( i, &sa, NULL );
    
    return 0;
}

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

/*
 *阻塞一个信号
 *signum:信号值 1~NSIG
 * */
int sigutil_block_sig(int signum)
{
    if(signum < 1 || signum >= NSIG)
        return -1;

    if(!g_promasksig.inited)
    {
        sigemptyset(&g_promasksig.promasksig);
        g_promasksig.inited = 1;
    }

    if(sigismember(&g_promasksig.promasksig,signum))
        return 0;

    if( 0 != sigaddset(&g_promasksig.promasksig,signum) )
        return -1;

    if( 0 != sigprocmask(SIG_BLOCK,&g_promasksig.promasksig,NULL) )
        return -1;

    return 0;
}

/*
 *将信号设置为非阻塞
  signum: 信号值 1~NSIG
 * */
int sigutil_unblock_sig(int signum)
{
    int ret = 0;    
    if(signum < 1 || signum >= NSIG)
         return -1;

    if(!g_promasksig.inited)
         return -1;   

    if(!sigismember(&g_promasksig.promasksig,signum))
         return 0;

    if( 0 != sigprocmask(SIG_UNBLOCK,&g_promasksig.promasksig,NULL) )
        return -1;

    if( 0 != sigdelset(&g_promasksig.promasksig,signum) )
    {
        ret = -1;
    }
     
    if( 0 != sigprocmask(SIG_BLOCK,&g_promasksig.promasksig,NULL) )
        return -1;

    return ret;
}


