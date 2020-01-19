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



enum{
    LOOP_TYPE_START,
    LOOP_TYPE_CON,
    LOOP_TYPE_STOP,
    LOOP_TYPE_MAX
};


struct file_item {
    void *this;
    char *fullpath;
    int flag;
};


typedef void(*genlog_cb)(const char *msg);
extern void fcp_set_log_cb(genlog_cb errcb, genlog_cb debugcb);

#endif
