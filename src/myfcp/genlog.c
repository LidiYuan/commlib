#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <syslog.h>
#include "genlog.h"

enum{
   LOG_LEVEL_TYPE_ERROR=0,
   LOG_LEVEL_TYPE_DEBUG,
   LOG_LEVEL_TYPE_MAX
};

static void genlog_error_output(const char *msg);
static void genlog_debug_output(const char *msg);


static genlog_cb genlog_cb_arr[LOG_LEVEL_TYPE_MAX] = {
               genlog_error_output,  //LOG_LEVEL_TYPE_ERROR
               genlog_debug_output,  //LOG_LEVEL_TYPE_DEBUG
};



static void genlog_error_output(const char *msg)
{
#ifndef LOG_STDERR        
    openlog("myfcp",LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_ERR,"%s",msg);
    closelog();
#else
    printf("Err: %s\n",msg);
#endif

    return;
}


static void genlog_debug_output(const char *msg)
{

#ifndef LOG_STDERR                
    openlog("myfcp",LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_DEBUG,"%s",msg);
    closelog();
#else
    printf("Debug: %s\n",msg);
#endif    

    return;
}


void genlog_set_cb(genlog_cb errcb, genlog_cb debugcb)
{
    genlog_cb_arr[LOG_LEVEL_TYPE_ERROR] = errcb;
    genlog_cb_arr[LOG_LEVEL_TYPE_DEBUG] = debugcb;
}


void genlog_print(int loglevel, const char *fmt, ...)
{
   va_list ap;
   int n;
   char *buff;

   va_start(ap, fmt);
   n = vsnprintf(NULL, 0, fmt, ap);
   va_end(ap);

   if(n < 0)
   {
       return;
   }

   buff = (char*)malloc(n+1);
   if(NULL == buff)
       return;

   memset(buff,0,n+1);
   va_start(ap, fmt);
   n = vsnprintf(buff, n+1, fmt, ap);
   va_end(ap);

   switch(loglevel)
   {
   case LOG_ERR:
       if(NULL != genlog_cb_arr[LOG_LEVEL_TYPE_ERROR])
           genlog_cb_arr[LOG_LEVEL_TYPE_ERROR](buff);
       break;
   case LOG_DEBUG:
       if(NULL != genlog_cb_arr[LOG_LEVEL_TYPE_DEBUG])
           genlog_cb_arr[LOG_LEVEL_TYPE_DEBUG](buff);
       break;
   default:
       break;

   }
   free(buff);

   return;
}








