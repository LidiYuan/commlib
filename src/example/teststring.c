#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>


int main(int argc,char *argv[])
{
    char buff[100]={"    dasdsa  "};

    printf("#%s#\n",fcp_strim(buff));


    return 0;
}
