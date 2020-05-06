【一】文件介绍

库名介绍: myfcp (my function componets)

src/myfcp.h 总的头文件，想要使用此库,必须包含此头文件
src/example/testproc.c  一些和进程信息相关的例子
src/example/testnet.c   一些和网络操作相关的例子
src/example/testos.c    一些和系统信息获取相关的例子
src/example/testfile.c  一些和文件操作相关的例子
src/example/acceptcli.c  网络建立 客户端例子
src/example/acceptser.c  网络操作 服务端例子
src/example/teststring.c  字符串操作的例子
src/example/testdaemon.c   使用守护进程的操作例子
src/example/testsignal.c  信号使用的例子

src/myfcp/fcp_proc.c 含有一些和进程信息相关的导出函数
src/myfcp/fcp_net.c  含有一些和网络操作相关的导出函数
src/myfcp/fcp_os.c   含有一些和系统信息获取相关的导出函数
src/myfcp/fcp_file.c   含有一些和文件操作相关的导出函数
src/myfcp/fcp_string.c  含有一些和字符串操作相关的导出函数
src/myfcp/fcp_mm.c     含有一些内存操作相关的导出函数
src/myfcp/fcp_socket.c 含有一些socket 操作
src/myfcp/fcp_signal.c  信号操作相关
src/myfcp/fcp_base.c   一些杂项操作


【二】
	适配版本
		ubuntu18
		centos7
		centos6
		
【三】函数介绍
(1) 网络操作相关
	1) extern int com_foreach_local_ipv4(COMM_SIP_CALLBACK singleip,void *arg);
	功能:
	  遍历本地所有的ipv4
	参数介绍: 
	  COMM_SIP_CALLBACK 是个回调函数原型为typedef int (*COMM_SIP_CALLBACK)(void *arg,const char *ipbuff);
	  arg是用户数据
	返回值
	   成功返回0  失败返回负数 
	使用举例看src/example/testnet.c

	2) extern int com_is_local_ipv4(const char *ipv4addr);
	功能:
	  判断一个ipv4是否为本地ip
	参数介绍:
	   ipv4addr需要判断的ip地址	
	返回值
		成功返回0  失败返回负数
	使用举例看src/example/testnet.c

	3) extern  int com_foreach_net_info(netproc_info_cb callback, unsigned int nettype,void*userarg);
	功能: 
	   遍历当前系统中的网络信息
	参数介绍:
	   callback:没发现一个网络信息 调用此回调函数,原型为typedef int(*netproc_info_cb)(NetProcInfo *pinfo,void *userarg);
	   nettype:网络类型 NETTYPE_TCP 或者 NETTYPE_UDP
	   userarg：用户私有数据
	返回值:
	   成功返回0  失败返回负数 
	使用举例看src/example/testnet.c
	 
	结构介绍:
		typedef struct{
			unsigned int bucket;           //hash桶编号
			char srcaddr[MAX_IPV4_STRLEN]; //ip源地址
			unsigned int srcport;          //ip源端口 
			char dstaddr[MAX_IPV4_STRLEN]; //ip目的地址
			unsigned int dstport;          //ip目的端口
			int connstate;                 //连接状态
			unsigned int sendbytes;        //发送字节数
			unsigned int recvbytes;        //接收字节数
			unsigned int uid;              //用户uid
			unsigned int inode;            //此socket 的inode号
			unsigned int skrefcnt;         //sock的引用计数
			unsigned long sockaddr;        //sock结构在内存中的地址
			int pid;                       //进程号
			char procpath[MAX_PATH_LEN];   //进程路径 
		}NetProcInfo;
	连接状态:
		ESTABLISHED
		SYN_SENT
		SYN_RECV
		FIN_WAIT1
		FIN_WAIT2
		TIME_WAIT
		CLOSE
		CLOSE_WAIT
		LAST_ACK
		LISTEN
		CLOSING
	 
(2) 进程操作相关
	1)extern int com_find_proc_pid(procpid_cb callback,void *userarg);
	功能:
		遍历/proc下的所有pid
	参数介绍:
		callback:每获得一个pid就会回调此函数 原型,typedef int (*procpid_cb)(const char *name,void *usrarg);
		userarg:用户私有数据
	返回值:
		成功返回0 失败返回负数
	使用举例看src/example/testproc.c	
	
	2)extern int process_cpu_mtime(unsigned int pid);
    功能:
		获得某个进程占用cpu的时间(包括其线程),单位毫秒
	参数:
		pid  进程的pid
	返回值:
		失败返回-1, 成功返回占用cpu的总时间(ms)
	使用举例看src/example/testproc.c
	
	3)extern int is_kernel_thread(unsigned int pid);
	功能:
		判断给定进程pid是否为内核线程
	参数:
		pid  进程的pid
	返回值:
		是内核线程返回1,否则返回0	
	使用举例看src/example/testproc.c
	
    4)extern long process_max_number(void);
	功能:
		获得系统上允许运行的最大进程数
	参数:
		无
	返回值:
		失败返回-1,成功返回进程最大数量		
	使用举例看src/example/testproc.c
	
	5)extern int process_cmdline(unsigned int pid,char *linebuff,unsigned int size);
		功能:
			获得某个进程的命令行
		参数:
			pid:进程pid
			linebuff:返回进程的命令行
			size: 缓存的大小
		返回值:
			失败返回-1,成功返回0		
		使用举例看src/example/testproc.c
    
	6) int taskutil_kill_task(pid_t pid)
		功能: 根据pid杀掉一个进程
		参数:
			   pid  进程的pid
		返回值:
			   成功返回0， 失败返回-1	
    
	7) int taskutil_elf_type(const char *filename)
	   功能: 获得elf文件的类型 
       参数:
             filename 文件路径
       返回值: 如下类型值
            ELF_TYPE_ERROR,  获取失败
			ELF_TYPE_EXE,    可执行文件
			ELF_TYPE_DYN,    动态库  
			ELF_TYPE_CORE,   core文件
			ELF_TYPE_REL,     重定向文件
			ELF_TYPE_OTHER,	  未识别  
    8) int taskutil_current_session(void)
	   功能: 获得当前的会话id
       参数:无
       返回值: 成功返回sessid 失败返回-1	   

(3) OS信息获取相关
	1) extern int os_info_uuid(char *buff,unsigned int bufsize);
	功能:
		获得系统的uuid
	参数介绍:
		 buff:返回uuid信息
		 bufsize:缓存的大小
	返回值:
		 成功返回 0 失败返回负值		 
	使用举例:
		src/example/testos.c

	2) extern int os_info_bootid(char *buff,unsigned int bufsize);
	功能:
		获得系统的bootid
	参数介绍:
		 buff:返回bootid信息
		 bufsize:缓存的大小
	返回值:
		 成功返回 0 失败返回负值
	使用举例:
		src/example/testos.c

	3) extern time_t os_info_boot_time(void)
	功能:
		获得系统启动时间
	参数:
		无
	返回值:
		系统启动的时间戳,若失败返回-1,  可以用date -d @时间戳 看时间
	使用举例:
		src/example/testos.c
		
	4) extern time_t os_info_last_shutdow_time(void)
	功能:
		获得系统上次关机的时间,有可能会获得失败,当/var/log/wtmp文件过大的时间,会将此信息冲刷掉
	参数:
		无
	返回值:
		成功返回上次管家的时间戳, 失败返回-1.
	使用举例:
		src/example/testos.c
		
	5) extern int os_info_running_tty(login_info_cb callback,void *userarg)
	功能:
		获得当前已经登录并运行着的终端信息, 每获得一个,就会调用一次callback,用户可以在callback中对每条登录信息进行处理
	参数:
		callback 用户提供的回调函数,用于接收终端登录信息,原型为typedef int (*login_info_cb)(struct login_info *uinfo,void *userarg);
			#define MAX_USER_NAME 128
			#define MAX_TTY_LEN  128
			#define MAX_HOST_NAME 256
			struct login_info
			{
				uid_t pid;                    //进程pid
				char username[MAX_USER_NAME]; //登录的用户名
				time_t logintime;             //登录的时间
				char tty[MAX_TTY_LEN];        //终端名 如pts/1
				char hostname[MAX_HOST_NAME]; //登录的远端host名,如远端ip 
				time_t  logouttime;           //终端登出的时间为-1的话,表示一直在运行中
			}; 	
		userarg: 用户自己的私有数据
	返回值:
		成功返回 0,失败返回-1
	使用举例:
		src/example/testos.c
		
	6) extern int os_info_logout_tty(login_info_cb callback, void *userarg)	
	功能:
		获得当前已经登出的终端信息,获得的个数要看/var/log/wtmp文件中存储的个数
	参数:	
		callback 用户提供的回调函数,用于接收每条终端登出信息,原型为typedef int (*login_info_cb)(struct login_info *uinfo,void *userarg);
		userarg: 用户自己的私有数据
	返回值:
		成功返回 0,失败返回-1
	使用举例:
		src/example/testos.c
		
	7) extern int os_info_version(void);
	功能:
		获得当前系统的版本号 如UBUNTU18 CENTOS7
	参数:	
		无
	返回值:
		成功返回系统类型如下 失败返回-1,对还未支持的类型会返回-1
		 OS_VERSION_UBUNTU18
		 OS_VERSION_CENTOS7
	使用举例:
		src/example/testos.c
	
	7) extern int os_info_machine_id(char*buff, unsigned int size); /*size you can set MAX_MACHINE_ID_LEN*/
	功能:
		获得一个 在安装或首次启动操作系统时生成的、专属于本系统的、独一无二的"machine ID"
	参数:
		buff  返回机器id  机器id现在长度为32位   
		size  建议设置为 MAX_MACHINE_ID_LEN 此值在basecomm.h中定义
	返回值:
		成功返回0  失败返回-1
	使用举例:
		src/example/testos.c
		
(4) 文件操作相关
	1) extern int comm_foreach_dir_entry(const char *path,struct file_item *entry);
	功能: 
		遍历目录path下的所有目录
	参数:
		path: 要遍历的目录
		entry: 返回的参数,使用前必须对entry指向的struct file_item项用memset()进行设置为0
				struct file_item {
						void *this;     //this指针  标志这一次的遍历
						char *fullpath; //返回遍历的子目录的全路径
						int flag;	    // 控制循环的标志,如果希望提前结束循环则将flag设置为LOOP_TYPE_STOP,然后再次调用 comm_foreach_dir_entry()将结束循环 
				};	定义在在basecomm.h中
	返回值: 
		成功返回0  遍历完或失败返回-1
	其对应的宏为 foreach_dir_entry(path,entry)  定义在在commfile.h
	使用举例:
		src/example/testfile.c	
		
    2) extern int comm_foreach_regfile_entry(const char *path,struct file_item *entry);
	功能: 
		遍历目录path下的所有普通文件
	参数:
		参照comm_foreach_dir_entry()
	返回值: 
		成功返回0  遍历完或失败返回-1
	其对应的宏为 foreach_regfile_entry(path,entry)  定义在在commfile.h
	使用举例:
		请看comm_foreach_dir_entry()的使用
	
	3)如下函数 判断文件类型  是返回1  否则返回0
		extern int file_is_dir(const char *path);
		extern int file_is_reg(const char *path);
		extern int file_is_lnk(const char *path);
		extern int file_is_blk(const char *path);
		extern int file_is_chr(const char *path);
		extern int file_is_fifo(const char *path);
		extern int file_is_sock(const char *path);
	4) extern int file_execute_type(const char *path)
	功能:
		获得可执行文件类型
	参数:
		path: 文件路径名
	返回值:
		类型值 如下:
			EXEC_TYPE_NOTEXEC,          /*文件不可执行*/
			EXEC_TYPE_ELF_EXC,          /*可执行ELF*/
			EXEC_TYPE_ELF_DYN,          /*动态库*/
			EXEC_TYPE_ELF_REL,          /*可重定向文件*/
			EXEC_TYPE_ELF_CORE,         /*core 文件*/ 
			EXEC_TYPE_SHELL_SCRPT,      /*是shell脚本*/
			EXEC_TYPE_PYTHON_SCRPT,     /*python脚本*/
			EXEC_TYPE_SUSPICIOUS_SCRPT, /*有执行权限 但没有写脚本头如(没写#!/bin/bash)*/
			EXEC_TYPE_OTHER_SCRPT,      /*暂未识别的脚本*/
	使用举例:
		src/example/testfile.c	
		
		
(5)	字符串操作相关(返回值为指向str串的地址) 
	1) char *fcp_left_strim(char *str)  去掉左边的空格
	   char *fcp_right_strim(char *str) 去掉右边的空格
	   char *fcp_strim(char *str)		去掉左边和右边的空格
	2)    
	   
(6) 其他相关操作		
	1) extern void fcp_set_log_cb(genlog_cb errcb, genlog_cb debugcb);
	功能: 
		设置库函数日志的输出回调
		在为设置回调的情况下,若./configure 使用了--enable-stderr=yes则错误消息将显示在标准错误输出上
							 若./configure 没有使用--enable-stderr=yes或使用了--enable-stderr=no 则错误将在syslog日志中显示
	参数:
		errcb 级别为err的错误消息接收回调
		debugcb 级别为debug的错误消息回调
		 类型为 typedef void(*genlog_cb)(const char *msg); 
	返回值:
		无
	
(7) socket操作相关
    1) int netutil_socket_nonblocking(int sockfd)
		功能: 将给定的socket设置为非阻塞socket
		参数: 
		   sockfd  socket文件描述符
		返回值:
			成功返回0  失败返回-1
	2) int netutil_socket_reuse_addr(int sockfd)
        功能: 让处于timewait状态下的socket可以重新绑定
        参数:
            sockfd socket文件描述符
        返回值:
             成功返回0 失败返回-1
    3) int netutil_socket_reuse_port(int sockfd)
        功能: 允许多个进程或者线程绑定到同一端口，提高服务器程序的性能,需要linux版本支持SO_REUSEPORT
        参数:
            sockfd socket文件描述符
        返回值: 成功返回0 失败返回-1
    4) int netutil_socket_closeonexec(int sockfd)
        功能: 在进程创建子进程执行exec的时候不继承父进程的这个sockfd
        参数: sockfd socket文件描述符
        返回值: 成功返回 0,失败返回-1
    5) int netutil_socket_nonblock_connect(const struct sockaddr *sa, int socklen)
        功能: 创建非阻塞socket连接
        参数:  
            sa: 要连接的服务器地址
            socklen: sa的真正长度
        返回值:
            成功返回创建的sock描述符,失败返回-1
		注意: 用户自己close socket文件描述符	
	6) int netutil_socket_block_connect(const struct sockaddr *seraddr, int socklen)
       	功能: 创建阻塞socket连接
        参数:  
            sa: 要连接的服务器地址
            socklen: sa的真正长度
        返回值:
            成功返回创建的sock描述符,失败返回-1
		注意: 用户自己close socket文件描述符	
	7) int netutil_socket_unix_pair(int pair[2])		
		   功能: 建立socket对 用于父子进程间的全双工通信
		   参数: pair  sock对每个pair[x]即可读 也可写
		   返回值: 功能返回0，失败返回-1
    8) int netutil_socket_accept(int sockfd, int backlog,struct sockaddr *seraddr,int addrlen, pf_set_listen_socket listensock_callback, pf_accept_callback accept_callback)
		   功能: 创建服务端的监听端口 并等待连接的建立
		   参数:
			  sockfd: 监听端口sock 传入-1,则函数自动创建一个监听sock, 若传入的不为0,则函数只执行accept操作,每次返回都有一个连接建立
			  backlog: 监听队列中未完成3次握手的队列的长度,用于传给listen
			  seraddr:监听的服务端地址
			  addrlen:监听服务端seraddr的真实长度
			  listensock_callback: 在未执行listen之前用户可以对监听socket进行属性设置
			  accept_callback:当收到一个连接时候的回调函数
		   返回值:
			  返回 监听socket描述符
		   注意:需要用户自己close 监听和连接的socket的描述符 
		   
(8)信号的相关处理,在其中会保存一个全局变量,用于保存用户修改过的信号,可以方便用户用del函数快速恢复信号原先的处理方式
	1) int sigutil_sig_add(int sig,pf_sig_handler handler)
	   功能: 添加一个信号的处理
       参数:
            sig:信号值  [1~NSIG-1]
            handler: 信号处理函数 原型是typedef void (*pf_sig_handler)(int)
       返回值: 成功返回0 失败返回-1
    2) int sigutil_sig_del(int sig);
       功能: 将add的信号,恢复到原先的处理方式	
	   参数: 
	       sig 要恢复的信号
       返回值: 成功返回0 失败返回-1
    
	3) int sigutil_sig_delall(void)
	   功能: 恢复add添加的所有信号
	   参数: 无
       返回值: 始终返回0	
	   
    4) int sigutil_ignore_all_sig(void)
	   功能:将忽略所有的信号信息
	   参数:无
	   返回值: 始终返回0
	   
    5) int sigutil_unblock_sig(int signum) 
	   功能:在某个进程中 取消阻塞某个信号
       参数:信号值 1~NSIG-1
       返回值:成功返回0 失败返回-1
	   
    6) int sigutil_block_sig(int signum)
       功能: 将某个信号进行阻塞
       参数:信号值 1~NSIG-1
       返回值:成功返回0 失败返回-1	   







	