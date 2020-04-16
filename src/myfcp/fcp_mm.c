#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "fcp_mm.h"
#include "genmm.h"

int mmutil_realloc(void **ptr,size_t size)
{
    return gen_mm_realloc(ptr,size);
}


int mmutil_recalloc(void **ptr,size_t oldsize, size_t size)
{
    return gen_mm_recalloc(ptr,oldsize,size);
}






