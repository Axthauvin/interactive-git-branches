#include "popup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>

#include "interactive.h"

static int get_term_width_popup(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 20)
        return w.ws_col;
    return 80;
}

static int get_term_height_popup(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 10)
        return w.ws_row;
    return 24;
}

char *popup_input(const char *title)
{
    clearScreen();
    int term_w = get_term_width_popup();
    int term_h = get_term_height_popup();
    int popup_w = 50;
    int popup_h = 6;
    if (popup_w > term_w)
        popup_w = term_w - 2;

    int start_x = (term_w - popup_w) / 2;
    int start_y = (term_h - popup_h) / 2;

    char input[256] = { 0 };
    int len = 0;

    while (1)
    {
        printf("\033[%d;1H", start_y);
        printf("\033[%dC\e[1;37m╔", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╗\e[0m\n");

        printf("\033[%dC\e[1;37m║", start_x);
        int title_len = strlen(title);
        int lpad = (popup_w - 2 - title_len) / 2;
        int rpad = popup_w - 2 - title_len - lpad;
        printf("%*s\e[1;96m%s\e[1;37m%*s║\e[0m\n", lpad, "", title, rpad, "");

        printf("\033[%dC\e[1;37m╠", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╣\e[0m\n");

        printf("\033[%dC\e[1;37m║", start_x);
        int ipad = popup_w - 2 - len - 2;
        if (ipad < 0)
            ipad = 0;
        printf(" \e[0m%s\e[5m_\e[25m%*s\e[1;37m║\e[0m\n", input, ipad, "");

        printf("\033[%dC\e[1;37m╚", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╝\e[0m\n");
        fflush(stdout);

        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0)
            break;

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
                    }
                }
                continue;
            }
            return NULL;
        }
        else if (c == '\n' || c == '\r')
        {
            if (len > 0)
                return strdup(input);
        }
        else if (c == 127 || c == 8)
        {
            if (len > 0)
            {
                len--;
                input[len] = '\0';
            }
        }
        else if (c >= 32 && c <= 126)
        {
            if (len < (int)sizeof(input) - 1)
            {
                input[len] = c;
                len++;
                input[len] = '\0';
            }
        }
    }
    return NULL;
}

int popup_choice(const char *title, const char *opt1, const char *opt2)
{
    int term_w = get_term_width_popup();
    int term_h = get_term_height_popup();
    int popup_w = 50;
    int popup_h = 6;
    if (popup_w > term_w)
        popup_w = term_w - 2;

    int start_x = (term_w - popup_w) / 2;
    int start_y = (term_h - popup_h) / 2;

    int selected = 1;

    while (1)
    {
        printf("\033[%d;1H", start_y);
        printf("\033[%dC\e[1;37m╔", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╗\e[0m\n");

        printf("\033[%dC\e[1;37m║", start_x);
        int title_len = strlen(title);
        int lpad = (popup_w - 2 - title_len) / 2;
        int rpad = popup_w - 2 - title_len - lpad;
        printf("%*s\e[1;96m%s\e[1;37m%*s║\e[0m\n", lpad, "", title, rpad, "");

        printf("\033[%dC\e[1;37m╠", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╣\e[0m\n");

        char options_buf[256];
        int opt_len_visible;
        if (selected == 1)
        {
            snprintf(options_buf, sizeof(options_buf),
                     "[\e[7m%s\e[27m]    [%s]", opt1, opt2);
            opt_len_visible = 2 + strlen(opt1) + 6 + strlen(opt2);
        }
        else
        {
            snprintf(options_buf, sizeof(options_buf),
                     "[%s]    [\e[7m%s\e[27m]", opt1, opt2);
            opt_len_visible = 2 + strlen(opt1) + 6 + strlen(opt2);
        }

        int opad_l = (popup_w - 2 - opt_len_visible) / 2;
        int opad_r = popup_w - 2 - opt_len_visible - opad_l;

        printf("\033[%dC\e[1;37m║\e[0m%*s%s%*s\e[1;37m║\e[0m\n", start_x,
               opad_l, "", options_buf, opad_r, "");

        printf("\033[%dC\e[1;37m╚", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╝\e[0m\n");
        fflush(stdout);

        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0)
            break;

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
                        if (seq[1] == 'C')
                        {
                            selected = 2;
                        }
                        else if (seq[1] == 'D')
                        {
                            selected = 1;
                        }
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else if (c == '\n' || c == '\r')
        {
            return selected;
        }
    }
    return 0;
}

void popup_message(const char *title, const char *message)
{
    int term_w = get_term_width_popup();
    int term_h = get_term_height_popup();
    int popup_w = 150;
    int popup_h = 6;
    if (popup_w > term_w)
        popup_w = term_w - 2;

    int start_x = (term_w - popup_w) / 2;
    int start_y = (term_h - popup_h) / 2;

    while (1)
    {
        printf("\033[%d;1H", start_y);
        printf("\033[%dC\e[1;37m╔", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╗\e[0m\n");

        printf("\033[%dC\e[1;37m║", start_x);
        int title_len = strlen(title);
        int lpad = (popup_w - 2 - title_len) / 2;
        int rpad = popup_w - 2 - title_len - lpad;
        printf("%*s\e[1;31m%s\e[1;37m%*s║\e[0m\n", lpad, "", title, rpad, "");

        printf("\033[%dC\e[1;37m╠", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╣\e[0m\n");

        printf("\033[%dC\e[1;37m║", start_x);
        int msg_len = strlen(message);
        int mlpad = (popup_w - 2 - msg_len) / 2;
        int mrpad = popup_w - 2 - msg_len - mlpad;
        printf("%*s\e[0m%s\e[1;37m%*s║\e[0m\n", mlpad, "", message, mrpad, "");

        printf("\033[%dC\e[1;37m║", start_x);
        const char *opt = "[\e[7m OK \e[27m]";
        int opt_len_visible = 6;
        int opad_l = (popup_w - 2 - opt_len_visible) / 2;
        int opad_r = popup_w - 2 - opt_len_visible - opad_l;
        printf("\e[0m%*s%s%*s\e[1;37m║\e[0m\n", opad_l, "", opt, opad_r, "");

        printf("\033[%dC\e[1;37m╚", start_x);
        for (int i = 0; i < popup_w - 2; i++)
            printf("═");
        printf("╝\e[0m\n");
        fflush(stdout);

        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0)
            break;

        if (c == '\n' || c == '\r' || c == '\033' || c == ' ')
        {
            if (c == '\033')
            {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 0 || seq[0] == 27)
                {
                    break;
                }
                if (read(STDIN_FILENO, &seq[1], 1) > 0)
                {
                }
            }
            break;
        }
    }
}
