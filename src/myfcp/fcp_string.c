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


int fcp_foreach_field(const char *buff,char keys,struct fcp_one_item *entry)
{
    return gen_foreach_field(buff,keys,entry);
}

char *fcp_base64_decode(const char *str)
{
    return gen_base64_decode(str);
}

char *fcp_base64_encode(const char *str)
{
    return gen_base64_encode(str);
}

int fcp_file_md5(const char *file_path, char *md5_str)
{
    return gen_file_md5(file_path,md5_str);
}

int fcp_string_md5(unsigned char *str, unsigned int str_len, char *md5_str)
{
    return gen_string_md5(str,str_len,md5_str);
}

