#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>


#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"
#define SERVER_BACK_LOG 10
#define EPOLL_FD_COUNT 256
#define RECV_EP_EVENT_MAX 64

int main(int argc, char *argv[])
{
    int socket_fd = -1;
    struct sockaddr_in server_addr;
    int addr_len = 0;
    int ret = -1;
    int epoll_fd = 0;
    struct epoll_event ep_event;
    struct epoll_event recv_ep_event[RECV_EP_EVENT_MAX];
    int timeout = 5 * 1000;
    int event_num = 0;
    int i = 0;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        printf("fail to socket\n");
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

    memset(&ep_event, 0, sizeof(ep_event));
    ep_event.events = EPOLLIN | EPOLLOUT;
    ep_event.data.fd = socket_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ep_event);

    ret = connect(socket_fd, (struct sockaddr *)&server_addr, addr_len);
    if (ret < 0)
    {
        printf("failed to connect\n");
        exit(1);
    }

    while (1)
    {
        event_num = epoll_wait(epoll_fd, recv_ep_event,
                RECV_EP_EVENT_MAX, timeout);
        switch (event_num)
        {
            case 0:
                printf("timeout\n");
                break;
            case -1:
                printf("error\n");
                break;
            default:
                printf("recv something\n");
                for (i = 0; i < event_num; i++)
                {
                    if (recv_ep_event[i].events & EPOLLIN)
                    {
                        printf("read something\n");
                    }
                    else if (recv_ep_event[i].events & EPOLLOUT)
                    {
                        printf("connect success\n");
                        memset(&ep_event, 0, sizeof(ep_event));
                        ep_event.events = EPOLLOUT;
                        ep_event.data.fd = recv_ep_event[i].data.fd;
                        
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL,
                                recv_ep_event[i].data.fd, &ep_event);
                    }
                }
                break;
        }
    }

    printf("server hello\n");
    return 0;
}
