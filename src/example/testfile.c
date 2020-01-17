#include <stdio.h>
#include <stdlib.h>
#include <myfcp.h>



int main(int argc,char *argv[])
{
    struct file_item ent={0};

    foreach_dir_entry("/proc", &ent)
    {
        printf("%s\n",ent.fullpath);
        
        /*
            若想提前结束循环  可以将ent.flag设置成LOOP_TYPE_STOP 然后在调用foreach_dir_entry("/proc", &ent) 如下:
            if(you want stop loop)
            {
                ent.flag = LOOP_TYPE_STOP;
                continue;    //当再次调用foreach_dir_entry的时候发现flag为STOP则退出循环
            }        
        */
    }

    
    


    return 0;
}
