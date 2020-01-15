#include <stdio.h>
#include <stdlib.h>
#include <mylib4c.h>


static int iploopcallback(void *arg, const char *ipaddr)
{
    printf("%s\n",ipaddr);	
    return COMM_CONTINUE_IPGET;
}

static int netprocinfo(NetProcInfo *pinfo,void *userarg)
{
    printf("%d %s %d %s %d %d  %d %d %d %p  %u %u :%d ==> %s\n",pinfo->bucket, pinfo->srcaddr,
                                                    pinfo->srcport,pinfo->dstaddr,
                                                    pinfo->dstport,pinfo->connstate,
                                                    pinfo->uid,pinfo->inode,
                                                    pinfo->skrefcnt, (void*)(pinfo->sockaddr),
                                                    pinfo->sendbytes,pinfo->recvbytes,pinfo->pid,pinfo->procpath);

    return 0;
}

int main(int argc,char *argv[])
{

#if 1	
    //get local ipv4 test1	
    if( 0 != com_foreach_local_ipv4(iploopcallback,NULL) )
    {
        printf("get ip addr is error\n");
	return -1;
    }
#endif

#if 1 
    if( com_is_local_ipv4("192.168.129.134") )
    {
        printf("ip is local ipv4 addr\n");
    }
    else
    {
        printf("ip is not local ipv4 addr\n");
    }
#endif

    com_foreach_net_info(netprocinfo, NETTYPE_TCP,NULL);
    com_foreach_net_info(netprocinfo, NETTYPE_UDP,NULL);





    
    return 0;
}
