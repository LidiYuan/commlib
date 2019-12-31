#include <stdio.h>
#include <stdlib.h>
#include "gencompo4c.h"


int callback(const char *fpath,void *userarg)
{
    printf(" %s\n",fpath);
        
    return 0; // -1   stop list
}

int main(int argc,char *argv[])
{
    if( com_find_proc_pid(callback,NULL) != RET_SUCCESS )
       printf("list error\n");

    return 0;
}
