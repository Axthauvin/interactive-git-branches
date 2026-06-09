#include "interactive.h"

void enableRawMode(struct termios *orig_termios)
{
    tcgetattr(STDIN_FILENO, orig_termios);

    struct termios raw = *orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(struct termios *orig_termios)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void drawMenu(int selected, char **options)
{
    printf("\033[H\033[J");

    for (int i = 0; i < 3; i++)
    {
        if (i == selected)
            printf("> %s", options[i]);
        else
            printf("  %s", options[i]);
    }
}