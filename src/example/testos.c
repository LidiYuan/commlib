#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mylib4c.h>
#include <time.h>

static int running_user(struct login_info *uinfo,void *userarg)
{

    struct tm resu1={0};
    
    (void)localtime_r(&uinfo->logintime,&resu1);

    printf("pid:%d User:%s, host:%s, tty:%s, logintime:%ld(%04d-%02d-%02d %02d:%02d:%02d)\n",uinfo->pid,uinfo->username,uinfo->hostname,uinfo->tty,uinfo->logintime,resu1.tm_year+1900,resu1.tm_mon+1,resu1.tm_mday,resu1.tm_hour,resu1.tm_min,resu1.tm_sec);

    return 0; //return -1,stop  loop
}

static int logout_user(struct login_info *uinfo,void *userarg)
{
    struct tm resu1={0};
    struct tm resu2={0};
    
    (void)localtime_r(&uinfo->logintime,&resu1);
    (void)localtime_r(&uinfo->logouttime,&resu2);

    printf("User:%s, host:%s, tty:%s, logintime:%ld(%04d-%02d-%02d %02d:%02d:%02d), logouttime: %ld(%04d-%02d-%02d %02d:%02d:%02d)\n",uinfo->username,uinfo->hostname,uinfo->tty,uinfo->logintime,resu1.tm_year+1900,resu1.tm_mon+1,resu1.tm_mday,resu1.tm_hour,resu1.tm_min,resu1.tm_sec,uinfo->logouttime,resu2.tm_year+1900,resu2.tm_mon+1,resu2.tm_mday,resu2.tm_hour,resu2.tm_min,resu2.tm_sec );
    return 0;
}



int main(int argc,char *argv[])
{
    time_t t;	
    char buff[256]={0};
    int ret;

    printf("\n***************************output info  start *****************************************\n\n");
    if(os_info_uuid(buff,256) == 0)
    {
        printf("uuid:%s\n",buff);
    }
    else
    {
        printf("get uuid is error\n");
    }
    
    memset(buff,0,256);
    if(os_info_bootid(buff,256) == 0)
    {
        printf("bootid:%s\n",buff);
    }
    else
    {
        printf("get bootid is error\n");
    }

    memset(buff,0,256);
    if( os_info_machine_id(buff,MAX_MACHINE_ID_LEN) ==0 )
    {
        printf("Machine id is:%s\n",buff);
    }
    else
    {
        printf("get machine id is error\n");
    }

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

    printf("\n");
    if( os_info_running_tty(running_user,NULL) != 0)
    {
         printf("get running tty  failed\n");
    }
    printf("\n");

    printf("logout tty:\n");
    if( os_info_logout_tty(logout_user,NULL) !=0)
    {
        printf("get logout tty failed\n");
    }


    printf("\n\n");
    if( (ret =os_info_version()) >= 0)
    {
        switch(ret)
        {
        case OS_VERSION_UBUNTU18:
            printf("system version is ubuntu18\n");
            break;
        case OS_VERSION_CENTOS7:
            printf("system version is centos7\n");
            break;
        default:
            printf("Current system version is not adapter\n");
            break;
        }
    }
    printf("\n***************************output info  end *****************************************\n\n");



    return 0;
}
