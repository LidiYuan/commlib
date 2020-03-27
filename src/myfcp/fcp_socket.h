#ifndef _FCP_SOCKET_H_
#define _FCP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>

int netutil_socket_nonblocking(int sockfd);
int netutil_socket_reuse_addr(int sockfd);
int netutil_socket_reuse_port(int sockfd);
int netutil_socket_closeonexec(int sockfd);
int netutil_socket_nonblock_connect(const struct sockaddr *sa, int socklen);
int netutil_socket_block_connect(const struct sockaddr *seraddr, int socklen);




#endif
