#include <stddef.h>
#include "include/poll.h"

int poll_create(poll_t *poll)
{
    return ((poll->fd = epoll_create1(0)) == -1);
}

int poll_add(poll_t *poll, int fd, uint32_t events)
{
    return ((epoll_ctl(poll->fd, EPOLL_CTL_ADD, fd, &(struct epoll_event){
        .events     = events,
        .data.fd    = fd
    })) == -1);
}

int poll_mod(poll_t *poll, int fd, uint32_t events)
{
    return ((epoll_ctl(poll->fd, EPOLL_CTL_MOD, fd, &(struct epoll_event){
        .events     = events,
        .data.fd    = fd
    })) == -1);
}

int poll_del(poll_t *poll, int fd)
{
    return ((epoll_ctl(poll->fd, EPOLL_CTL_DEL, fd, NULL)) == -1);
}

int poll_wait(poll_t *poll, size_t max)
{
    struct epoll_event events[max];
    int count = epoll_wait(poll->fd, events, max, -1);

    // Check if epoll returned prematurely.
    if (count == -1)
        return (1);

    // Process the events.
    for (int i = 0; i < count; i++)
        poll->cb(events[i].data.fd, events[i].events, poll);

    return (0);
}
