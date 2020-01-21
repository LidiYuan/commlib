#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myfcp.h>


int main(int argc,char *argv[])
{
    struct fcp_one_item entry={0};   
    char buff[100]={"    dasdsa  "};

    printf("#%s#\n\n",fcp_strim(buff));

    foreach_line_field("  sa da 222  s33",' ',&entry)
    {
        printf("#%s#\n",entry.data);
        if( !strcmp(entry.data,"222") )  
        {
            entry.flag = LOOP_TYPE_STOP; /*设置此标志将结束循环*/
            continue;
        }
    }


    return 0;
}
