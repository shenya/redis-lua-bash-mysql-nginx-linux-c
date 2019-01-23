#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

int socket_util_set_reuse(int socket_fd)
{
    int ret = 0;
    int reuse = 1;

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,(const void *)&reuse,
            sizeof(int));
    if (0 != ret)
    {
        printf("failed to set reuse: %s\n", strerror(errno));
    }

    return ret;
}

int socket_util_set_nonblock(int socket_fd)
{
    int flags = 0;
    int ret = 0;

    flags = fcntl(socket_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    ret = fcntl(socket_fd, F_SETFL, flags);
    if (0 != ret)
    {
        printf("fail to fcntl: %s\n", strerror(errno));
    }

    return ret;
}

int socket_util_set_sendbuf_length(int socket_fd, int length)
{
    int ret = 0;

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, &length, sizeof(length));
    if (0 != ret)
    {
        printf("fail to set sendbuf length: %s\n", strerror(errno));
    }

    return ret;
}

int socket_util_set_recvbuf_length(int socket_fd, int length)
{
    int ret = 0;

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &length, sizeof(length));
    if (0 != ret)
    {
        printf("fail to set rcvbuf length: %s\n", strerror(errno));
    }

    return ret;
}

int socket_util_get_sendbuf_length(int socket_fd, int *out_length)
{
    int ret = 0;
    int len = 0;
    int length = 0;

    len = sizeof(length);
    ret = getsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, &length, (socklen_t *)&len);
    if (0 != ret)
    {
        printf("fail to get sendbuf length: %s\n", strerror(errno));
    }
    else
    {
        *out_length = length;
    }

    return ret;
}

int socket_util_get_recvbuf_length(int socket_fd, int *out_length)
{
    int ret = 0;
    int len = 0;
    int length = 0;

    len = sizeof(length);
    ret = getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &length, (socklen_t *)&len);
    if (0 != ret)
    {
        printf("fail to get recvbuf length: %s\n", strerror(errno));
    }
    else
    {
        *out_length = length;
    }

    return ret;
}

int socket_util_epoll_event_op(int epoll_fd, int operation, int fd, int event)
{
    struct epoll_event ep_event;

    memset(&ep_event, 0, sizeof(ep_event));
    ep_event.events = event;
    ep_event.data.fd = fd;

    epoll_ctl(epoll_fd, operation, fd, &ep_event);

    return 0;
}

int socket_util_epoll_event_op_user(int epoll_fd, int operation, int fd,
        int events, void *user_data_ptr)
{
    struct epoll_event ep_event;

    memset(&ep_event, 0, sizeof(ep_event));
    ep_event.events = events;
    ep_event.data.ptr = user_data_ptr;

    epoll_ctl(epoll_fd, operation, fd, &ep_event);

    return 0;
}
