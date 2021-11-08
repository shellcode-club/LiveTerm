#pragma once

typedef struct
{
    int     master;
    int     slave;
} pty_t;

int pty_create(pty_t *pty);
void pty_destroy(pty_t *pty);
