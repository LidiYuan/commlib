#include <stdio.h>
#include <stdlib.h>
#include "commfun.h"


int callback(const char *fpath,void *userarg)
{
    printf(" %s\n",fpath);
        
    return 0; // -1   stop list
}

int main(int argc,char *argv[])
{
    if( comm_list_all_procpid(callback,NULL) !=0 )
       printf("list error\n");

    return 0;
}
