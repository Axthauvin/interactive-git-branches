#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "git.h"
#include "interactive.h"
#include "popup.h"
#include "search.h"

int main()
{
    if (!isGitRepository())
    {
        exit(EXIT_FAILURE);
    }

    branches *branches = getGitBranches();

    struct termios orig_termios;
    enableRawMode(&orig_termios);

    int selected = 0;
    drawMenu(selected, branches, "  git branch manager  ");

    bool in_search_mode = false;
    char search_query[256] = { 0 };

    struct branches matches = make_matches(branches->count);

    while (1)
    {
        char c;
        int res = read(STDIN_FILENO, &c, 1);
        if (res < 0)
        {
            disableRawMode(&orig_termios);
            clearScreen();
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }

        if (c == '\033')
        {
            struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
            if (poll(&pfd, 1, 50) > 0 && (pfd.revents & POLLIN))
            {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[')
                {
                    if (read(STDIN_FILENO, &seq[1], 1) > 0)
                    {
                        int current_max = in_search_mode ? matches.count - 1
                                                         : branches->count - 1;

                        switch (seq[1])
                        {
                        case 'A':
                            if (selected > 0)
                                selected--;
                            break;

                        case 'B':
                            if (selected < current_max)
                                selected++;
                            else
                                selected = 0;
                            break;
                        }
                    }
                }
            }
            else
            {
                if (in_search_mode)
                {
                    in_search_mode = false;
                    search_query[0] = '\0';
                    selected = 0;
                    drawMenu(selected, branches, "  git branch manager  ");
                    continue;
                }
                else
                {
                    disableRawMode(&orig_termios);
                    clearScreen();
                    exit(EXIT_SUCCESS);
                }
            }

            if (!in_search_mode)
                drawMenu(selected, branches, "  git branch manager  ");
            else
                draw_search_bar(search_query, &matches, selected);
        }
        else if (c == '\n' || c == '\r')
        {
            if (!in_search_mode || matches.count > 0)
            {
                break;
            }
        }
        else if (!in_search_mode && (c == 's' || c == 'S'))
        {
            in_search_mode = true;
            search_query[0] = '\0';
            selected = 0;

            update_matches(branches, &matches, "");
            draw_search_bar(search_query, &matches, selected);
        }
        else if (!in_search_mode && (c == 'n' || c == 'N'))
        {
            printf("\a");
            char *new_branch = popup_input("New branch name:");
            if (new_branch)
            {
                if (strlen(new_branch) > 0)
                {
                    if (gitCreateBranch(new_branch))
                    {
                        freeGitBranches(branches);
                        branches = getGitBranches();
                        free_matches(&matches);
                        matches = make_matches(branches->count);
                        update_matches(branches, &matches, "");
                    }
                }
                free(new_branch);
            }
            drawMenu(selected, branches, "  git branch manager  ");
        }
        else if (!in_search_mode && (c == 'd' || c == 'D'))
        {
            if (branches->count > 0 && selected < branches->count)
            {
                char title[256];
                snprintf(title, sizeof(title), "Delete branch '%s'?",
                         branches->branches[selected].name);
                int choice = popup_choice(title, "Yes", "Cancel");
                if (choice == 1)
                {
                    bool proceed = true;
                    const char *base_branches[] = { "main", "master",
                                                    "dev",  "develop",
                                                    "prod", "staging" };
                    for (size_t i = 0;
                         i < sizeof(base_branches) / sizeof(base_branches[0]);
                         i++)
                    {
                        if (strcmp(branches->branches[selected].name,
                                   base_branches[i])
                            == 0)
                        {
                            char warn_title[256];
                            snprintf(warn_title, sizeof(warn_title),
                                     "WARNING: Delete base branch '%s'?",
                                     base_branches[i]);
                            int confirm =
                                popup_choice(warn_title, "Delete", "Cancel");
                            if (confirm != 1)
                            {
                                proceed = false;
                            }
                            break;
                        }
                    }

                    if (proceed)
                    {
                        if (gitDeleteBranch(branches->branches[selected].name))
                        {
                            freeGitBranches(branches);
                            branches = getGitBranches();
                            free_matches(&matches);
                            matches = make_matches(branches->count);
                            update_matches(branches, &matches, "");
                            if (selected >= branches->count && selected > 0)
                            {
                                selected--;
                            }
                        }
                        else
                        {
                            popup_message(
                                "Error",
                                "Failed to delete branch! Make sure it's "
                                "not the current branch and that it "
                                "doesn't have unmerged changes.");
                        }
                    }
                }
            }
            drawMenu(selected, branches, "  git branch manager  ");
        }
        else if (!in_search_mode
                 && (c == 'i' || c == 'I'))
        {
            clearScreen();
            printf("\n\n\n\n");
            printf("    \e[1;95mG\e[1;96mB\e[1;97mS\e[0m - \e[1;93mGit Branch "
                   "Switcher\e[0m\n");
            printf("    \e[2;37mA simple terminal tool to manage your git "
                   "branches\e[0m\n");
            printf("    \e[2;37mCreated by \e[1;94maxthauvin\e[0m\n");
            printf("    \e[2;37mPlease go leave a star on GitHub: "
                   "\e[1;94mhttps://github.com/axthauvin/gbs\e[0m ❤️\n");
            printf("\n\n\n\n");
            printf("\e[2;37mPress any key to return to the menu...\e[0m");
            read(STDIN_FILENO, &c, 1);
            drawMenu(selected, branches, "  git branch manager  ");
        }
        else if (in_search_mode)
        {
            size_t len = strlen(search_query);

            if (c == 127 || c == 8)
            {
                if (len > 0)
                {
                    search_query[len - 1] = '\0';
                }
            }
            else if (c >= 32 && c <= 126)
            {
                if (len < sizeof(search_query) - 1)
                {
                    search_query[len] = c;
                    search_query[len + 1] = '\0';
                }
            }

            update_matches(branches, &matches, search_query);

            selected = 0;
            draw_search_bar(search_query, &matches, selected);
        }
    }

    disableRawMode(&orig_termios);
    clearScreen();

    char *selected_branch =
        get_selected_branch(branches, &matches, selected, in_search_mode);

    if (!gitSwitch(selected_branch))
    {
        exit(EXIT_FAILURE);
    }

    free_matches(&matches);

    return 0;
}