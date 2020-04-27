#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>
#include <signal.h>
#include <unistd.h>

void dealhandler(int sig)
{
    printf("hello %d\n",sig);

    //将信号删除
    if(sigutil_sig_del(sig) )
    {
        printf("del sig %d is error\n",sig);
    } 
    return;
}


int main(int argc,char *argv[])
{
    //添加SIGINT的信号处理
    if(sigutil_sig_add(SIGINT,dealhandler) )
    {
        printf("add SIGINT error\n");
    }

    while(1)
    {
        sleep(2);
    }

    return 0;
}
