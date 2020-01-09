#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gencompo4c.h"


static int running_user(struct login_info *uinfo,void *userarg)
{
    printf("User: %s, host: %s, tty: %s, logintime:%ld\n",uinfo->username,uinfo->hostname,uinfo->tty,uinfo->logintime);

    return 0;//return -1,stop  loop
}

static int logout_user(struct login_info *uinfo,void *userarg)
{
    printf("User: %s, host: %s, tty: %s, logintime:%ld, logouttime: %ld\n",uinfo->username,uinfo->hostname,uinfo->tty,uinfo->logintime,uinfo->logouttime);
    return 0;
}



int main(int argc,char *argv[])
{
    time_t t;	
    char buff[256]={0};

    if(os_info_uuid(buff,256) == 0)
    {
        printf("uuid:%s\n",buff);
    }
    
    memset(buff,0,256);
    if(os_info_bootid(buff,256) == 0)
    {
        printf("bootid:%s\n",buff);
    }

    printf("*****************************\n");
    t = os_info_boot_time();
    if( -1 != t)
    {
        printf("boot time:%ld\n",t);
    }

    t = os_info_last_shutdow_time();
    if( -1 != t)
    {
        printf("Last shutdown time:%ld\n",t);
    }

    if( os_info_running_tty(running_user,NULL) != 0)
    {
         printf("get running tty  failed\n");
    }
    printf("\n");

    if( os_info_logout_tty(logout_user,NULL) !=0)
    {
        printf("get logout tty failed\n");
    }



    return 0;
}
