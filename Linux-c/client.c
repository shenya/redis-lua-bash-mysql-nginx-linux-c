#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

#include "socket_handler.h"
#include "socket_util.h"

#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"
#define SERVER_BACK_LOG 10
#define EPOLL_FD_COUNT 256
#define RECV_EP_EVENT_MAX 64
#define READ_BUF_MAX 1024

int stdin_read_callback(socket_event_t *user_event);

int socket_read_callback(socket_event_t *user_event);

int socket_write_callback(socket_event_t *user_event);

int socket_connect_callback(socket_event_t *user_event);

int client_active_fd_process(int epoll_fd, struct epoll_event *recv_ep_event,
        int event_count, int socket_fd);

int g_socket_fd;

int main(int argc, char *argv[])
{
    int socket_fd = -1;
    struct sockaddr_in server_addr;
    int addr_len = 0;
    int ret = -1;
    int epoll_fd = 0;
    struct epoll_event recv_ep_event[RECV_EP_EVENT_MAX];
    int timeout = 5 * 1000;
    int event_num = 0;
    socket_event_t *user_event = NULL;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        printf("fail to socket\n");
        exit(1);
    }

    g_socket_fd = socket_fd;

    ret = socket_util_set_nonblock(socket_fd);
    if (0 != ret)
    {
        printf("fail to fcntl: %s\n", strerror(errno));
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    addr_len = sizeof(server_addr);

    epoll_fd = epoll_create(EPOLL_FD_COUNT);
    if (epoll_fd < 0)
    {
        printf("failed to epoll create\n");
        exit(1);
    }

    user_event = (socket_event_t *)malloc(sizeof(socket_event_t));
    user_event->epoll_fd = epoll_fd;
    user_event->fd = socket_fd;
    user_event->events = EPOLLIN | EPOLLOUT;
    user_event->cb.read_callback = socket_read_callback;
    user_event->cb.write_callback = socket_connect_callback;
    socket_event_op(user_event, EPOLL_CTL_ADD);

    user_event = (socket_event_t *)malloc(sizeof(socket_event_t));
    user_event->epoll_fd = epoll_fd;
    user_event->fd = STDIN_FILENO;
    user_event->events = EPOLLIN;
    user_event->cb.read_callback = stdin_read_callback;
    user_event->cb.write_callback = NULL;

    socket_event_op(user_event, EPOLL_CTL_ADD);

    ret = connect(socket_fd, (struct sockaddr *)&server_addr, addr_len);
    if (ret < 0)
    {
        if (EINPROGRESS == errno)
        {
            printf("failed to connect: errno:%d, %s\n", errno, strerror(errno));
        }
        else
        {
            printf("failed to connect: errno:%d, %s\n", errno, strerror(errno));
            exit(1);
        }
    }

#if 0
    i=3;
    while (i)
    {
        read_num = read(socket_fd,
                read_buf, READ_BUF_MAX);
        i=i--;
        printf("read_num:%d, errno: %d, err:%s\n", read_num, errno, strerror(errno));
        sleep(1);
    }
    exit(1);
#endif

    while (1)
    {
        event_num = epoll_wait(epoll_fd, recv_ep_event,
                RECV_EP_EVENT_MAX, timeout);
        switch (event_num)
        {
            case 0:
                //printf("timeout\n");
                break;
            case -1:
                printf("error\n");
                break;
            default:
                 client_active_fd_process(epoll_fd, recv_ep_event,
                        event_num, socket_fd);
                break;
        }
    }

    printf("client hello\n");
    return 0;
}

int client_active_fd_process(int epoll_fd, struct epoll_event *recv_ep_event,
        int event_count, int socket_fd)
{
    int i = 0;    
    socket_event_t *user_event = NULL;

    for (i = 0; i < event_count; i++)
    {
        user_event = (socket_event_t *)recv_ep_event[i].data.ptr;
        if (recv_ep_event[i].events & EPOLLIN)
        {
            user_event->cb.read_callback(user_event);

        }
        else if (recv_ep_event[i].events & EPOLLOUT)
        {
            user_event->cb.write_callback(user_event);
        }
    }

    return 0;
}

int stdin_read_callback(socket_event_t *user_event)
{
    int ret = 0;
    char read_buf[READ_BUF_MAX] = {0};

    ret = read(user_event->fd,
            read_buf, READ_BUF_MAX);
    printf("read from stdin, ret[%d]\n", ret);
    if (ret > 0)
    {
        read_buf[ret - 1] = '\0';
        printf("read buf[%s]\n", read_buf);

        ret = write(g_socket_fd, read_buf, ret - 1);
    }
    else
    {
        socket_event_op(user_event, EPOLL_CTL_DEL);
        close(user_event->fd);
    }

    return 0;
}

int socket_read_callback(socket_event_t *user_event)
{
    int ret = 0;
    char read_buf[READ_BUF_MAX] = {0};

    ret = read(user_event->fd,
            read_buf, READ_BUF_MAX);                            
    printf("read from socket, ret[%d]\n", ret);
    if (ret > 0)
    {
        read_buf[ret] = '\0';
        printf("read buf[%s]\n", read_buf);
    }
    else if (0 == ret)
    {
        printf("server closed\n");
        socket_event_op(user_event, EPOLL_CTL_DEL);
        close(user_event->fd);
    }
    else
    {
        printf("server error\n");
        socket_event_op(user_event, EPOLL_CTL_DEL);
        close(user_event->fd);
    }

    return 0;
}

int socket_connect_callback(socket_event_t *user_event)
{
    printf("connect success\n");
    user_event->cb.write_callback = socket_write_callback;

    socket_event_op(user_event, EPOLL_CTL_MOD);

    return 0;
}

int socket_write_callback(socket_event_t *user_event)
{
    printf("%s\n", __FUNCTION__);

    user_event->events &= ~EPOLLOUT;
    socket_event_op(user_event, EPOLL_CTL_MOD);

    return 0;
}
