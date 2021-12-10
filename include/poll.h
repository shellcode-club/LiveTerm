#pragma once

#include <sys/epoll.h>
#include <sys/signalfd.h>

typedef struct poll_t poll_t;
typedef void (*poll_cb_t)(int fd, uint32_t events, poll_t *poll);

struct poll_t
{
    poll_cb_t   cb;
    void        *data;
    int         fd;
};

int poll_create(poll_t *poll);
int poll_add(poll_t *poll, int fd, uint32_t events);
int poll_mod(poll_t *poll, int fd, uint32_t events);
int poll_del(poll_t *poll, int fd);
int poll_wait(poll_t *poll, size_t max);
