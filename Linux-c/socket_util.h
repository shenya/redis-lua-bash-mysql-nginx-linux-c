#ifndef __SOCKET_UTIL_H__
#define __SOCKET_UTIL_H__

int socket_util_set_reuse(int socket_fd);

int socket_util_set_nonblock(int socket_fd);

int socket_util_set_sendbuf_length(int socket_fd, int length);

int socket_util_set_recvbuf_length(int socket_fd, int length);

int socket_util_get_sendbuf_length(int socket_fd, int *out_length);

int socket_util_get_recvbuf_length(int socket_fd, int *out_length);

int socket_util_epoll_event_op(int epoll_fd, int operation, int fd, int event);

int socket_util_epoll_event_op_user(int epoll_fd, int operation, int fd,
        int events, void *user_data_ptr);

#endif
