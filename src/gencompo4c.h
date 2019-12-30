#ifndef __GENCOMPO4C_H
#define __GENCOMPO4C_H

#define RET_SUCCESS   0
#define RET_FAILED   -1

/////////////////////about net/////////////////////////
#define COMM_STOP_IPGET -1
#define COMM_CONTINUE_IPGET 0
typedef int (*COMM_SIP_CALLBACK)(void *arg,const char *ipbuff);

extern int com_foreach_local_ipv4(COMM_SIP_CALLBACK singleip,void *arg);
extern int com_is_local_ipv4(const char *ipv4addr);



////////////////////////about file///////////////////////////
typedef int (*procpid_cb)(const char *name,void *usrarg);
extern int comm_find_proc_pid(procpid_cb callback,void *userarg);


#endif
