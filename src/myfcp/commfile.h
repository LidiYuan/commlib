#ifndef _COMM_FILE_H_
#define _COMM_FILE_H_
#include <string.h>
#include "basecomm.h"


extern int comm_foreach_dir_entry(const char *path,struct file_item *entry);
extern int comm_foreach_regfile_entry(const char *path,struct file_item *entry);


/*entry is struct file_item{} ptr*/
#define foreach_dir_entry(dirpath, entry)   \
        memset(entry,0,sizeof(struct file_item)); \
        while( comm_foreach_dir_entry(dirpath,entry) == 0 )


/*entry is struct file_item{} ptr*/
#define foreach_regfile_entry(dirpath, entry) \
        memset(entry,0,sizeof(struct file_item));  \
        while( comm_foreach_regfile_entry(dirpath,entry) == 0 )


#endif
