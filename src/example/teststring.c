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
    char md5sum[256]={0}; 

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

    if(0 == fcp_string_md5("hello",strlen("hello"),md5sum))
    {
        printf("string md5 [hello]=>%s\n",md5sum);
    }
    if(0 == fcp_file_md5("/etc/passwd",md5sum))
    {
        printf("file md5 [/etc/passwd]=>%s\n",md5sum);
    }

    if(NULL != (tmpstr1 = strutils_path_last_component("/te/da/sss///")) )
    {
        printf("path[ /te/da/sss/// ]last component is %s\n",tmpstr1);
    }
    

    return 0;
}
