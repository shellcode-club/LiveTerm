#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "include/pty.h"

int pty_create(pty_t *pty)
{
    const char *pts;

    if ((pty->master = posix_openpt(O_RDWR | O_NOCTTY)) == -1)
        return (1);

    if (grantpt(pty->master) || unlockpt(pty->master))
        return (1);

    if ((pts = ptsname(pty->master)) == NULL)
        return (1);

    if ((pty->slave = open(pts, O_RDWR | O_NOCTTY)) == -1)
        return (1);

    return (0);
}

void pty_destroy(pty_t *pty)
{
    close(pty->master);
    close(pty->slave);
}
