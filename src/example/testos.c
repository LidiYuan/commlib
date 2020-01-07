#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gencompo4c.h"


int main(int argc,char *argv[])
{
    char buff[256]={0};

    if(os_info_uuid(buff,256) == 0)
    {
        printf("uuid:%s\n",buff);
    }
    memset(buff,0,256);
    if(os_info_bootid(buff,256) == 0)
    {
        printf("bootid:%s\n",buff);
    }


    return 0;
}
