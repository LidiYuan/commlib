#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum{
   OS_INFO_UUID =0,
   OS_INFO_BOOTID
};

static int os_info(char *buff,int type,unsigned int buffsize)
{
    FILE *fp = NULL;	
    switch(type)
    {
    case OS_INFO_UUID:
	 fp = fopen("/proc/sys/kernel/random/uuid","r");
         if(NULL == fp)
	     return -1;
         if(NULL == fgets(buff,(int)buffsize,fp))
	     return -1;
         
	 if(buff[strlen(buff) -1] == '\n') 	 
             buff[strlen(buff) -1] = '\0';
      
         break;
    case OS_INFO_BOOTID:
	 fp = fopen("/proc/sys/kernel/random/boot_id","r");
         if(NULL == fp)
	     return -1;

         if(NULL == fgets(buff,(int)buffsize,fp))
	     return -1;
         
	 if(buff[strlen(buff) -1] == '\n') 	 
             buff[strlen(buff) -1] = '\0';
      
         break;

    default:
	    break;
    }

    return 0;
}

int os_info_uuid(char *buff,unsigned int bufsize)
{
    if(NULL == buff)
	    return -1;
       
    return os_info(buff,OS_INFO_UUID,bufsize);
}

int os_info_bootid(char *buff,unsigned int bufsize)
{
    if(NULL == buff)
	    return -1;

    return os_info(buff,OS_INFO_BOOTID,bufsize);  
}




