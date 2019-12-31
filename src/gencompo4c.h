#ifndef __GENCOMPO4C_H
#define __GENCOMPO4C_H

#define RET_SUCCESS   0
#define RET_FAILED   -1

#define MAX_PATH_LEN 1024


/////////////////////about net/////////////////////////
#define COMM_STOP_IPGET -1
#define COMM_CONTINUE_IPGET 0
typedef int (*COMM_SIP_CALLBACK)(void *arg,const char *ipbuff);

extern int com_foreach_local_ipv4(COMM_SIP_CALLBACK singleip,void *arg);
extern int com_is_local_ipv4(const char *ipv4addr);

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

typedef int(*netproc_info_cb)(NetProcInfo *pinfo,void *userarg);  
extern  int com_foreach_net_info(netproc_info_cb callback, unsigned int nettype,void*userarg); /*udp or tcp*/


////////////////////////about file///////////////////////////
typedef int (*procpid_cb)(const char *name,void *usrarg);
extern int com_find_proc_pid(procpid_cb callback,void *userarg);


#endif
