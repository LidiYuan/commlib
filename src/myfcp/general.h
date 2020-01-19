#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <sys/types.h>
#include <dirent.h>

#include "fcp_base.h"

typedef int (*gen_procpid_cb)(const char *name,void *usrarg);

extern int general_find_proc_pid(gen_procpid_cb callback,void *userarg);
extern int general_get_field(char *tmpbuff,char key,int fieldnum,char **field);


extern int general_foreach_dir_entry(const char *path,    struct file_item *entry);
extern int general_foreach_regfile_entry(const char *path,struct file_item *entry);


extern int general_os_version(void);


#endif
