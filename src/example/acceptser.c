#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int net_accept_callbck(int sockfd, struct sockaddr *addr,int addrlen)
{
    char buff[256]={0};

    if( read(sockfd,buff,256) <=0 )
    {
        printf("server: read error\n");
    }
    else
    {
        printf("Server: read %s\n",buff);
    }

    if( write(sockfd,buff,strlen(buff)) <=0 )
    {
        printf("Server: write error\n");
    }
    else
    {
        printf("Server: write %s\n",buff);
    }


    close(sockfd);

    return 0;
}


int net_set_listen_sock(int listensock)
{
    netutil_socket_reuse_addr(listensock);
    return 0;
}


int main(int argc,char *argv[])
{
    
    int sockfd;
    struct sockaddr_in seraddr={0};

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(8090);
    seraddr.sin_addr.s_addr = INADDR_ANY;    
 
    if( (sockfd = netutil_socket_accept(-1, 5, (struct sockaddr*)&seraddr,sizeof(seraddr),net_set_listen_sock,net_accept_callbck)) <= 0)
    {
        printf("create accept error\n");
    }
        
    return 0;
}
