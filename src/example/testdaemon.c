#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <myfcp.h>




int main(int argc,char *artgv[])
{
    
    int fd;
    pid_t pid;

    printf("*****start a daemon\n");
    taskutil_task_to_daemon(0,0,0,0);
    printf("this is daemon\n");
    
    
    fd = open("/var/log/testdaemon",O_CREAT|O_WRONLY|O_APPEND,0644);
    if(fd < 0)
    {
        printf("open file error\n");
	return -1;
    }

    //(守护进程设置SIGCHILD忽略后)测试子进程是否会成为僵尸进程 
    //用ps -ef | grep Z 看是否会存在僵尸进程
    pid = fork();
    if(pid <=0)
    {
        if(write(fd,"child:hello\n",strlen("child:hello\n")) <=0 )
	{
	    printf("write error\n");
	}
	exit(0);
    }


    if(write(fd,"this is a daemon\n",strlen("this is a daemon\n")) <= 0)
    {
        printf("write error\n");
    }
    
    
    while(1)
    {
        if( write(fd,"hello daemon\n",strlen("hello daemon\n")) <=0 )
	{
	    printf("write error\n");
	}

	sleep(1);
    }

    close(fd);
    return 0;
}
