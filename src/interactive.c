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

void clearScreen()
{
    printf("\033[H\033[J");
    fflush(stdout); // ensure clear screen is executed immediately
}

void drawMenu(int selected, char **options)
{
    clearScreen();

    for (int i = 0; i < 3; i++)
    {
        if (i == selected)
            printf("\e[0;32m> %s\e[0;37m", options[i]);
        else
            printf("  %s", options[i]);
    }

    printf("\n\n[s] Search for a branch - [n] Create a new branch\n");
    printf("\n\nUse arrow keys to navigate and Enter to select.\n");
    printf("Press ESC to exit.\n");
}