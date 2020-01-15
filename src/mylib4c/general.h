#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <sys/types.h>
#include <dirent.h>
#include "basecomm.h"

typedef int (*gen_procpid_cb)(const char *name,void *usrarg);

extern int general_find_proc_pid(gen_procpid_cb callback,void *userarg);
extern int general_get_field(char *tmpbuff,char key,int fieldnum,char **field);


struct dir_entry{
     DIR *dir;
     struct dirent *dent;
};
extern int general_dir_entry(const char *path,struct dir_entry *entry);
extern void general_close_dir(struct dir_entry *entry);


extern int general_os_version(void);


#endif
