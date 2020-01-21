#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>



int main(int argc,char *argv[])
{
    struct fcp_one_item ent={0};
    int type; 

    foreach_dir_entry("/proc", &ent)
    {
        printf("%s\n",ent.data);
        
        /*
            若想提前结束循环  可以将ent.flag设置成LOOP_TYPE_STOP 然后在调用foreach_dir_entry("/proc", &ent) 如下:
            if(you want stop loop)
            {
                ent.flag = LOOP_TYPE_STOP;
                continue;    //当再次调用foreach_dir_entry的时候发现flag为STOP则退出循环
            }        
        */
    }
    
    type = file_execute_type("/bin/bash");
    switch(type)
    {
    case EXEC_TYPE_NOTEXEC:
        printf("  this is not a exec file\n");
        break;
    case EXEC_TYPE_ELF_EXC:
        printf("this is a execute file\n");
        break;
    case EXEC_TYPE_ELF_DYN:
        printf("this is a dynamic lib\n");
        break;
    case EXEC_TYPE_ELF_REL:
        printf("this is a redirection file\n");
        break;
    case EXEC_TYPE_ELF_CORE:
        printf("thi is a core file\n");
        break;
    case EXEC_TYPE_SHELL_SCRPT:
        printf(" this is a shell script\n");
        break;
    case EXEC_TYPE_PYTHON_SCRPT:
        printf(" this is a python script\n");
        break;
    case EXEC_TYPE_SUSPICIOUS_SCRPT:
        printf(" this is suspicious script\n");
        break;
    case EXEC_TYPE_OTHER_SCRPT:
        printf("this is other script\n");
        break;
    }

    return 0;
}
    
