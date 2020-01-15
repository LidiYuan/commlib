#include <stdio.h>
#include <stdlib.h>
#include "gencompo4c.h"


int callback(const char *fpath,void *userarg)
{
    printf(" %s\n",fpath);
        
    return 0; // -1   stop list
}

int main(int argc,char *argv[])
{
    int mt;
    char buff[512]={0};

    if( com_find_proc_pid(callback,NULL) != RET_SUCCESS )
       printf("list error\n");

    printf("*****************proc info**************\n");
    printf("max process number: %ld\n",process_max_number());
    if(is_kernel_thread(2) )
    {
        printf("pid 2  is a kernel thread\n");
    }

    mt = process_cpu_mtime(2);
    if( mt >=0 )
    {
        printf("pid 2 cpu millsecond time: %d(ms)\n",mt);
    }

    if(0 == process_cmdline(1,buff,512) )
        printf("pid 1 cmdline: %s\n",buff);
    

    return 0;
}
