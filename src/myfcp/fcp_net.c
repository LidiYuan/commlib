#include <stdlib.h>
#include <stdio.h>      
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "general.h"
#include "fcp_net.h"
#include "genlog.h"

#define NETTCP_FILE   "/proc/net/tcp"
#define NETUDP_FILE   "/proc/net/udp"
#define PROCARPPATH   "/proc/net/arp"
#define PING_DEN_PATH "/proc/sys/net/ipv4/icmp_echo_ignore_all"

enum{
   NET_FIELD_BUCKET,
   NET_FIELD_SRCADDR,
   NET_FIELD_DSTADDR,
   NET_FIELD_STATE,
   NET_FIELD_SENDRECVBYTES,
   NET_FIELD_DEFAULT1,
   NET_FIELD_DEFAULT2, 
   NET_FIELD_UID,
   NET_FIELD_DEFAULT3, 
   NET_FIELD_INODE,
   NET_FIELD_SKREFCNT,
   NET_FIELD_SOCKADDR,
   NET_FIELD_MAX 
};


static int deal_proc_pid_info(const char *name,void *userarg)
{	
    char *pathbuff=NULL;
    char sympath[256]={0};
    DIR *dir = NULL;
    struct dirent *pent;
    int inode;
    char *pstr;
    int ret =0;

    NetProcInfo *pinfo = (NetProcInfo*)userarg;

    if(NULL == name || NULL == userarg)
	    return 0;
    
    pathbuff = (char *)malloc(strlen(name)+strlen("/fd") + 256);
    if(NULL == pathbuff)
	   return 0;
    
    memset(pathbuff,0,strlen(name)+strlen("/fd") + 256);
    sprintf(pathbuff,"%s/fd",name);

    dir = opendir(pathbuff);
    if(NULL == dir)
    {
        goto done;
    }
    
    while((pent = readdir(dir) ))
    {
	memset(sympath,0,256);    
	sprintf(pathbuff,"%s/fd/%s",name,pent->d_name);    
        if( readlink(pathbuff,sympath,256) < 0)
		continue;
	if( !strstr(sympath,"socket:"))	
		continue;
        pstr = strchr(sympath,'[');
        if(NULL == pstr)
	    continue;
        pstr ++;
        inode = atoi(pstr);
        if( inode != pinfo->inode )
	    continue;             

        ret = -1;
        break; 
         
    }
    closedir(dir);  
    
    if( -1 == ret)
    {
        if( (pstr = strrchr(name,'/')) )
	{
	     //printf("********* %s",pstr);	
	     pstr++;
	     pinfo->pid = atoi(pstr);
	}	
        sprintf(pathbuff,"%s/exe",name);
        if(readlink(pathbuff,pinfo->procpath,MAX_PATH_LEN) < 0)
	    goto done;
    }

done:
    free(pathbuff);
    return ret;
}

static int parse_proc_info_by_inode(NetProcInfo *pinfo)
{
    return general_find_proc_pid(deal_proc_pid_info,(void*)pinfo);
}


static int parse_net_field(char **field,NetProcInfo *pinfo)
{
    char *ptmp;
    unsigned long tmpvalue;
    if(NULL == field || NULL == pinfo)
        return -1;
    
    if( NULL != field[NET_FIELD_BUCKET] ) 
        pinfo->bucket = strtoul(field[NET_FIELD_BUCKET],NULL,16);
    
    if( NULL != field[NET_FIELD_SRCADDR] )
    {
        ptmp = strchr(field[NET_FIELD_SRCADDR],':');
        if(NULL != ptmp)
	{
           *ptmp = '\0';
            ptmp++;
            tmpvalue = strtoul(ptmp,NULL,16);
            pinfo->srcport = (unsigned int)tmpvalue;	
	}
                
        tmpvalue = strtoul(field[NET_FIELD_SRCADDR],NULL,16);
        inet_ntop(AF_INET,&tmpvalue,pinfo->srcaddr,MAX_IPV4_STRLEN);
    } 
    
    if( NULL != field[NET_FIELD_DSTADDR] )
    {
        ptmp = strchr(field[NET_FIELD_DSTADDR],':');
        if(NULL != ptmp)
	{
           *ptmp = '\0';
            ptmp++;
            tmpvalue = strtoul(ptmp,NULL,16);
            pinfo->dstport = (unsigned int)tmpvalue;   
	}
               
        tmpvalue = strtoul(field[NET_FIELD_DSTADDR],NULL,16);
        inet_ntop(AF_INET,&tmpvalue,pinfo->dstaddr,MAX_IPV4_STRLEN);
    }
    
    if(NULL != field[NET_FIELD_STATE])
    {
        pinfo->connstate = strtoul(field[NET_FIELD_STATE],NULL,16);
    }
    
    if(NULL != field[NET_FIELD_SENDRECVBYTES] )
    {
        ptmp = strchr(field[NET_FIELD_SENDRECVBYTES],':');
        if(NULL != ptmp)
	{
           *ptmp = '\0';	   
            ptmp++;
            pinfo->recvbytes = strtoul(ptmp,NULL,16);
	}
                
        pinfo->sendbytes = strtoul(field[NET_FIELD_SENDRECVBYTES],NULL,16);
        
    }

    if( NULL != field[NET_FIELD_UID] )
    {
        pinfo->uid = atoi(field[NET_FIELD_UID]);
    }
    
    if(NULL != field[NET_FIELD_INODE] )
    {
        pinfo->inode = atoi(field[NET_FIELD_INODE]);
    }
    
    if( NULL != field[NET_FIELD_SKREFCNT] )
    {
        pinfo->skrefcnt = atoi(field[NET_FIELD_SKREFCNT] );
    }

    if( NULL != field[NET_FIELD_SOCKADDR] )
    {
        pinfo->sockaddr = strtoul(field[NET_FIELD_SOCKADDR],NULL,16);
    }

    return parse_proc_info_by_inode(pinfo);
     
}



int com_foreach_net_info(netproc_info_cb callback, unsigned int nettype,void*userarg) /*udp or tcp*/
{
   FILE *fp = NULL; 
   char tmpbuff[1024]={0};
   NetProcInfo  procnetinfo={0};
   char *pbuff = NULL;
   char *field[NET_FIELD_MAX]={0};  

   if(nettype > NETTYPE_UDP) 
       return -1;
    
   if(NETTYPE_TCP == nettype)
       fp = fopen(NETTCP_FILE,"r");
   else if(NETTYPE_UDP == nettype)
       fp = fopen(NETUDP_FILE,"r");
   else
       return -1;

   if(NULL == fp)
       return -1;
    
   if( NULL == fgets(tmpbuff,1024,fp) )
       return -1;
    
    memset(tmpbuff,0,1024);
    while((pbuff = fgets(tmpbuff,1024,fp)))
    {
        if( 0 == general_get_field(tmpbuff,' ',NET_FIELD_MAX,field) )
        {
            if( 0 == parse_net_field(field,&procnetinfo))
                if(0 != callback(&procnetinfo,userarg))
		       break;	
        }
        
        memset(tmpbuff,0,1024);  
    }           


   fclose(fp);
}

int com_foreach_local_ipv4(netipv4_cb singleip,void *arg)
{
    int ret = 0;
    char ipbuff[128]={0};
    struct ifaddrs *tmpif,*ifaddr=NULL; 
    void *ipaddr = NULL;  

    if( -1 == getifaddrs(&ifaddr) )
       return -1;

    tmpif = ifaddr;
    
    while(tmpif)
    {
        memset(ipbuff,0,128);  
        if( AF_INET == tmpif->ifa_addr->sa_family )
        {
            ipaddr = &((struct sockaddr_in *)tmpif->ifa_addr)->sin_addr;
            if( inet_ntop(AF_INET, ipaddr, ipbuff, INET_ADDRSTRLEN) )
            {
                ret = singleip(arg,ipbuff);
                if( COMM_STOP_IPGET == ret )
                {
                    ret = 0;   
                    goto out;
                }
                else if( COMM_CONTINUE_IPGET != ret )
                {
                    goto out;
                }
            } 
        }
        tmpif=tmpif->ifa_next;
    }   
out: 
    if(NULL != ifaddr)
        freeifaddrs(ifaddr);

    return ret;
}


int com_is_local_ipv4(const char *ipv4addr)
{
    int ret = 0;
    char ipbuff[128]={0};
    struct ifaddrs *tmpif,*ifaddr=NULL;
    void *ipaddr = NULL;

    if( NULL == ipv4addr || ipv4addr[0] == '\0' )
        return 0;

    if( -1 == getifaddrs(&ifaddr) )
       return 0;

    tmpif = ifaddr;
    while(tmpif)
    {
        memset(ipbuff,0,128);
        if( AF_INET == tmpif->ifa_addr->sa_family )
        {
            ipaddr = &((struct sockaddr_in *)tmpif->ifa_addr)->sin_addr;
            if( inet_ntop(AF_INET, ipaddr, ipbuff, INET_ADDRSTRLEN) )
            {
                if( !strncmp(ipbuff,ipv4addr,strlen(ipbuff)) ) 
                {
                    ret = 1;
                    break;   
                }  
            }
        }
        tmpif=tmpif->ifa_next;
    }

    if( NULL != ifaddr )
        freeifaddrs(ifaddr);

    return ret;
}


static int fcp_foreach_comm_arp(struct fcp_one_item *entry,int arpflag)
{
    FILE *fp = NULL;
    char buff[1024]={0};
    char strflag[20]={0};
    char strtype[20]={0};
    char strmask[16]={0};
    int flag;

    if(NULL == entry)
    {
        return -1;
    }

    switch(entry->flag)
    {
    case LOOP_TYPE_START:
        fp = fopen(PROCARPPATH,"r");
        if(NULL == fp)
        {
            printf("Open %s error\n",PROCARPPATH);
            return -1;
        }
        entry->this = (void*)fp;
        entry->flag = LOOP_TYPE_CON;
        if(NULL == fgets(buff,1024,fp))
            goto done;

        break;
    case LOOP_TYPE_CON:
        fp = (FILE *)entry->this;
        if(NULL == fp)
            goto done;
        break;
    case LOOP_TYPE_STOP:
    default:
        fp = (FILE *)entry->this;
        goto done;
    }
next:
    memset(buff,0,1024);
    if( NULL == fgets(buff,1024,fp) )
        goto done;

    memset(entry->arp_ipv4,0,sizeof(entry->arp_ipv4));
    memset(entry->arp_hwaddr,0,sizeof(entry->arp_hwaddr));
    memset(entry->arp_devname,0,sizeof(entry->arp_devname));
    sscanf(buff,"%s%s%s%s%s%s",entry->arp_ipv4,strtype,strflag,entry->arp_hwaddr,strmask,entry->arp_devname);
    flag = strtol(strflag,NULL,16);

    if( !(flag & arpflag))
        goto next;

    entry->arp_hwtype = strtol(strtype,NULL,16);
    entry->arp_flag = flag;

    return 0;

done:
    if(NULL != fp)
        fclose(fp);

    return -1;
}


int fcp_foreach_permit_arp(struct fcp_one_item *entry)
{
    return fcp_foreach_comm_arp(entry,0x04);
}

int fcp_foreach_complete_arp(struct fcp_one_item *entry)
{
    return fcp_foreach_comm_arp(entry,0x02);
}

int fcp_foreach_publish_arp(struct fcp_one_item *entry)
{
    return fcp_foreach_comm_arp(entry,0x08);
}


static int fcp_set_pingv4_state(int flag)
{
   FILE *fp;
   int ret = 0;
   char buff[10]={0};

   fp = fopen(PING_DEN_PATH,"w");
   if(NULL == fp)
   {
       print_err("Open file [%s] error\n",PING_DEN_PATH);
       return -1;
   }

   sprintf(buff,"%d",flag);
   if( fwrite(buff,1,1,fp) != 1 )
   {
       print_err("Write file[%s] error\n",PING_DEN_PATH);
       ret = -1;
   }

   fclose(fp);

   return ret;
}


int fcp_forbid_pingv4(void)
{
    return fcp_set_pingv4_state(1);
}

int fcp_allow_pingv4(void)
{
    return fcp_set_pingv4_state(0);
}

int fcp_is_allow_pingv4(void)
{
   FILE *fp;
   int ret = 0;
   char buff[10]={0};

   fp = fopen(PING_DEN_PATH,"r");
   if(NULL == fp)
   {
       print_err("Open file [%s] error\n",PING_DEN_PATH);
       return -1;
   }

   if(fread(buff,1,10,fp) < 1 )
   {
       fclose(fp);
       return -1;
   }
   ret = atoi(buff);

   fclose(fp);
   return !(ret);
}


