#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pwd.h>
#include <wait.h>

#include "liveterm.h"

int pty_create(pty_t *pty)
{
    if ((pty->master = getpt()) == -1)
        return (1);

    if (grantpt(pty->master) || unlockpt(pty->master))
        return (1);

    if ((pty->slave = open(ptsname(pty->master), (O_RDWR | O_NOCTTY))) == -1)
        return (1);

    return (0);
}

void pty_destroy(pty_t *pty)
{
    close(pty->master);
    close(pty->slave);
}

int main(int argc, char **argv)
{
    struct passwd   *pwd;
    pty_t           pty;
    pid_t           child;

    // Ignore CTRL + C.
    signal(SIGINT, SIG_IGN);

    if (pty_create(&pty))
        return (1);

    if ((child = fork()) == -1)
        return (1);

    if (child == 0) {

        // Get user's shell location.
        if ((pwd = getpwuid(getuid())) == NULL)
            return (1);

        // Set the PTY as IO.
        dup2(pty.slave, STDOUT_FILENO);
        dup2(pty.slave, STDERR_FILENO);
        dup2(pty.master, STDIN_FILENO);

        // Show debug message.
        printf("Starting a shared terminal.\n");

        // Start a new shell.
        execv(pwd->pw_shell, &(char*){ 0 });

        // Show error message.
        printf("Unexpected error.\n");

        return (1);
    }

    // Wait on child process.
    while ((child = wait(NULL)) > 0);

    // Show debug message.
    printf("Closing shared terminal.\n");

    // Destroy pseudo terminal.
    pty_destroy(&pty);

    return (0);
}
