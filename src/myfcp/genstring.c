#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "genstring.h"

char *gen_left_strim(char *str)
{
    while (isspace(*str))
		++str;

	return (char *)str;
}

char *gen_right_strim(char *str)
{
    size_t size;
    char *pend = NULL;

    if( !(size=strlen(str) ))
        return str;

    pend = str+size-1;
    while(pend>=str && isspace(*pend))
        pend--;
    *(pend+1) = '\0';

    return str;
}


char *gen_strim(char *str)
{
	return gen_left_strim(gen_right_strim(str));
}


