#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "general.h"

#define PROC_DIR "/proc"

int com_find_proc_pid(procpid_cb callback,void *userarg)
{
     return general_find_proc_pid(callback,userarg);
}



