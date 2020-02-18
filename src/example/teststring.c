#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myfcp.h>


int main(int argc,char *argv[])
{
    struct fcp_one_item entry={0};   
    char buff[100]={"    dasdsa  "};
    char *tmpstr1;
    char *tmpstr2;

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

    tmpstr1 = fcp_base64_encode("hello world");
    if(NULL != tmpstr1)
    {
	printf("base64 encord[hello world]==>%s\n",tmpstr1);
        tmpstr2 = fcp_base64_decode(tmpstr1);
        if(NULL != tmpstr2)
	{
	    printf("base64 decode [%s]==>%s\n",tmpstr1,tmpstr2);
	    free(tmpstr2); 
	}
	
	free(tmpstr1);
    }


    return 0;
}
