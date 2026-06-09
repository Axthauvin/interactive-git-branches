#include "interactive.h"

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

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
static int get_term_width(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 20)
        return w.ws_col;
    return 80; // fallback
}

static void draw_hline(int inner_w, const char *left, const char *fill,
                       const char *right)
{
    printf("\e[2;37m%s", left);
    for (int i = 0; i < inner_w; i++)
        printf("%s", fill);
    printf("%s\e[0m\n", right);
}

static void draw_line(int inner_w, const char *prefix_ansi, const char *content,
                      int content_len, const char *suffix_ansi,
                      const char *suffix, int suffix_len)
{
    int pad = inner_w - content_len - suffix_len;
    if (pad < 0)
        pad = 0;
    printf("\e[2;37m║%s%s", prefix_ansi, content);
    printf("%*s", pad, "");
    printf("%s%s\e[2;37m║\e[0m\n", suffix_ansi, suffix);
}

void drawMenu(int selected, char **options)
{
    clearScreen();

    int total = get_term_width();
    int inner = total - 2;

    const int badge_w[] = { 9, 9, 5 };

    //  header
    draw_hline(inner, 0xE2808F ? "╔" : "+", "═", "╗");
    const char *title = "  git branch manager  ";
    int title_len = strlen(title);
    int lpad = (inner - title_len) / 2;
    int rpad = inner - title_len - lpad;
    printf("\e[2;37m║\e[1;37m%*s%s%*s\e[2;37m║\e[0m\n", lpad, "", title, rpad,
           "");
    draw_hline(inner, "╠", "═", "╣");

    // options
    for (int i = 0; i < 3; i++)
    {
        int len = strlen(options[i]);
        int pad = inner - 3 - len;

        if (pad < 0)
            pad = 0;

        if (i == selected)
        {
            printf("\e[2;37m║ \e[1;96m▌ %s%*s\e[0;36m\e[2;37m║\e[0m\n",
                   options[i], pad, "");
        }
        else
        {
            printf("\e[2;37m║   \e[2;37m%s%*s\e[2;37m║\e[0m\n", options[i], pad,
                   "");
        }
    }

    draw_hline(inner, "╠", "═", "╣");
    const char *actions = "  [s] search   [n] new branch";
    int act_len = strlen(actions);
    printf("\e[2;37m║\e[2m%s%*s\e[2;37m║\e[0m\n", actions, inner - act_len, "");
    draw_hline(inner, "╚", "═", "╝");

    printf("\n\e[2;37m ↑↓ navigate · Enter select · Esc quit\e[0m\n");
}