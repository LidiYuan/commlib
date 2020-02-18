#ifndef _GEN_STRING_H_
#define _GEN_STRING_H_

#include "fcp_base.h"

extern char *gen_left_strim(char *str);
extern char *gen_right_strim(char *str);
extern char *gen_strim(char *str);


#define gen_isspace(c)  ((c) == ' ' || (c) == '\t')


extern int gen_foreach_field(const char *buff,char keys,struct fcp_one_item *entry);

#define gen_foreach_line_field(linebuff,keys,entry) \
        memset(entry,0,sizeof(struct fcp_one_item)); \
        while( !gen_foreach_field((linebuff), (keys), (entry)) )


/* need user to free result*/
extern char *gen_base64_decode(const char *code);
extern char *gen_base64_encode(const char *code);

extern int gen_string_md5(unsigned char *str, unsigned int str_len, char *md5_str);
extern int gen_file_md5(const char *file_path, char *md5_str);


#endif
