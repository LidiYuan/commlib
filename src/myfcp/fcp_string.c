#include <stdio.h>
#include <stdlib.h>
#include "fcp_string.h"
#include "genstring.h"


char *fcp_left_strim(char *str)
{
    return gen_left_strim(str);
}

char *fcp_right_strim(char *str)
{
    return gen_right_strim(str);
}


char *fcp_strim(char *str)
{
    return gen_strim(str);
}
