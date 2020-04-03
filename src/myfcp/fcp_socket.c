#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "fcp_socket.h"


int netutil_socket_nonblocking(int sockfd)
{
    int flag;

    if ( (flag = fcntl(sockfd, F_GETFL, NULL)) < 0 )
    {
        return -1;
    }

    if( !(flag & O_NONBLOCK) )
    {
        if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0 )
        {
            return -1;
        }
    }

    return 0;
}



int netutil_socket_reuse_addr(int sockfd)
{
    int value = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*) &value,sizeof(value));
}


int netutil_socket_reuse_port(int sockfd)
{
#if defined(SO_REUSEPORT)
    int value = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void*) &value,sizeof(value));
#else
    return -1;
#endif
}


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


int netutil_socket_nonblock_connect(const struct sockaddr *sa, int socklen)
{
    int sockfd;
    int ret = 0;

    if( (sockfd = socket(sa->sa_family,SOCK_STREAM,0)) < 0 )
            return -1;
    if( netutil_socket_nonblocking(sockfd) < 0 )
    {
        ret = -1;
        goto done;
    }

    if ( connect(sockfd, sa, socklen) < 0 )
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



int netutil_socket_unix_pair(int pair[2])
{

    if( 0 != socketpair(AF_UNIX,SOCK_STREAM,0,pair) )
        return -1;

    return 0;
}


int netutil_socket_accept(int backlog,struct sockaddr *seraddr,int addrlen, pf_set_listen_socket listensock_callback, pf_accept_callback accept_callback)
{
    int sockfd;
    int newfd;
    int cliaddrlen;
    struct sockaddr cliaddr;

    sockfd = socket(seraddr->sa_family,SOCK_STREAM,0);
    if(sockfd <= 0)
    {
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
        return -1;
    }

    if( 0 != listen(sockfd,backlog)  )
    {
        close(sockfd);
        return -1;
    }

con:
    cliaddrlen = addrlen;
    newfd = accept(sockfd,&cliaddr,&cliaddrlen);

    if(NULL != accept_callback)
    {
       if( 0 == accept_callback(newfd,&cliaddr,cliaddrlen) )
       {
           goto con;        
       }
    }

    close(sockfd);

    return 0;    
}



