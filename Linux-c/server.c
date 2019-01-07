#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"
#define SERVER_BACK_LOG 10
#define EPOLL_FD_COUNT 256
#define RECV_EP_EVENT_MAX 64
#define READ_BUF_MAX 1024

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
    ret = bind(listen_fd, (struct sockaddr *)&server_addr,addr_len);
    if (0 != ret)
    {
        printf("failed to bind: %s\n", strerror(errno));
        exit(1);
    }

    ret = listen(listen_fd, SERVER_BACK_LOG);
    if (0 != ret)
    {
        printf("failed to listen\n");
        exit(1);
    }

    epoll_fd = epoll_create(EPOLL_FD_COUNT);
    if (epoll_fd < 0)
    {
        printf("failed to epoll create\n");
        exit(1);
    }

    memset(&ep_event, 0, sizeof(ep_event));
    ep_event.events = EPOLLIN;
    ep_event.data.fd = listen_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ep_event);

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
                    if (recv_ep_event[i].data.fd == listen_fd)
                    {
                        memset(&client_addr, 0, sizeof(client_addr));
                        addr_len = sizeof(client_addr);
                        accept_fd = accept(recv_ep_event[i].data.fd,
                                (struct sockaddr *)&client_addr, &addr_len);
                        if (accept_fd < 0)
                        {
                            printf("failed to accept\n");
                            continue;
                        }

                        printf("accept fd[%d] from client\n", accept_fd);
                        memset(&ep_event, 0, sizeof(ep_event));
                        ep_event.events = EPOLLIN;
                        ep_event.data.fd = accept_fd;
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &ep_event);
                    }
                    else if (recv_ep_event[i].events & EPOLLIN)
                    {
                        memset(read_buf, 0, sizeof(read_buf));
                        ret = read(recv_ep_event[i].data.fd,
                                read_buf, READ_BUF_MAX);
                        printf("read something from fd[%d], ret[%d]\n",
                                recv_ep_event[i].data.fd, ret);
                        if (ret > 0)
                        {
                            printf("read buf[%s]\n", read_buf);
                            write(recv_ep_event[i].data.fd, read_buf, ret);
                        }
                        else
                        {
                            printf("client error fd[%d]\n",
                                    recv_ep_event[i].data.fd);
                            memset(&ep_event, 0, sizeof(ep_event));
                            ep_event.events = EPOLLIN;
                            ep_event.data.fd = recv_ep_event[i].data.fd;
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL,
                                    recv_ep_event[i].data.fd, &ep_event);
                            close(recv_ep_event[i].data.fd);
                        }
                    }
                }
                break;
        }
    }

    printf("server hello\n");
    return 0;
}
