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



/* need user to free result*/
char *gen_base64_encode(const char *str)
{
    long len;
    long str_len;
    unsigned char *res;
    int i,j;
    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    str_len=strlen(str);
    len = (str_len %3 !=0 ) ? ((str_len/3+1)*4) : (str_len/3*4);

    res=(char*)malloc(sizeof(unsigned char)*len+1);
    if(NULL == res)
        return NULL;
    memset(res,0,sizeof(char) *len  + 1);

    for(i=0,j=0;i<len-2;j+=3,i+=4)
    {
        res[i]=base64_table[str[j]>>2];
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)];
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)];
        res[i+3]=base64_table[str[j+2]&0x3f];
    }

    switch(str_len % 3)
    {
        case 1:
            res[i-2]='=';
            res[i-1]='=';
            break;
        case 2:
            res[i-1]='=';
            break;
    }

    return res;
}


/* need user to free result*/
char *gen_base64_decode(const char *code)
{
    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,62,0,0,0,
                 63,52,53,54,55,56,57,58,
                 59,60,61,0,0,0,0,0,0,0,0,
                 1,2,3,4,5,6,7,8,9,10,11,12,
                 13,14,15,16,17,18,19,20,21,
                 22,23,24,25,0,0,0,0,0,0,26,
                 27,28,29,30,31,32,33,34,35,
                 36,37,38,39,40,41,42,43,44,
                 45,46,47,48,49,50,51
               };
    long len;
    long str_len;
    unsigned char *res;
    int i,j;

    len=strlen(code);

    if(strstr(code,"=="))
    {
        str_len=len/4*3-2;
    }
    else if(strstr(code,"="))
    {
        str_len=len/4*3-1;
    }
    else
    {
        str_len=len/4*3;
    }

    res=(char*)malloc(sizeof(char)*str_len+1);
    if(NULL == res)
        return NULL;

    memset(res,0,sizeof(char)*str_len+1);

    for(i=0,j=0;i < len-2;j+=3,i+=4)
    {
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4);
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2);
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]);
    }

    return res;
}



