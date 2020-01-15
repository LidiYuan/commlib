#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

static int general_os_version_con(const char *filepath,unsigned int fieldnum,char *verbuff,int bufsize)
{
    char **fieldptr = NULL;
    int ret = -1;
    FILE *fp = NULL;
    char buff[256]={0};

    if(NULL == filepath || 0 == fieldnum || NULL == verbuff )
        return -1;

    fieldptr = (char **)malloc(sizeof(char *)*fieldnum);
    if(NULL == fieldptr)
        return -1;
    memset(fieldptr,0,sizeof(char *)*fieldnum);

    if(NULL == (fp = fopen(filepath,"r")))
        goto done;

    if( NULL == fgets(buff,255,fp) )
        goto done;

    if(strlen(buff) <=0 )
        goto done;

    if( 0 != general_get_field(buff,' ',fieldnum,fieldptr) )
        goto done;

    if(NULL == fieldptr[0] || NULL == fieldptr[fieldnum-1] )
        goto done;

    snprintf(verbuff,bufsize,"%s%s",fieldptr[0],fieldptr[fieldnum-1]);
    ret = 0;

done:
    if(NULL != fp)
        fclose(fp);

    if(NULL != fieldptr)
        free(fieldptr);


    return ret;
}


int general_os_version(void)
{
    char osversion[256]={0};
    struct stat st;
    int i = 0;
    char *tmpptr;

    if( 0 == stat("/etc/redhat-release",&st) )
    {
        /*CentOS Linux release 7.6.1810 (Core)*/
        if( 0 != general_os_version_con("/etc/redhat-release",4,osversion,256) )
            return -1;
    }
    else if( 0 == stat("/etc/issue",&st) )
    {
        /*Ubuntu 18.04.2 LTS*/
        if( 0 != general_os_version_con("/etc/issue",2,osversion,256) )
            return -1;
    }
    else
        return -1;

    for(i = 0; i<strlen(osversion); i++)
    {
        if( osversion[i] >='A' && osversion[i] <='Z' )
        {
            osversion[i] = osversion[i] + ('a' - 'A');
        }
    }

    if( tmpptr = strchr(osversion,'.') )
    {
        *tmpptr = '\0';
    }


    if(!strcmp("ubuntu18",osversion))
        return OS_VERSION_UBUNTU18;
    if(!strcmp("centos7",osversion))
        return  OS_VERSION_CENTOS7;

    return -1;

}



