#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "gencompo4c.h"

#define PROC_DIR "/proc"
int comm_find_proc_pid(procpid_cb callback,void *userarg)
{
     struct dirent *pent =NULL;
     char *pathbuff = NULL;
     int ret = 0;

     DIR *dir = opendir(PROC_DIR);

     if(NULL == dir)
         return RET_FAILED;

     while( NULL != ( pent = readdir(dir))  )
     {
         if(!strcmp(pent->d_name,".") || !strcmp(pent->d_name,".."))
                 continue;

         if( pent->d_type != DT_DIR )
                 continue;

         if( 0 == atoi(pent->d_name) )
                 continue;

          pathbuff = (char *)malloc(sizeof(char)*(strlen(PROC_DIR) + strlen(pent->d_name) + 10));
          if(NULL == pathbuff)
              continue;

          memset(pathbuff,0,sizeof(char)*(strlen(PROC_DIR) + strlen(pent->d_name) + 10));
          sprintf(pathbuff,"%s/%s",PROC_DIR,pent->d_name);
          ret = callback(pathbuff,userarg);
          free(pathbuff);
          pathbuff = NULL;
          if( 0 != ret )
              break;
     }

     return RET_SUCCESS;
}



