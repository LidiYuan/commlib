#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "general.h"
#define PROC_DIR "/proc"

struct general_inode_entry{
    int  bufsize;
    char *fullpath;
    char *dirpath;
    DIR *dir;
    struct  dirent *ent;
};


static int general_foreach_inode_entry(const char *dirpath, struct fcp_one_item *entry,int type)
{
    struct general_inode_entry* pNode = NULL;

    if(NULL == entry || NULL == dirpath )
        return -1;

    switch(entry->flag)
    {
    case LOOP_TYPE_START:

        pNode = (struct general_inode_entry *)malloc(sizeof(struct general_inode_entry));
        if(NULL == pNode)
            return -1;

        memset(pNode,0,sizeof(struct general_inode_entry));
        pNode->dirpath = (char*)malloc(strlen(dirpath) +10);
        if(NULL == pNode->dirpath)
        {
            free(pNode);
            return -1;
        }
        memset(pNode->dirpath,0,strlen(dirpath)+10);
        memcpy(pNode->dirpath,dirpath,strlen(dirpath));

        pNode->dir = opendir(dirpath);
        if(NULL == pNode->dir)
        {
            free(pNode->dirpath);
            free(pNode);
            return -1;
        }
        entry->flag = LOOP_TYPE_CON;
        entry->this = (void*)pNode;

        break;
    case LOOP_TYPE_CON:
        pNode = (struct general_inode_entry *)(entry->this);
        if(NULL != pNode->fullpath)
        {
            free(pNode->fullpath);
            entry->data = pNode->fullpath= NULL;
            pNode->bufsize = 0;
        }
        break;
    case LOOP_TYPE_STOP:
        pNode = (struct general_inode_entry *)(entry->this);
        goto stopread;
    default:
        return -1;
    }

    do{

        pNode->ent = readdir(pNode->dir);
        if(NULL == pNode->ent)
            goto stopread;

    }while( !strcmp(pNode->ent->d_name,".") || !strcmp(pNode->ent->d_name,"..") || (pNode->ent->d_type != type));


    pNode->bufsize = strlen(pNode->dirpath)+strlen(pNode->ent->d_name)+10;
    pNode->fullpath = (char*)malloc(pNode->bufsize);
    if(NULL == pNode->fullpath)
            goto stopread;

    memset(pNode->fullpath,0,pNode->bufsize);

    if(pNode->dirpath[strlen(pNode->dirpath)-1] == '/')
        snprintf(pNode->fullpath,pNode->bufsize,"%s%s",pNode->dirpath,pNode->ent->d_name);
    else
        snprintf(pNode->fullpath,pNode->bufsize,"%s/%s",pNode->dirpath,pNode->ent->d_name);
    
    entry->data = pNode->fullpath;
    return 0;

stopread:

    if(NULL != pNode->fullpath)
    {
        free(pNode->fullpath);
        pNode->fullpath = NULL;
    }

    free(pNode->dirpath);
    closedir(pNode->dir);
    free(pNode);
    entry->data = NULL;
    entry->this = NULL;
    entry->flag = 0;

    return -1;
}


int general_foreach_dir_entry(const char *path,struct fcp_one_item *entry)
{
    return general_foreach_inode_entry(path,entry,DT_DIR);
}

int general_foreach_regfile_entry(const char *path,struct fcp_one_item *entry)
{
    return general_foreach_inode_entry(path,entry,DT_REG);
}


int general_find_proc_pid(gen_procpid_cb callback,void *userarg)
{
     int ret = 0;
     char *tmpstr = NULL;  
     struct fcp_one_item ent={0};/*must init*/ 

     while( !general_foreach_dir_entry(PROC_DIR, &ent) )
     {
         if( NULL == (tmpstr = strrchr(ent.data,'/')) )
            continue;

         if( 0 == atoi(tmpstr+1) )  
            continue;

          ret = callback(ent.data,userarg);
          
          if( 0 != ret )
          {
              ent.flag = LOOP_TYPE_STOP; /*stop loop*/     
              continue;
          }
      }

     return 0;
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



