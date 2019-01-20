#ifndef __SOCKET_HANDLER_H__
#define __SOCKET_HANDLER_H__

typedef struct socket_event socket_event_t;

typedef struct socket_event_call_back socket_event_cb_t;

struct socket_event_call_back
{
    int (*read_callback)(socket_event_t *user_event);
    int (*write_callback)(socket_event_t *user_event);
};

struct socket_event
{
    int fd;
    int epoll_fd;
    int events;
    int type;
    void *arg;
    struct socket_event_call_back cb;
    long last_active_timestamp;
};

int socket_event_op(socket_event_t *user_event, int operation, int events);

#endif
