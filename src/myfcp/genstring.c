#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "genstring.h"

struct gen_field_value
{
    char *strbuff;
    char *ptr;
};


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


int gen_foreach_field(const char *buff,char keys,struct fcp_one_item *entry)
{
    struct gen_field_value  *node = NULL;
    char *end = NULL;

    if(NULL == buff || 0 == strlen(buff) || NULL == entry || entry->flag >= (unsigned int)LOOP_TYPE_MAX)
    {
        return -1;
    }

    switch(entry->flag)
    {
    case LOOP_TYPE_START:
        node = (struct gen_field_value*)malloc(sizeof(struct gen_field_value));
        if(NULL == node)
        {
            return -1;
        }
        memset(node,0,sizeof(struct gen_field_value));
        node->strbuff = (char *)malloc(strlen(buff)+10);
        if(NULL == node->strbuff)
        {
            free(node);
            return -1;
        }
        memset(node->strbuff,0,strlen(buff)+10);
        memcpy(node->strbuff,buff,strlen(buff));
        if( '\n' ==  node->strbuff[strlen(node->strbuff) -1 ] )
            node->strbuff[strlen(node->strbuff) -1 ] = '\0';

        node->ptr = node->strbuff;
        entry->this = (void*)node;
        entry->flag = (unsigned int)LOOP_TYPE_CON;

        break;

    case LOOP_TYPE_CON:
	 if(NULL == entry->this)
            goto done;
        node = (struct gen_field_value *)entry->this;
        break;

    case LOOP_TYPE_STOP:
    default:
        node = (struct gen_field_value *)entry->this;
        goto done;
        break;
    }

    while(gen_isspace( *(node->ptr) ) || *(node->ptr) == keys)
        node->ptr++;

    end = node->ptr;
    while('\0' != (*end) && (*end) != keys)
    {
        end++;
    }

    if(node->ptr >= end)
        goto done;

    if(entry->data)
    {
        free(entry->data);
        entry->data = NULL;
    }

    entry->data = (char *)malloc(end - node->ptr+10);
    if(NULL == entry->data)
        goto done;
	 memset(entry->data,0, (end - node->ptr)+10);
    memcpy(entry->data,node->ptr,(end - node->ptr));
    node->ptr = end;

    return 0;

done:
    if(node && node->strbuff)
    {
        free(node->strbuff);
    }
    if(node)
        free(node);

    entry->this = NULL;
    if(entry->data)
        free(entry->data);

    return -1;

}




