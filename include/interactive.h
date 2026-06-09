#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#pragma once

void enableRawMode(struct termios *orig_termios);

void disableRawMode(struct termios *orig_termios);
void drawMenu(int selected, char **options);