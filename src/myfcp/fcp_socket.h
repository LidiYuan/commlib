#ifndef _FCP_SOCKET_H_
#define _FCP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>


typedef int (*pf_accept_callback)(int sockfd,struct sockaddr *addr,int addrlen);
typedef int (*pf_set_listen_socket)(int listenfd);

extern int netutil_socket_nonblocking(int sockfd);
extern int netutil_socket_reuse_addr(int sockfd);
extern int netutil_socket_reuse_port(int sockfd);
extern int netutil_socket_closeonexec(int sockfd);
extern int netutil_socket_nonblock_connect(const struct sockaddr *sa, int socklen);
extern int netutil_socket_block_connect(const struct sockaddr *seraddr, int socklen);
extern int netutil_socket_unix_pair(int pair[2]);
extern int netutil_socket_accept(int backlog,struct sockaddr *seraddr,int addrlen, pf_set_listen_socket listensock_callback, pf_accept_callback accept_callback);

#endif
