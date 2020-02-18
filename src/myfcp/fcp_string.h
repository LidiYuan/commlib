#ifndef _FCP_STRING_H_
#define _FCP_STRING_H_

#include <stdio.h>
#include <string.h>
#include "fcp_base.h"

    
extern char *fcp_left_strim(char *str);
extern char *fcp_right_strim(char *str);
extern char *fcp_strim(char *str);


extern int fcp_foreach_field(const char *buff,char keys,struct fcp_one_item *entry);

#define foreach_line_field(linebuff,keys,entry) \
        memset(entry,0,sizeof(struct fcp_one_item)); \
        while( !fcp_foreach_field((linebuff), (keys), (entry)) )

/*need user to free result*/
extern char *fcp_base64_encode(const char *str);

/*need user to free result*/
extern char *fcp_base64_decode(const char *code);

extern int fcp_file_md5(const char *file_path, char *md5_str);
extern int fcp_string_md5(unsigned char *str, unsigned int str_len, char *md5_str);

#endif
