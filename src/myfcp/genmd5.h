#ifndef __GEN_MD5_H_
#define __GEN_MD5_H_

#define MD5_SIZE         16
#define MD5_STR_LEN       (MD5_SIZE * 2)

struct md5_ctx
{
        unsigned int count[2];
        unsigned int state[4];
        unsigned char buffer[64];
};


extern void md5_init(struct md5_ctx *context);
extern void md5_update(struct md5_ctx *context, unsigned char *input, unsigned int inputlen);
extern void md5_final(struct md5_ctx *context, unsigned char digest[16]);



#endif
