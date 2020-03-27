#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>


int callback(const char *fpath,void *userarg)
{
    printf(" %s\n",fpath);
        
    return 0; // -1   stop list
}

int main(int argc,char *argv[])
{
    int mt;
    char buff[512]={0};
    struct task_proc_stat state={0};
    int pid;

    if(argc < 2)
    {
        printf("Usage:  testproc  pid\n");
	return -1;
    } 
    pid = atoi(argv[1]);
    if(pid <=0 )
    {
        printf("Usage:  testproc  pid\n");
        return -1;
    }

    if( com_find_proc_pid(callback,NULL) != 0 )
       printf("list error\n");

    printf("*****************proc info**************\n");
    printf("max process number: %ld\n",process_max_number());
    if(taskutil_kernel_task(pid) )
    {
        printf("pid %d  is a kernel thread\n",pid);
    }
    else
    {
        printf("pid %d  is not a kernel thread\n",pid); 
    }

    mt = process_cpu_mtime(2);
    if( mt >=0 )
    {
        printf("pid 2 cpu millsecond time: %d(ms)\n",mt);
    }

    if(0 == process_cmdline(1,buff,512) )
        printf("pid 1 cmdline: %s\n",buff);
 
    printf("**********************proc stat****start**************************\n");
    if(0 == taskutil_task_proc_stat(pid, &state) )
    {
        printf("Pid= %u\n",state.pid);
	printf("Comm= %s\n",state.comm);
	printf("State= ");
	switch(state.state)
	{
	 case TASK_STAT_RUNNIG:
             printf("Running ...\n");
	     break;
         case TASK_STAT_INTERRUPTIBLE:
             printf("Sleeping in an interruptible wait\n");
             break;
         case TASK_STAT_UNINTERRUPTIBLE:
             printf("Waiting in uninterruptible disk sleep\n");
             break;
         case TASK_STAT_ZOMBIE:
             printf("Zombie\n");
             break;
         case TASK_STAT_STOP:
             printf("Stopped (on a signal) or (before Linux 2.6.33) trace stopped\n");
             break;
         case TASK_STAT_TASK_TRACED:
             printf("Tracing stop (Linux 2.6.33 onward)\n");
             break;
         case TASK_STAT_DEAD:  
             printf("Dead\n");
             break;
         default:
             break;     
	}   

	printf("ppid= %u\n",state.ppid);
	printf("pgid= %u\n",state.pgid);
	printf("session= %u\n",state.session);
	printf("ttynr major= %d minor=%d \n",state.tty_nr_major,state.tty_nr_minor);
	printf("tpgid= %d\n",state.tpgid);
	printf("flags= 0x%x\n",state.flags);
    }
    printf("**********************proc stat****stop**************************\n");


    return 0;
}
