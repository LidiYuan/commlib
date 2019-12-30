#include <stdlib.h>
#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include "gencompo4c.h"


int com_foreach_local_ipv4(COMM_SIP_CALLBACK singleip,void *arg)
{
    int ret = 0;
    char ipbuff[128]={0};
    struct ifaddrs *tmpif,*ifaddr=NULL; 
    void *ipaddr = NULL;  

    if( -1 == getifaddrs(&ifaddr) )
       return -1;

    tmpif = ifaddr;
    
    while(tmpif)
    {
        memset(ipbuff,0,128);  
        if( AF_INET == tmpif->ifa_addr->sa_family )
        {
            ipaddr = &((struct sockaddr_in *)tmpif->ifa_addr)->sin_addr;
            if( inet_ntop(AF_INET, ipaddr, ipbuff, INET_ADDRSTRLEN) )
            {
                ret = singleip(arg,ipbuff);
                if( COMM_STOP_IPGET == ret )
                {
                    ret = 0;   
                    goto out;
                }
                else if( COMM_CONTINUE_IPGET != ret )
                {
                    goto out;
                }
            } 
        }
        tmpif=tmpif->ifa_next;
    }   
out: 
    if(NULL != ifaddr)
        freeifaddrs(ifaddr);

    return ret;
}


int com_is_local_ipv4(const char *ipv4addr)
{
    int ret = 0;
    char ipbuff[128]={0};
    struct ifaddrs *tmpif,*ifaddr=NULL;
    void *ipaddr = NULL;

    if( NULL == ipv4addr || ipv4addr[0] == '\0' )
        return 0;

    if( -1 == getifaddrs(&ifaddr) )
       return 0;

    tmpif = ifaddr;
    while(tmpif)
    {
        memset(ipbuff,0,128);
        if( AF_INET == tmpif->ifa_addr->sa_family )
        {
            ipaddr = &((struct sockaddr_in *)tmpif->ifa_addr)->sin_addr;
            if( inet_ntop(AF_INET, ipaddr, ipbuff, INET_ADDRSTRLEN) )
            {
                if( !strncmp(ipbuff,ipv4addr,strlen(ipbuff)) ) 
                {
                    ret = 1;
                    break;   
                }  
            }
        }
        tmpif=tmpif->ifa_next;
    }

    if( NULL != ifaddr )
        freeifaddrs(ifaddr);

    return ret;
}





