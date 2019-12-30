#include <stdio.h>
#include <stdlib.h>
#include "gencompo4c.h"


static int iploopcallback(void *arg, const char *ipaddr)
{
    printf("%s\n",ipaddr);	
    return COMM_CONTINUE_IPGET;
}

int main(int argc,char *argv[])
{

#if 1	
    //get local ipv4 test1	
    if( 0 != com_foreach_local_ipv4(iploopcallback,NULL) )
    {
        printf("get ip addr is error\n");
	return -1;
    }
#endif

#if 1    
    if( com_is_local_ipv4("192.168.129.134") )
    {
        printf("ip is local ipv4 addr\n");
    }
    else
    {
        printf("ip is not local ipv4 addr\n");
    }
#endif



    
    return 0;
}
