#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>

#include "general.h"
#include "fcp_proc.h"


/*
  函数功能 获得elf的类型
  返回值为ELF_TYPE_* 类型 
*/

int taskutil_elf_type(const char *filename)
{
    FILE *fp = NULL;
    Elf32_Ehdr  hdr={0};
    int ret;

    fp = fopen(filename,"r");
    if(NULL == fp)
           return ELF_TYPE_ERROR;

    if((ret = fread(&hdr,1,sizeof(hdr),fp)) != sizeof(hdr))
    {
         fclose(fp);
         return ELF_TYPE_OTHER;
    }
    fclose(fp);

    if( !(hdr.e_ident[0] == 0x7f  && hdr.e_ident[1] == 'E' &&  hdr.e_ident[2] == 'L' && hdr.e_ident[3] == 'F') )
       return ELF_TYPE_OTHER;

    switch(hdr.e_type)
    {
        case ET_REL:
            return ELF_TYPE_REL;
        case ET_EXEC:
            return ELF_TYPE_EXE;
        case ET_DYN:
            return ELF_TYPE_DYN;
        case ET_CORE:
            return ELF_TYPE_CORE;
        default:
            return ELF_TYPE_OTHER;
    }

    return ELF_TYPE_ERROR;
}



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



int taskutil_task_proc_stat(unsigned int pid, struct task_proc_stat *pstat)
{
    struct stat st;
    char path[256]={0};
    FILE *fp;
    char databuff[2048]  ={0};
    char *field[9] = {0};
    int tty_nr = 0;

    if( NULL == pstat)
    {
        return -1;    
    }

    snprintf(path,256,"/proc/%u/stat",pid);
    if( 0 != stat(path,&st) )
    {
        return -1; 
    }
    
    fp = fopen(path,"r");
    if(NULL == fp)
        return -1;
    
    if( NULL == fgets(databuff,2048,fp) )
    {
        fclose(fp);
	return -1;
    }

    if( -1 == general_get_field(databuff,' ',9,field))
    {
        fclose(fp);
	return -1;
    }
    
    if(NULL != field[0] )  //pid
        pstat->pid = atoi(field[0]);
    if(NULL != field[1])   //comm
    {
        snprintf(pstat->comm,256,"%s",field[1]+1);
	if( pstat->comm[strlen(pstat->comm) -1] == ')' )
            pstat->comm[strlen(pstat->comm) -1] = '\0';	
    }   
    if(NULL != field[2]) //state
    {
        if( !strcmp("R",field[2]) )       pstat->state = TASK_STAT_RUNNIG;
	else if( !strcmp("S",field[2]) )  pstat->state = TASK_STAT_INTERRUPTIBLE;
	else if( !strcmp("D",field[2]) )  pstat->state = TASK_STAT_UNINTERRUPTIBLE;
	else if( !strcmp("Z",field[2]) )  pstat->state = TASK_STAT_ZOMBIE;
	else if( !strcmp("T",field[2]) )  pstat->state = TASK_STAT_STOP;
	else if( !strcmp("t",field[2]) )  pstat->state = TASK_STAT_TASK_TRACED;
	else if( !strcmp("X",field[2]) )  pstat->state = TASK_STAT_DEAD;
	else if( !strcmp("x",field[2]) )  pstat->state = TASK_STAT_DEAD;
	else pstat->state = TASK_STAT_UNKNOW; 
    }
    if(NULL != field[3] )// ppid
    {
        pstat->ppid = atoi(field[3]);     
    }

    if(NULL != field[4])  //pgid
    {
        pstat->pgid = atoi(field[4]);     
    }

    if(NULL != field[5]) //session
    {
        pstat->session = atoi(field[5]);     
    }

    if(NULL != field[6]) //tty_nr
    {  
        tty_nr = atoi(field[6]);     
        pstat->tty_nr_major  = major(tty_nr);
        pstat->tty_nr_minor  = minor(tty_nr);
    }

    if(NULL != field[7] ) //tpgid
    {  
        pstat->tpgid = atoi(field[7]);     
    }

    if(NULL != field[8]) //task->flags
    {
        pstat->flags = atoi(field[8]);
    }

    fclose(fp);
    return 0;
}


int taskutil_kernel_task(unsigned int pid)
{
    struct task_proc_stat state={0};  
 
    if( 0 != taskutil_task_proc_stat(pid, &state))
    {
        return 0;
    }
    if(state.flags & 0x00200000)  //PF_KTHREAD
        return 1;

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
    struct fcp_one_item ent={0}; //must init

    snprintf(threadpath,512,"/proc/%u/task/",pid);

    while( !general_foreach_dir_entry(threadpath,&ent) )
    {
        snprintf(path,512,"%s/schedstat",ent.data);

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


void taskutil_task_to_daemon(int nochdir,int nofileclose,int nostdclose,int nochildign)
{
    pid_t pid;
    rlim_t     i = 0;
    struct rlimit rl;
    rlim_t filenum;
    int tmpfd;

    pid = fork();
    if(pid > 0)
    {
        exit(0);
    }
    if(pid < 0)
    {
        exit(-1);
    }

    if(setsid() < 0 )
    {
        exit(-1);
    }
    
    pid = fork();
    if(pid > 0)
    {
        exit(0);
    }

    if(pid < 0)
    {
        exit(-1);
    }
      
    
    if(!nostdclose)
    {
        tmpfd = open("/dev/null", O_RDWR);	    
        for (i = 0; i < 3; i++)
        {
	    dup2(tmpfd, i);
        }

	if(tmpfd > 2)
	    close(tmpfd);
    }

    if(!nofileclose)
    {
        if (getrlimit(RLIMIT_NOFILE, &rl)  < 0)
	{
	    exit(-1);
	}

        filenum =rl.rlim_max;
	if(RLIM_INFINITY == filenum)
	{
	    filenum = 1024;
	}

        for(i=3; i<filenum; i++)
	{
	    close(i); 
	}
    }
   
    if(!nochdir)
    {
        if(chdir("/") < 0)
	{
	    exit(-1);
	}
    }

    if(!nochildign)
    {
        signal(SIGCHLD,SIG_IGN);
    }

    umask(0);

    return;    

}




