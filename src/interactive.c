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

void drawMenu(size_t selected, branches *branches, const char *title)
{
    clearScreen();

    int total = get_term_width();
    int inner = total - 2;

    //  header
    draw_hline(inner, 0xE2808F ? "╔" : "+", "═", "╗");
    int title_len = strlen(title);
    int lpad = (inner - title_len) / 2;
    int rpad = inner - title_len - lpad;
    printf("\e[2;37m║\e[1;37m%*s\e[0;37m%s%*s\e[2;37m║\e[0m\n", lpad, "", title,
           rpad, "");
    draw_hline(inner, "╠", "═", "╣");

    // options
    for (size_t i = 0; i < branches->count; i++)
    {
        int len = strlen(branches->branches[i].name);
        int pad = inner - 3 - len;

        if (pad < 0)
            pad = 0;

        if (i == selected)
        {
            printf("\e[2;37m║\e[0;37m \e[1;96m> %s%*s\e[0;36m\e[2;37m║\e[0m\n",
                   branches->branches[i].name, pad, "");
        }
        else
        {
            printf("\e[2;37m║   \e[0;37m%s%*s\e[2;37m║\e[0m\n",
                   branches->branches[i].name, pad, "");
        }
    }

    draw_hline(inner, "╠", "═", "╣");
    const char *actions = "  [s] search   [n] new branch";
    int act_len = strlen(actions);
    printf("\e[2;37m║\e[2m%s%*s\e[2;37m║\e[0m\n", actions, inner - act_len, "");
    draw_hline(inner, "╚", "═", "╝");

    printf("\n\e[0;37m ↑↓ navigate · Enter select · Esc quit\e[0m\n");
}

void draw_search_bar(const char *query, struct branches *matches,
                     size_t selected)
{
    clearScreen();

    // `matches` is expected to be already filtered by `query`

    // correct selected index if out of bounds
    if (selected >= matches->count)
        selected = matches->count - 1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
    if (selected < 0) // always unsigned --- IGNORE ---
        selected = 0;

    const char *title = "  search git branches  ";
    if (strlen(query) > 0)
    {
        static char title_buf[256];
        snprintf(title_buf, sizeof(title_buf), "  search: %s  ", query);
        title = title_buf;
    }

    drawMenu(selected, matches, title);
}