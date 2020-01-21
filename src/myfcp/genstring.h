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


#endif
