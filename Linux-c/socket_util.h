#ifndef __SOCKET_UTIL_H__
#define __SOCKET_UTIL_H__

int socket_util_set_reuse(int socket_fd);

int socket_util_set_nonblock(int socket_fd);

#endif