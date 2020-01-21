#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "general.h"
#include "fcp_proc.h"



int com_find_proc_pid(procpid_cb callback,void *userarg)
{
     return general_find_proc_pid(callback,userarg);
}

long process_max_number(void)
{
    FILE *fp = NULL;
    char buff[20]={0};
    long number = 0;

    fp = fopen("/proc/sys/kernel/pid_max","r");
    if(NULL == fp)
        return -1;

    if(NULL == fgets(buff,20,fp) )
    {
        fclose(fp);
        return -1;
    }
    number = atol(buff);

    fclose(fp);
    return number;
}

int is_kernel_thread(unsigned int pid)
{
    char buff[1024]={0};
    struct stat st;

    snprintf(buff,1024,"/proc/%u/",pid);
    if( stat(buff,&st) != 0)
    {
        return 0;
    }

    snprintf(buff,1024,"/proc/%u/exe",pid);
    if( 0 != stat(buff,&st) )
    {
        return 1;
    }

    return 0;
}

int process_cpu_mtime(unsigned int pid)
{
    FILE *fp = NULL;
    char buff[256]={0};
    char path[512]={0};
    char threadpath[512]={0};
    long long nt = 0;
    int summt = 0;
    struct file_item ent={0}; //must init

    snprintf(threadpath,512,"/proc/%u/task/",pid);

    while( !general_foreach_dir_entry(threadpath,&ent) )
    {
        snprintf(path,512,"%s/schedstat",ent.fullpath);

        fp = fopen(path,"r");
        if( NULL == fp)
        {
            memset(path,0,512);
            continue;
        }

        if( fgets(buff,256,fp) )
        {
            nt = atoll(buff);
            summt += (nt / 1000000);
        }
        fclose(fp);
        memset(buff,0,256);
        memset(path,0,512);
    }

    return summt;
}


int process_cmdline(unsigned int pid,char *linebuff,unsigned int size)
{
    FILE *fp = NULL;
    char buff[1024]={0};
    int i;

    if(NULL == linebuff || size == 0)
        return -1;

    snprintf(buff,1024,"/proc/%u/cmdline",pid);
    fp = fopen(buff,"r");
    if(NULL == fp)
        return -1;

    memset(linebuff,'\n',size);
    if( NULL == fgets(linebuff,size,fp) )
    {
        fclose(fp);
        return -1;
    }

    for(i = 0; i< size; i++)
    {
        if(linebuff[i] == '\n')
        {
            linebuff[i] = '\0';
            break;
        }
        if(linebuff[i] == '\0')
        {
            linebuff[i] = ' ';
        }
    }

    fclose(fp);

    return 0;
}







