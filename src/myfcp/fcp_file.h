#ifndef _FCP_FILE_H_
#define _FCP_FILE_H_
#include <string.h>
#include "fcp_base.h"

extern int comm_foreach_dir_entry(const char *path,struct fcp_one_item *entry);
extern int comm_foreach_regfile_entry(const char *path,struct fcp_one_item *entry);


/*entry is struct file_item{} ptr*/
#define foreach_dir_entry(dirpath, entry)   \
        memset(entry,0,sizeof(struct fcp_one_item)); \
        while( comm_foreach_dir_entry(dirpath,entry) == 0 )


/*entry is struct file_item{} ptr*/
#define foreach_regfile_entry(dirpath, entry) \
        memset(entry,0,sizeof(struct fcp_one_item));  \
        while( comm_foreach_regfile_entry(dirpath,entry) == 0 )



enum{
    EXEC_TYPE_NOTEXEC=0,        /*文件不可执行*/
    EXEC_TYPE_ELF_EXC,          /*可执行ELF*/
    EXEC_TYPE_ELF_DYN,          /*动态库*/
    EXEC_TYPE_ELF_REL,          /*可重定向文件*/
    EXEC_TYPE_ELF_CORE,         /*core 文件*/ 
    EXEC_TYPE_SHELL_SCRPT,      /*是shell脚本*/
    EXEC_TYPE_PYTHON_SCRPT,     /*python脚本*/
    EXEC_TYPE_SUSPICIOUS_SCRPT, /*有执行权限 但没有写脚本头如(没写#!/bin/bash)*/
    EXEC_TYPE_OTHER_SCRPT,      /*暂未识别的脚本*/
    EXEC_TYPE_MAX
};


extern int file_is_dir(const char *path);
extern int file_is_reg(const char *path);
extern int file_is_lnk(const char *path);
extern int file_is_blk(const char *path);
extern int file_is_chr(const char *path);
extern int file_is_fifo(const char *path);
extern int file_is_sock(const char *path);

extern int file_execute_type(const char *path); /*返回类型如EXEC_TYPE_xxx*/
extern int fileutil_open_on_closeexec(const char *pathname, int flags, int mode);


#endif
