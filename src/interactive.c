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

void drawMenu(int selected)
{
    const char *options[] = { "Option 1", "Option 2", "Option 3" };

    printf("\033[H\033[J"); // Efface l'écran

    for (int i = 0; i < 3; i++)
    {
        if (i == selected)
            printf("> %s\n", options[i]);
        else
            printf("  %s\n", options[i]);
    }
}