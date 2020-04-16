#ifndef __FCP_MM_H_
#define __FCP_MM_H_


int mmutil_realloc(void **ptr,size_t size);
int mmutil_recalloc(void **ptr,size_t oldsize, size_t size);

#endif
