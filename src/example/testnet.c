#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>


static int iploopcallback(void *arg, const char *ipaddr)
{
    printf("%s\n",ipaddr);	
    return COMM_CONTINUE_IPGET;
}

static int netprocinfo(NetProcInfo *pinfo,void *userarg)
{
    printf("\n");	
    printf("bucket:%d\nsrcaddr:%s\nsrcport:%d\ndstaddr:%s\ndstport:%d\nconnstate:%d\nuid:%d\ninode:%d\nrefcnt:%d\nsockadr:%p\nsendbytes:%u\nrecvbytes:%u\npid:%d ==> %s\n",pinfo->bucket, pinfo->srcaddr,
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

    printf("\n******************tcp info******************************\n");
    com_foreach_net_info(netprocinfo, NETTYPE_TCP,NULL);
    
    printf("\n******************udp info******************************\n");
    com_foreach_net_info(netprocinfo, NETTYPE_UDP,NULL);


    printf("\n******************get arp info**************************\n");
    struct fcp_one_item entry={0};

    printf("permit arp:\n");
    foreach_permit_arp(&entry)
    {
        printf("ipaddr[%s] -- hwaddr[%s] -- devname[%s] -- hwtype[%d] -- arpflag[%d]\n",entry.arp_ipv4,entry.arp_hwaddr,entry.arp_devname,entry.arp_hwtype,entry.arp_flag);
    }

    printf("complete arp:\n");
    foreach_complete_arp(&entry)
    {
        printf("ipaddr[%s] -- hwaddr[%s] -- devname[%s] -- hwtype[%d] -- arpflag[%d]\n",entry.arp_ipv4,entry.arp_hwaddr,entry.arp_devname,entry.arp_hwtype,entry.arp_flag);

    }

    printf("publish arp:\n");
    foreach_publish_arp(&entry)
    {
        printf("ipaddr[%s] -- hwaddr[%s] -- devname[%s] -- hwtype[%d] -- arpflag[%d]\n",entry.arp_ipv4,entry.arp_hwaddr,entry.arp_devname,entry.arp_hwtype,entry.arp_flag);

    }
    
    return 0;
}
