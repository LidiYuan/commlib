#ifndef __GENERAL_H
#define __GENERAL_H

#include "gencompo4c.h"
extern int general_find_proc_pid(procpid_cb callback,void *userarg);
extern int general_get_field(char *tmpbuff,char key,int fieldnum,char **field);



#endif
