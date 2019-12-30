#ifndef __GENCOMPO4C_H
#define __GENCOMPO4C_H

/////////////////////about net/////////////////////////
#define COMM_STOP_IPGET -1
#define COMM_CONTINUE_IPGET 0
typedef int (*COMM_SIP_CALLBACK)(void *arg,const char *ipbuff);

extern int com_foreach_local_ipv4(COMM_SIP_CALLBACK singleip,void *arg);
extern int com_is_local_ipv4(const char *ipv4addr);



////////////////////////about file///////////////////////////
typedef int (*pidcallback)(const char *fpath,void *userarg);
extern int comm_list_all_procpid(pidcallback callback,void *usrarg);


#endif
