#ifndef _FCP_NET_H_
#define _FCP_NET_H_

#include "fcp_base.h"

#define COMM_STOP_IPGET -1
#define COMM_CONTINUE_IPGET 0


#define MAX_IPV4_STRLEN 16
#define NETTYPE_TCP  0
#define NETTYPE_UDP  1



/***connect state***/
enum{
    ESTABLISHED=1,
    SYN_SENT,
    SYN_RECV,
    FIN_WAIT1,
    FIN_WAIT2,
    TIME_WAIT,
    CLOSE,
    CLOSE_WAIT,
    LAST_ACK,
    LISTEN,
    CLOSING
};


typedef struct{
    unsigned int bucket;
    char srcaddr[MAX_IPV4_STRLEN];
    unsigned int srcport;
    char dstaddr[MAX_IPV4_STRLEN];
    unsigned int dstport;
    int connstate;   //connect state
    unsigned int sendbytes;
    unsigned int recvbytes;
    unsigned int uid;
    unsigned int inode;
    unsigned int skrefcnt;
    unsigned long sockaddr;
    int pid;
    char procpath[MAX_PATH_LEN];

}NetProcInfo;

typedef int (*netipv4_cb)(void *arg,const char *ipbuff);
typedef int (*netproc_info_cb)(NetProcInfo *pinfo,void *userarg);

extern int com_foreach_local_ipv4(netipv4_cb singleip,void *arg);
extern int com_is_local_ipv4(const char *ipv4addr);
extern int com_foreach_net_info(netproc_info_cb callback, unsigned int nettype,void*userarg); /*udp or tcp*/


#endif
