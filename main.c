#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "git.h"
#include "interactive.h"

int main()
{
    // First we check if its a git repo !

    if (!isGitRepository())
    {
        exit(EXIT_FAILURE);
    }

    getGitBranches();

    // struct termios orig_termios;
    // enableRawMode(&orig_termios);

    // int selected = 0;
    // drawMenu(selected);

    // while (1)
    // {
    //     char c;
    //     read(STDIN_FILENO, &c, 1);

    //     if (c == '\033')
    //     { // ESC
    //         char seq[2];
    //         read(STDIN_FILENO, &seq[0], 1);
    //         read(STDIN_FILENO, &seq[1], 1);

    //         if (seq[0] == '[')
    //         {
    //             switch (seq[1])
    //             {
    //             case 'A': // Flèche haut
    //                 if (selected > 0)
    //                     selected--;
    //                 break;

    //             case 'B': // Flèche bas
    //                 if (selected < 2)
    //                     selected++;
    //                 break;
    //             }
    //         }

    //         drawMenu(selected);
    //     }

    //     if (c == '\n' || c == '\r')
    //     {
    //         break;
    //     }
    // }

    // disableRawMode(&orig_termios);

    // printf("\nVous avez choisi l'option %d\n", selected + 1);

    return 0;
}