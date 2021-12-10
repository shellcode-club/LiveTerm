#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <signal.h>
#include "include/pty.h"
#include "include/poll.h"
#include "include/term.h"

volatile sig_atomic_t running = 1;

void on_poll(int fd, uint32_t events, poll_t *poll)
{
    char    buf[2048];
    size_t  len = read(fd, buf, 2048);

    // Set null byte.
    buf[len] = 0;

    if (fd == STDIN_FILENO) {
        write(3, buf, len);

    } else {
        write(0, buf, len);
    }
}

void on_child_quit(int sig)
{
    running = 0;
}

int main(int argc, char **argv)
{
    pty_t pty;
    pid_t child;

    // Ignore CTRL + C.
    signal(SIGCHLD, on_child_quit);

    // Create pseudo terminal.
    if (pty_create(&pty))
        return (1);

    // Fork process.
    if ((child = fork()) == -1)
        return (1);

    if (child == 0) {
        struct passwd *pwd;

        // Create process group.
        if (setsid() == -1)
            return (1);

        // Get user's shell location.
        if ((pwd = getpwuid(getuid())) == NULL)
            return (1);

        // Close master.
        close(pty.master);

        // Set the pty as IO.
        if (dup2(pty.slave, STDIN_FILENO) == -1)
            return (1);

        if (dup2(pty.slave, STDOUT_FILENO) == -1)
            return (1);

        if (dup2(pty.slave, STDERR_FILENO) == -1)
            return (1);

        // Show debug message.
        puts("Starting a shared terminal.");

        // Start a new shell.
        execve(pwd->pw_shell, (char*[]){ pwd->pw_shell, 0 }, (char*[]){ 0 });

        // Show error message.
        puts("Unexpected error.");

        return (1);

    } else {
        poll_t poll = (poll_t){ on_poll, NULL, 0 };

        // Set raw terminal.
        if (term_set_raw())
            return (1);

        // Create epoll.
        if (poll_create(&poll))
            return (1);

        // Add pty descriptor.
        if (poll_add(&poll, pty.master, EPOLLIN))
            return (1);

        // Add stdin descriptor.
        if (poll_add(&poll, STDIN_FILENO, EPOLLIN))
            return (1);

        // Run indefinitely.
        while (running) {
            poll_wait(&poll, 16);
        }
    }

    // Destroy pseudo terminal.
    pty_destroy(&pty);

    // Restore terminal.
    term_restore();

    // Show debug message.
    puts("Closing shared terminal.");

    return (0);
}
