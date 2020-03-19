#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include "fcp_base.h"
#include "genlog.h"


void fcp_set_log_cb(genlog_cb errcb, genlog_cb debugcb)
{
    return genlog_set_cb(errcb, debugcb);
}


int program_is_running(const char *path)
{
    int fd;
    int ret;
    const char *tmppath = "/tmp/.procforbid";

    if(NULL != path)
        tmppath = path;

    fd = open(tmppath,O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0)
    {
        return 1;
    }

    ret = flock(fd, LOCK_EX | LOCK_NB);
    if(ret < 0)
    {
        close(fd);
        return 1;
    }


    return 0;
}

