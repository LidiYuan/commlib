#ifndef _FCP_BASE_H_
#define _FCP_BASE_H_

#include <syslog.h>

enum{
    OS_VERSION_UBUNTU18 = 0,
    OS_VERSION_CENTOS7,
    OS_VERSION_MAX_NUM
};

#define MAX_PATH_LEN 1024
#define MAX_MACHINE_ID_LEN 34


#define IPV4_STR_LEN  16
#define ETH_HWADDR_STR_LEN 32
#define ETHDEV_NAME_LEN  256



enum {
    LOOP_TYPE_START,
    LOOP_TYPE_CON,
    LOOP_TYPE_STOP,
    LOOP_TYPE_MAX
};

struct fcp_one_item{
    void *this;
    char *data;
    unsigned int flag;  //LOOP_TYPE_XXX
    union {
        char *data;
        struct{
            char devname[ETHDEV_NAME_LEN];
            char ipv4[IPV4_STR_LEN];
            char hwaddr[ETH_HWADDR_STR_LEN];
            int  hwtype;
            int  arpflag;
        }arp_entry;
    }item;


#define arp_devname item.arp_entry.devname
#define arp_ipv4    item.arp_entry.ipv4
#define arp_hwaddr  item.arp_entry.hwaddr
#define arp_hwtype  item.arp_entry.hwtype
#define arp_flag    item.arp_entry.arpflag

#define buffdata   item.data;
};




typedef void(*genlog_cb)(const char *msg);
extern void fcp_set_log_cb(genlog_cb errcb, genlog_cb debugcb);
extern int program_is_running(const char *path);


#endif
