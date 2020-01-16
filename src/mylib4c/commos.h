#ifndef _COMMOS_H_
#define _COMMOS_H_

#include "basecomm.h"

#define MAX_USER_NAME 128
#define MAX_TTY_LEN  128
#define MAX_HOST_NAME 256

struct login_info
{
    char username[MAX_USER_NAME];
    time_t logintime;
    char tty[MAX_TTY_LEN];
    char hostname[MAX_HOST_NAME];
    time_t  logouttime;
};

typedef int (*login_info_cb)(struct login_info *uinfo,void *userarg);

extern int os_info_uuid(char *buff,unsigned int bufsize);
extern int os_info_bootid(char *buff,unsigned int bufsize);
extern int os_info_machine_id(char*buff, unsigned int size); /*size you can set MAX_MACHINE_ID_LEN*/


extern time_t os_info_boot_time(void);
extern time_t os_info_last_shutdow_time(void);

extern int os_info_running_tty(login_info_cb callback,void *userarg);
extern int os_info_logout_tty(login_info_cb callback, void *userarg);
extern int os_info_version(void);  /*return OS_VERSION_xx  basecomm.h*/




#endif
