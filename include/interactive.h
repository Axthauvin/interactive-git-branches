#pragma once

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "git.h"

void enableRawMode(struct termios *orig_termios);

void disableRawMode(struct termios *orig_termios);
void drawMenu(size_t selected, struct branches *branches, const char *title);
void draw_search_bar(const char *query, struct branches *matches,
                     size_t selected);

void clearScreen();