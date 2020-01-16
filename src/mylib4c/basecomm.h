#ifndef _BASECOMM_H_
#define _BASECOMM_H_

enum{
    OS_VERSION_UBUNTU18 = 0,
    OS_VERSION_CENTOS7,
    OS_VERSION_MAX_NUM
};

#define MAX_PATH_LEN 1024


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


#endif
