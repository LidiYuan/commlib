#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
    struct sockaddr_in seraddr={0};
    int sockfd;
    char buff[256]={0};

    if(argc < 2)
    {
        printf("Usage acceptcli  ip ....\n");
	return -1;
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(8090);
    if(0 == inet_aton(argv[1],&seraddr.sin_addr) )
    {
        printf("conver %s to net is error\n",argv[1]);
	return -1;
    }

    sockfd = netutil_socket_block_connect((struct sockaddr *)&seraddr,sizeof(seraddr));
    if(sockfd <=0 )
    {
        printf("connect error\n");
	return -1;
    } 
   
    if( write(sockfd,"hello",strlen("hello")) <=0)
    {
        printf("send data error\n");
    }
    else
    {
        printf("****Write: %s\n","hello");
    }

    if( read(sockfd,buff,256) <= 0 )
    {
        printf("read error\n");
    }
    else
    {
        printf("***read: %s\n",buff);
    }

    close(sockfd);
            
    return 0;
}
