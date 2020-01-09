#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "general.h"

#define PROC_DIR "/proc"

int general_dir_entry(const char *path,struct dir_entry *entry)
{
    if(NULL == entry || path == NULL || strlen(path) == 0)
        return -1;
  
    if(entry->dir == NULL)
    {
        entry->dir = opendir(path);
        if(NULL == entry->dir)
            return -1;
    }

    do
    {
        entry->dent = readdir(entry->dir);
        if(NULL == entry->dent)
        {
            closedir(entry->dir);
            entry->dir = NULL;
            return -1;
        }
    }while( !strcmp(entry->dent->d_name,".") || !strcmp(entry->dent->d_name,"..") );


    return 0;
}

void general_close_dir(struct dir_entry *entry)
{
    if(NULL != entry && NULL != entry->dir)
    {
        closedir(entry->dir);
        entry->dir = NULL;
    }
}

int general_find_proc_pid(procpid_cb callback,void *userarg)
{
     char *pathbuff = NULL;
     int ret = 0;
     struct dir_entry ent={0}; //must init

     while( !general_dir_entry(PROC_DIR,&ent) )
     {
         if( ent.dent->d_type != DT_DIR )
                 continue;

         if( 0 == atoi(ent.dent->d_name) )
                 continue;

          pathbuff = (char *)malloc(sizeof(char)*(strlen(PROC_DIR) + strlen(ent.dent->d_name) + 10));
          if(NULL == pathbuff)
              continue;

          memset(pathbuff,0,sizeof(char)*(strlen(PROC_DIR) + strlen(ent.dent->d_name) + 10));
          sprintf(pathbuff,"%s/%s",PROC_DIR,ent.dent->d_name);
          ret = callback(pathbuff,userarg);
          free(pathbuff);
          pathbuff = NULL;
          if( 0 != ret )
              break;
     }
     general_close_dir(&ent);


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



