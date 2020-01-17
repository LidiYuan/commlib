#ifndef _COMMPROC_H_
#define _COMMPROC_H_


typedef int (*procpid_cb)(const char *name,void *usrarg);

extern int com_find_proc_pid(procpid_cb callback,void *userarg);
extern int process_cpu_mtime(unsigned int pid);
extern int is_kernel_thread(unsigned int pid);
extern long process_max_number(void);
extern int process_cmdline(unsigned int pid,char *linebuff,unsigned int size);


#endif
