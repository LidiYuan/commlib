#ifndef __GENERAL_H
#define __GENERAL_H

#include <sys/types.h>
#include <dirent.h>

#include "gencompo4c.h"
extern int general_find_proc_pid(procpid_cb callback,void *userarg);
extern int general_get_field(char *tmpbuff,char key,int fieldnum,char **field);

struct dir_entry{
     DIR *dir;
     struct dirent *dent;
};
extern int general_dir_entry(const char *path,struct dir_entry *entry);
extern void general_close_dir(struct dir_entry *entry);

#endif
