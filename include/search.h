#pragma once

#include <stddef.h>
#include "git.h"

struct branches make_matches(size_t capacity);
void free_matches(struct branches *matches);
void update_matches(struct branches *all, struct branches *matches,
                    const char *query);
char *get_selected_branch(struct branches *all, struct branches *matches,
                          int selected, int in_search_mode);
