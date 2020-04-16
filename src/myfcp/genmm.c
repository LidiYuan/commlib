#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


int gen_mm_realloc(void **ptr,size_t size)
{
    void *newptr = NULL;

    newptr = realloc(*ptr,size);
    if(NULL == newptr)
       return -1;

    *ptr = newptr;

    return 0;
}


int gen_mm_recalloc(void **ptr,size_t oldsize, size_t size)
{
    void *newptr = NULL;
    size_t newsize;

    newptr = realloc(*ptr,size);
    if(NULL == newptr)
        return -1;

    *ptr = newptr;
    newsize = malloc_usable_size(*ptr);

    if(newsize < size)
        newsize = size;

    if( newsize > oldsize)
    {
        memset((char*)(*ptr)+oldsize,0, newsize-oldsize);
    }

    return 0;
}
