#include "socket_util.h"
#include "socket_handler.h"

int socket_event_op(socket_event_t *user_event, int operation)
{
    int ret = 0;

    ret = socket_util_epoll_event_op_user(user_event->epoll_fd,
            operation, user_event->fd,
            user_event->events, user_event);

    return ret;
}
