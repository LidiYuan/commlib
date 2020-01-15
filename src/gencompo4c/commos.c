#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "list.h"
#include "general.h"

struct login_list{
    short   type;
    struct login_info info;
    struct list_head list;
};


enum{
   OS_INFO_UUID =0,
   OS_INFO_BOOTID
};

static int init_login_list(struct list_head *phead)
{

    FILE *fp = NULL;
    struct utmp record;
    struct login_list *pNode = NULL;

    fp = fopen(WTMP_FILENAME,"r");
    if(NULL == fp)
      return -1;

    while( fread(&record,sizeof(record),1,fp) > 0 )
    {
        if( (record.ut_type != USER_PROCESS) && (record.ut_type != DEAD_PROCESS))
            continue;

        pNode = (struct login_list *)malloc(sizeof(struct login_list));
        if(NULL == pNode)
            continue;

        memset(pNode,0,sizeof(struct login_list));
        if(record.ut_type == USER_PROCESS)
        {
            snprintf(pNode->info.username,MAX_USER_NAME,"%s",record.ut_user);
            snprintf(pNode->info.hostname,MAX_HOST_NAME,"%s",record.ut_host);
        }
        snprintf(pNode->info.tty,MAX_TTY_LEN,"%s",record.ut_line);
        pNode->info.logintime = record.ut_tv.tv_sec+(record.ut_tv.tv_usec/1000000);
        pNode->info.logouttime = (time_t)-1;
        pNode->type = record.ut_type;
        list_add_tail(&pNode->list,phead);
    }

    fclose(fp);
    return 0;
}

static void exit_login_list(struct list_head *head)
{
    struct list_head *pos,*next;
    struct login_list *pNode = NULL;


    list_for_each_safe(pos,next,head)
    {
        pNode = container_of(pos,struct login_list,list);
        list_del(pos);
        free(pNode);
    }

    return;
}

static int os_info(char *buff,int type,unsigned int buffsize)
{
    FILE *fp = NULL;	
    switch(type)
    {
    case OS_INFO_UUID:
	 fp = fopen("/proc/sys/kernel/random/uuid","r");
         if(NULL == fp)
	     return -1;
         if(NULL == fgets(buff,(int)buffsize,fp))
	     return -1;
         
	 if(buff[strlen(buff) -1] == '\n') 	 
             buff[strlen(buff) -1] = '\0';
      
         break;
    case OS_INFO_BOOTID:
	 fp = fopen("/proc/sys/kernel/random/boot_id","r");
         if(NULL == fp)
	     return -1;

         if(NULL == fgets(buff,(int)buffsize,fp))
	     return -1;
         
	 if(buff[strlen(buff) -1] == '\n') 	 
             buff[strlen(buff) -1] = '\0';
      
         break;

    default:
	    break;
    }

    return 0;
}

int os_info_uuid(char *buff,unsigned int bufsize)
{
    if(NULL == buff)
	    return -1;
       
    return os_info(buff,OS_INFO_UUID,bufsize);
}

int os_info_bootid(char *buff,unsigned int bufsize)
{
    if(NULL == buff)
	    return -1;

    return os_info(buff,OS_INFO_BOOTID,bufsize);  
}

time_t os_info_boot_time(void)
{
    FILE *fp = NULL;
    struct utmp record;

    fp = fopen(UTMP_FILENAME,"r");
    if(NULL == fp)
        return (time_t)-1;

    while( fread(&record,sizeof(record),1,fp) > 0 )
    {
        if(!strcmp(record.ut_user,"reboot") && BOOT_TIME == record.ut_type)
        {
            fclose(fp);
            return (time_t)(record.ut_tv.tv_sec+(record.ut_tv.tv_usec/1000000));
        }
    }

    fclose(fp);
    return (time_t) -1; 
}

time_t os_info_last_shutdow_time(void)
{
    FILE *fp = NULL;
    struct utmp record;
    time_t oldtime = -1;

    fp = fopen(WTMP_FILENAME,"r");
    if(NULL == fp)
        return (time_t)-1;

    while( fread(&record,sizeof(record),1,fp) > 0 )
    {
        if( !strcmp(record.ut_user,"shutdown") && RUN_LVL == record.ut_type )
        {
            oldtime = record.ut_tv.tv_sec+(record.ut_tv.tv_usec/1000000);
        }
    }

    fclose(fp);
    return oldtime;
}

int os_info_running_tty(login_info_cb callback,void *userarg)
{
    FILE *fp = NULL;
    struct utmp record;
    struct login_info ent;

    fp = fopen(UTMP_FILENAME,"r");
    if(NULL == fp)
        return -1;

    while( fread(&record,sizeof(record),1,fp) > 0 )
    {
        if( record.ut_type != USER_PROCESS)
            continue;

        memset(&ent,0,sizeof(ent));
        snprintf(ent.username,MAX_USER_NAME,"%s",record.ut_user);
        snprintf(ent.hostname,MAX_HOST_NAME,"%s",record.ut_host);
        snprintf(ent.tty,MAX_TTY_LEN,"%s",record.ut_line);
        ent.logintime = record.ut_tv.tv_sec+(record.ut_tv.tv_usec/1000000);
        ent.logouttime = (time_t)-1;

        if( callback(&ent,userarg) < 0)
                break;
    }

    fclose(fp);

    return 0;
}


int os_info_logout_tty(login_info_cb callback, void *userarg)
{
    struct list_head *pos,*next;
    struct list_head *tmppos;
    struct login_list *pNode;
    struct login_list *pNodeout;
    struct list_head tmphead={&tmphead,&tmphead};

    if(init_login_list(&tmphead) != 0)
        return -1;
    
    while( (tmppos = list_pop_front(&tmphead)) )
    {
        pNode = container_of(tmppos,struct login_list,list);
        if(pNode->type != USER_PROCESS)
        {
                free(pNode);
                continue;
        }

        list_for_each_safe(pos,next,&tmphead)
        {
            pNodeout = container_of(pos,struct login_list,list);
            if(pNodeout->type != DEAD_PROCESS)
                    continue;

            if( !strcmp(pNodeout->info.tty,pNode->info.tty) && pNode->info.logintime <= pNodeout->info.logintime)
            {
                pNode->info.logouttime = pNodeout->info.logintime;
                if( callback(&pNode->info,userarg) < 0)
                {
                    exit_login_list(&tmphead);
                    return 0;
                }
                list_del(pos);
                free(pNode);
                free(pNodeout);
                break;
            }
        }
    }

    exit_login_list(&tmphead);

    return 0;
}

int os_info_version(void)
{
    return general_os_version();
}




