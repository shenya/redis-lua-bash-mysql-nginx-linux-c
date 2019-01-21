#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>

#include "socket_handler.h"

#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"
#define SERVER_BACK_LOG 10
#define EPOLL_FD_COUNT 256
#define RECV_EP_EVENT_MAX 64
#define READ_BUF_MAX 1024

int socket_accept_callback(socket_event_t *user_event);

int client_read_callback(socket_event_t *user_event);

int main(int argc, char *argv[])
{
    int listen_fd = -1;
    struct sockaddr_in server_addr;
    int addr_len = 0;
    int ret = -1;
    int epoll_fd = 0;
    struct epoll_event ep_event;
    struct epoll_event recv_ep_event[RECV_EP_EVENT_MAX];
    int timeout = 5 * 1000;
    int event_num = 0;
    int accept_fd = -1;
    int i = 0;
    struct sockaddr_in client_addr;
    char read_buf[READ_BUF_MAX] = {0};
    int reuse = 1;
    socket_event_t *user_event = NULL;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        printf("fail to socket\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    addr_len = sizeof(server_addr);

    ret = socket_util_set_reuse(listen_fd);
    if (0 != ret)
    {
        printf("failed to set reuse: %s\n", strerror(errno));
        close(listen_fd);
        exit(1);
    }

    ret = bind(listen_fd, (struct sockaddr *)&server_addr,addr_len);
    if (0 != ret)
    {
        printf("failed to bind: %s\n", strerror(errno));
        close(listen_fd);
        exit(1);
    }

    ret = listen(listen_fd, SERVER_BACK_LOG);
    if (0 != ret)
    {
        printf("failed to listen\n");
        close(listen_fd);
        exit(1);
    }

    epoll_fd = epoll_create(EPOLL_FD_COUNT);
    if (epoll_fd < 0)
    {
        printf("failed to epoll create\n");
        close(listen_fd);
        exit(1);
    }

    user_event = (socket_event_t *)malloc(sizeof(socket_event_t));
    memset(user_event, 0, sizeof(socket_event_t));
    user_event->epoll_fd = epoll_fd;
    user_event->fd = listen_fd;
    user_event->events = EPOLLIN;
    user_event->cb.read_callback = socket_accept_callback;
    socket_event_op(user_event, EPOLL_CTL_ADD);

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
                printf("recv something\n");
                active_fd_process(epoll_fd, recv_ep_event,
                        event_num, listen_fd);
                break;
        }
    }

    printf("server hello\n");
    return 0;
}

int active_fd_process(int epoll_fd, struct epoll_event *recv_ep_event,
        int event_count, int listen_fd)
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
        else if(recv_ep_event[i].events & EPOLLOUT)
        {
            user_event->cb.write_callback(user_event);
        }
    }

    return 0;
}

int socket_accept_callback(socket_event_t *user_event)
{
    int ret = 0;
    char read_buf[READ_BUF_MAX] = {0};
    struct sockaddr_in client_addr;
    int addr_len = 0;
    int accept_fd = 0;
    socket_event_t *client_event = NULL;;

    memset(&client_addr, 0, sizeof(client_addr));
    addr_len = sizeof(client_addr);
    accept_fd = accept(user_event->fd,
            (struct sockaddr *)&client_addr, &addr_len);
    if (accept_fd < 0)
    {
        printf("failed to accept\n");
        return -1;
    }

    printf("accept fd[%d] from client\n", accept_fd);

    client_event = (socket_event_t *)malloc(sizeof(socket_event_t));
    memset(client_event, 0, sizeof(socket_event_t));
    client_event->epoll_fd = user_event->epoll_fd;
    client_event->fd = accept_fd;
    client_event->events = EPOLLIN;
    client_event->cb.read_callback = client_read_callback;
    socket_event_op(client_event, EPOLL_CTL_ADD);

    return 0;
}

int client_read_callback(socket_event_t *user_event)
{
    int ret = 0;
    char read_buf[READ_BUF_MAX] = {0};

    memset(read_buf, 0, sizeof(read_buf));
    ret = read(user_event->fd, read_buf, READ_BUF_MAX);
    printf("read something from fd[%d], ret[%d]\n",
            user_event->fd, ret);
    if (ret > 0)
    {
        printf("read buf[%s]\n", read_buf);
        ret = write(user_event->fd, read_buf, ret);
        printf("write ret: %d\n", ret);
    }
    else if (0 == ret)
    {
        printf("client closed fd[%d], ret:%d\n",
                user_event->fd, ret);

        socket_event_op(user_event, EPOLL_CTL_DEL);
        close(user_event->fd);
    }
    else
    {
        printf("client error fd[%d], ret:%d\n",
                user_event->fd, ret);

        socket_event_op(user_event, EPOLL_CTL_DEL);
        close(user_event->fd);
    }

    return 0;
}
