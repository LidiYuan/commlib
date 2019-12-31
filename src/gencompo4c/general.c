#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "general.h"

#define PROC_DIR "/proc"

int general_find_proc_pid(procpid_cb callback,void *userarg)
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
     closedir(dir);

     return RET_SUCCESS;
}


int general_get_field(char *tmpbuff,char key,int fieldnum,char **field)
{
    char *pstr = tmpbuff;
    int buflen;
    int fieldindex = 0;
    
    if(NULL == tmpbuff || NULL == field || fieldnum <=0 || tmpbuff[0] == '\0' )
        return -1; 
    
    buflen = strlen(tmpbuff);                                           
    
    while(*pstr == ' ')
        pstr++;
    
    if( '\n' ==  pstr[buflen-1] )
        pstr[buflen-1] = '\0';

    buflen = strlen(pstr);
    if(0 == buflen)
        return -1;
 
    field[fieldindex ++] = pstr;
    for( ; *pstr != '\0' ; pstr++ )
    {
        if( *pstr == key)
        {
            *pstr = '\0';
            if(fieldindex >= fieldnum )
                break;

            pstr++;
            while(*pstr == ' ' || *pstr == '\t')
                pstr++;

            field[ fieldindex++] = pstr;
        }
    }
    return 0;
}

