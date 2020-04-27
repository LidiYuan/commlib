#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include "fcp_socket.h"
#include "genlog.h"



/*
 *功能: 将给定的socket设置为非阻塞socket
  参数: 
       sockfd  socket文件描述符
  返回值:成功返回0  失败返回-1
 * */
int netutil_socket_nonblocking(int sockfd)
{
    int flag;

    if ( (flag = fcntl(sockfd, F_GETFL, NULL)) < 0 )
    {
        print_err("get sockfd flag  err[%s]\n",strerror(errno));    
        return -1;
    }

    if( !(flag & O_NONBLOCK) )
    {
        if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0 )
        {
            print_err("set sockfd flag nonblock error[%s]\n",strerror(errno));    
            return -1;
        }
    }

    return 0;
}



/*
 *功能: 让处于timewait状态下的socket可以重新绑定
  参数:
      sockfd socket文件描述符
  返回值:成功返回0 失败返回-1
 * */
int netutil_socket_reuse_addr(int sockfd)
{
    int value = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*) &value,sizeof(value));
}


/*
 *功能: 允许多个进程或者线程绑定到同一端口，提高服务器程序的性能,需要linux版本支持SO_REUSEPORT
  参数:
       sockfd socket文件描述符
  返回值: 成功返回0 失败返回-1
 * */
int netutil_socket_reuse_port(int sockfd)
{
#if defined(SO_REUSEPORT)
    int value = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void*) &value,sizeof(value));
#else
    return -1;
#endif
}


/*
 *功能: 在进程创建子进程执行exec的时候不继承父进程的这个sockfd
  参数: 
       sockfd socket文件描述符
  返回值: 成功返回 0,失败返回-1
 * */
int netutil_socket_closeonexec(int sockfd)
{
    int flag;

    if ((flag = fcntl(sockfd, F_GETFD, NULL)) < 0)
    {
        return -1;
    }

    if ( !(flag & FD_CLOEXEC) )
    {
        if (fcntl(sockfd, F_SETFD, flag | FD_CLOEXEC) < 0)
        {
            return -1;
        }
    }

    return 0;

}

/*
  功能: 创建非阻塞链接
  参数:  
      sa: 要连接的服务器地址
      socklen: sa的真正长度
  返回值:
      成功返回创建的sock描述符,失败返回-1
  注意: 用户自己close socket文件描述符    
 * */
int netutil_socket_nonblock_connect(const struct sockaddr *sa, int socklen)
{
    int sockfd;
    int ret = 0;

    if( (sockfd = socket(sa->sa_family,SOCK_STREAM,0)) < 0 )
    {   
        print_err("socket create error[%s]\n",strerror(errno));    
        return -1;
    }

    /*设置成非阻塞*/
    if( netutil_socket_nonblocking(sockfd) < 0 )
    {
        ret = -1;
        goto done;
    }
 
    
    if ( connect(sockfd, sa, socklen) < 0 )
    {
        ret = -1;   
        print_err("socket connect error[%s]\n", strerror(errno));    
        goto done;
    }

    ret = sockfd;
done:
    if(ret == -1)
        close(sockfd);

    return ret;
}


/*
 *功能: 创建阻塞sock连接
  参数:
      sa: 要连接的服务器地址
      socklen: sa的真正长度
  返回值: 成功返回创建的sock描述符,失败返回-1
  注意: 用户自己close socket文件描述符
 * */
int netutil_socket_block_connect(const struct sockaddr *seraddr, int socklen)
{
    int sockfd;
    int ret = 0;

    if( (sockfd = socket(seraddr->sa_family,SOCK_STREAM,0)) < 0 )
            return -1;

    if ( connect(sockfd, seraddr, socklen) < 0 )
    {
        ret = -1;
        goto done;
    }

    ret = sockfd;

done:
    if(ret == -1)
        close(sockfd);

    return ret;
}



/*
 *功能: 建立socket对 用于父子进程间的全双工通信
 *参数: pair  sock对每个pair[x]即可读 也可写
 *返回值: 功能返回0，失败返回-1
 * */
int netutil_socket_unix_pair(int pair[2])
{

    if( 0 != socketpair(AF_UNIX,SOCK_STREAM,0,pair) )
    {
        print_err("create socket pair error[%s]\n",strerror(errno));    
        return -1;
    }

    return 0;
}



/*
 *功能: 创建服务端的监听端口 并等待连接的建立
  参数:
      sockfd: 监听端口sock 传入-1,则函数自动创建一个监听sock, 若传入的不为0,则函数只执行accept操作,每次返回都有一个连接建立
      backlog: 监听队列中未完成3次握手的队列的长度,用于传给listen
      seraddr:监听的服务端地址
      addrlen:监听服务端seraddr的真实长度
      listensock_callback: 在未执行listen之前用户可以对监听socket进行属性设置
      accept_callback:当收到一个连接时候的回调函数
  返回值:
       返回 监听socket描述符
  注意:需要用户自己close 监听和连接的socket的描述符
 * */
int netutil_socket_accept(int sockfd, int backlog,struct sockaddr *seraddr,int addrlen, pf_set_listen_socket listensock_callback, pf_accept_callback accept_callback)
{
    int newfd;
    int cliaddrlen;
    struct sockaddr cliaddr;

    if(sockfd < 0 )
    {
        sockfd = socket(seraddr->sa_family,SOCK_STREAM,0);
        if(sockfd <= 0)
        {
           print_err("create socket error\n",strerror(errno));     
           return -1;
        }

        if(NULL == listensock_callback)
        {
            if( 0 != listensock_callback(sockfd))
            {
                close(sockfd);
                return -1;
            }
        }

        if( 0 != bind(sockfd,seraddr, addrlen) )
        {
            close(sockfd);
            print_err("socket bind error\n",strerror(errno));     
            return -1;
        }

        if( 0 != listen(sockfd,backlog)  )
        {
            close(sockfd);
            print_err("create listen error\n",strerror(errno));     
            return -1;
        }
    }

    cliaddrlen = addrlen;
    newfd = accept(sockfd,&cliaddr,&cliaddrlen);
    if(NULL != accept_callback && newfd > 0)
    {
       if( 0 != accept_callback(newfd,&cliaddr,cliaddrlen))
       {
           print_debug("accpet callback error\n");
       }        
    }

    return sockfd;    
}



