
文件介绍
src/gencompo4c.h 总的头文件，想要使用此库,必须包含此头文件
src/example/testfile.c  和文件操作相关的例子
src/example/testnet.c   和网络操作相关的例子

src/gencompo4c/commfile.c 含有一些和文件操作相关的导出函数
src/gencompo4c/commnet.c  含有一些和网络操作相关的导出函数
 
函数介绍
网络操作相关
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
 
文件操作相关
1)extern int com_find_proc_pid(procpid_cb callback,void *userarg);
功能:
	遍历/proc下的所有pid
参数介绍:
	callback:每获得一个pid就会回调此函数 原型,typedef int (*procpid_cb)(const char *name,void *usrarg);
    userarg:用户私有数据
返回值:
    成功返回0 失败返回负数
使用举例看src/example/testfile.c	

