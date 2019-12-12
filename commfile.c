#define _XOPEN_SOURCE 500
#define _GNU_SOURCE
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "list.h"
#include "commfun.h"

typedef struct{
    pidcallback funaddr;
    pthread_t pthid;
    void *arg;
    struct list_head list;
}PidCallBackList;

static struct list_head g_pidlistcallback={&g_pidlistcallback,&g_pidlistcallback};

static int comm_list_proc_pid(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    struct list_head *pos,*tmp;
    PidCallBackList *pNode = NULL;
    int find = 0;
    int ret = 0;
    int pid;

    if( NULL == fpath && ftwbuf==NULL &&  NULL == sb)    	
    {  	 
        if( nftw("/proc",comm_list_proc_pid,10,FTW_PHYS | FTW_ACTIONRETVAL ) < 0 )
        {
            return -1;
        }
    }
    else
    {
	if ( !strcmp(fpath, "/proc") )
	    return 0;

        if ( !(typeflag&FTW_D) )
            return 0;
    
        pid = atoi(fpath + ftwbuf->base);
        if( 0 == pid )
            return FTW_SKIP_SUBTREE;

        if( ftwbuf->level > 2 )
	    return FTW_SKIP_SUBTREE;	
         
        list_for_each_safe(pos,tmp,&g_pidlistcallback)
        {
	   pNode = container_of(pos,PidCallBackList,list);
	   if(pNode->pthid == pthread_self() )
	   {
	       find = 1; 	   
	       break;
	   }
	}
        if( 1 != find )
	    return FTW_STOP;	

	ret = pNode->funaddr(fpath,pNode->arg);	
	if( ret < 0 )
	    return FTW_STOP;

        return FTW_SKIP_SUBTREE;
    }

    return 0;
}


int comm_list_all_procpid(pidcallback callback,void *usrarg)
{  
    int ret = 0;
    PidCallBackList *pNode;

    if(NULL == callback)
        return -1;

    pNode = (PidCallBackList*)malloc(sizeof(PidCallBackList));
    if(NULL == pNode)
	    return -1;

    memset(pNode,0,sizeof(PidCallBackList));
    
    pNode->pthid = pthread_self();
    pNode->funaddr = callback; 
    pNode->arg = usrarg;
    list_add(&pNode->list,&g_pidlistcallback);
    
    if ( comm_list_proc_pid(NULL,NULL,0,NULL) < 0 )
    {
        ret = -1;
    }

    list_del(&pNode->list);
    free(pNode); 
    
    return ret;
}


