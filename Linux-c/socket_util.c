#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

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
