【一】文件介绍
src/mylib4c.h 总的头文件，想要使用此库,必须包含此头文件
src/example/testproc.c  一些和进程信息相关的例子
src/example/testnet.c   一些和网络操作相关的例子
src/example/testos.c    一些和系统信息获取相关的例子
src/example/testfile.c  一些和文件操作相关的例子

src/mylib4c/commproc.c 含有一些和进程信息相关的导出函数
src/mylib4c/commnet.c  含有一些和网络操作相关的导出函数
src/mylib4c/commos.c   含有一些和系统信息获取相关的导出函数
src/mylib4c/commfile.c   含有一些和文件操作相关的导出函数
 
【二】函数介绍
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
		获得系统上允许运行的进程数
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
	
