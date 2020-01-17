#include <stdio.h>
#include "general.h"
#include "commfile.h"
#include "genlog.h"

int comm_foreach_dir_entry(const char *path,struct file_item *entry)
{
    return general_foreach_dir_entry(path,entry);
}


int comm_foreach_regfile_entry(const char *path,struct file_item *entry)
{ 
    return general_foreach_regfile_entry(path,entry);
}



