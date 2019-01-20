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


#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"
#define SERVER_BACK_LOG 10
#define EPOLL_FD_COUNT 256
#define RECV_EP_EVENT_MAX 64
#define READ_BUF_MAX 1024

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
    char read_buf[READ_BUF_MAX] = {0};
    int read_num = 0;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        printf("fail to socket\n");
        exit(1);
    }

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

    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_ADD, socket_fd, EPOLLIN);

    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, EPOLLIN);

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
    struct sockaddr_in client_addr;
    int addr_len = 0;
    int accept_fd = 0;
    char read_buf[READ_BUF_MAX] = {0};
    int ret = 0;
    int read_num = 0;

    for (i = 0; i < event_count; i++)
    {
        if (STDIN_FILENO == recv_ep_event[i].data.fd)
        {
            if (recv_ep_event[i].events & EPOLLIN)
            {
                read_num = read(recv_ep_event[i].data.fd,
                        read_buf, READ_BUF_MAX);                            
                printf("read from stdin, ret[%d]\n", read_num);
                if (read_num > 0)
                {
                    read_buf[read_num - 1] = '\0';
                    write(socket_fd, read_buf, read_num);
                    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_ADD, socket_fd, EPOLLIN);
                }
                else
                {
                    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_DEL, STDIN_FILENO, EPOLLIN);
                }
            }
    
        }
        else 
        {
            if (recv_ep_event[i].events & EPOLLOUT)
            {
                printf("connect success\n");
                socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_DEL, recv_ep_event[i].data.fd, EPOLLIN);
            }
            else if (recv_ep_event[i].events & EPOLLIN)
            {
                read_num = read(recv_ep_event[i].data.fd,
                        read_buf, READ_BUF_MAX);
                printf("read from server, ret[%d]\n", read_num);
                if (read_num > 0)
                {
                    read_buf[read_num] = '\0';
                    printf("buf[%s]\n", read_buf);
                }
                else if (0 == read_num)
                {
                    printf("server closed: errno:%d, %s\n", errno, strerror(errno));
                    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_DEL,
                        recv_ep_event[i].data.fd, EPOLLIN);
                    close(recv_ep_event[i].data.fd);
                }
                else
                {
                    printf("server error: errno:%d, %s\n", errno, strerror(errno));
                    socket_util_epoll_event_op(epoll_fd, EPOLL_CTL_DEL,
                            recv_ep_event[i].data.fd, EPOLLIN);
                    close(recv_ep_event[i].data.fd);
                }
            }
        }
    }

    return 0;
}

