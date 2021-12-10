#include <unistd.h>
#include <termios.h>
#include "include/term.h"

static struct termios save;

void term_restore(void)
{
    // Set terminal attributes.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &save);
}

int term_set_raw(void)
{
    struct termios term;

    // Get terminal attributes.
    if (tcgetattr(STDIN_FILENO, &term) == -1)
        return (1);

    // Save term.
    save = term;

    // Disable echo.
    term.c_iflag        &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term.c_oflag        &= ~(OPOST);
    term.c_cflag        |=  (CS8);
    term.c_lflag        &= ~(ECHO | ICANON | IEXTEN | ISIG);
    term.c_cc[VMIN]     = 0;
    term.c_cc[VTIME]    = 1;

    // Set terminal attributes.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
        return (1);

    return (0);
}
