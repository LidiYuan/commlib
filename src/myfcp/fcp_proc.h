#ifndef _FCP_PROC_H_
#define _FCP_PROC_H_



enum task_state{
    TASK_STAT_RUNNIG = 0,             // R		
    TASK_STAT_INTERRUPTIBLE,  	      // S	
    TASK_STAT_UNINTERRUPTIBLE,        // D
    TASK_STAT_ZOMBIE,                 // Z
    TASK_STAT_STOP,                   // T
    TASK_STAT_TASK_TRACED,            // t
    TASK_STAT_DEAD,                   // X x
    TASK_STAT_UNKNOW       	
};

struct task_proc_stat{
    unsigned int pid;
    char comm[256];
    int state;
    unsigned int ppid;
    unsigned int pgid;
    unsigned int session;
    int tty_nr_major;
    int tty_nr_minor;
    int tpgid;
    int flags;  // task->flgas PF_KTHREAD
};


typedef int (*procpid_cb)(const char *name,void *usrarg);

extern int com_find_proc_pid(procpid_cb callback,void *userarg);
extern int process_cpu_mtime(unsigned int pid);
extern int taskutil_kernel_task(unsigned int pid);
extern long process_max_number(void);
extern int process_cmdline(unsigned int pid,char *linebuff,unsigned int size);
extern int taskutil_task_proc_stat(unsigned int pid, struct task_proc_stat *pstat);
extern void taskutil_task_to_daemon(int nochdir,int nofileclose,int nostdclose,int nochildign);

#endif
