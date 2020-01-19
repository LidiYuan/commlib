#include <stdio.h>
#include <stdlib.h>

#include "fcp_base.h"
#include "genlog.h"


void fcp_set_log_cb(genlog_cb errcb, genlog_cb debugcb)
{
    return genlog_set_cb(errcb, debugcb);
}

