#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <linux/elf.h>
#include <fcntl.h>

#include "general.h"
#include "fcp_file.h"
#include "genlog.h"


int comm_foreach_dir_entry(const char *path,struct file_item *entry)
{
    return general_foreach_dir_entry(path,entry);
}

int comm_foreach_regfile_entry(const char *path,struct file_item *entry)
{ 
    return general_foreach_regfile_entry(path,entry);
}


static int judge_file_type(const char *path, int type)
{
    struct stat st;
        
    if(stat(path,&st) != 0)
        return 0;
    if(st.st_mode & S_IFMT == type)
        return 1;
    
    return 0;
}

int file_is_dir(const char *path)
{
    return judge_file_type(path,S_IFDIR);
}

int file_is_reg(const char *path)
{
    return judge_file_type(path,S_IFREG);
}

int file_is_lnk(const char *path)
{
    return judge_file_type(path,S_IFLNK);
}

int file_is_blk(const char *path)
{
    return judge_file_type(path,S_IFBLK);
}

int file_is_chr(const char *path)
{
    return judge_file_type(path,S_IFCHR);
}

int file_is_fifo(const char *path)
{
    return judge_file_type(path,S_IFIFO);
}

int file_is_sock(const char *path)
{
    return judge_file_type(path,S_IFSOCK);
}



int file_execute_type(const char *path)
{
    int fd;
    char buff[256]={0};
    ssize_t ret = 0;
    struct elf32_hdr *pe32hdr=NULL;
    struct stat st;
    
    if( file_is_reg(path) && !file_is_lnk(path))
        return EXEC_TYPE_NOTEXEC;

    if( stat(path,&st) || st.st_size == 0)
        return EXEC_TYPE_NOTEXEC;

    if( (fd = open(path,O_RDONLY)) <=0 )
        return EXEC_TYPE_NOTEXEC;

    ret = read( fd,buff,sizeof(struct elf32_hdr) );
    if( ret == sizeof(struct elf32_hdr) )
    {
        if(memcmp(buff, "\177ELF", sizeof("\177ELF")-1) == 0)
        {
            pe32hdr = (struct elf32_hdr*)buff;
            if( ET_EXEC == pe32hdr->e_type )
                return EXEC_TYPE_ELF_EXC;

            if( ET_DYN == pe32hdr->e_type )
                return EXEC_TYPE_ELF_DYN;

            if( ET_REL == pe32hdr->e_type )
                return EXEC_TYPE_ELF_REL;

            if( ET_CORE == pe32hdr->e_type)
                return EXEC_TYPE_ELF_CORE;
        }
    }

    if(!strncmp(buff,"#!/bin/bash",strlen("#!/bin/bash")) || !strncmp(buff,"#!/bin/sh",strlen("#!/bin/sh")))
	   return EXEC_TYPE_SHELL_SCRPT;

    if(!strncmp(buff,"#!/bin/python",strlen("#!/bin/python")))
        return EXEC_TYPE_PYTHON_SCRPT;

    if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
        return EXEC_TYPE_SUSPICIOUS_SCRPT;

    return EXEC_TYPE_NOTEXEC;    
}




