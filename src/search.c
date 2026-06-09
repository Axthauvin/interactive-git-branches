#include "search.h"

#include <stdlib.h>
#include <string.h>

struct branches make_matches(size_t capacity)
{
    struct branches m;
    m.branches = (struct branch *)malloc(capacity * sizeof(struct branch));
    m.count = 0;
    return m;
}

void free_matches(struct branches *matches)
{
    if (matches && matches->branches)
    {
        free(matches->branches);
        matches->branches = NULL;
        matches->count = 0;
    }
}

void update_matches(struct branches *all, struct branches *matches,
                    const char *query)
{
    if (!all || !matches || !query)
        return;

    matches->count = 0;
    for (size_t i = 0; i < all->count; i++)
    {
        if (strstr(all->branches[i].name, query) != NULL)
        {
            matches->branches[matches->count] = all->branches[i];
            matches->count++;
        }
    }
}

char *get_selected_branch(struct branches *all, struct branches *matches,
                          int selected, int in_search_mode)
{
    if (in_search_mode && matches && matches->count > 0)
    {
        if (selected < 0)
            selected = 0;
        if ((size_t)selected >= matches->count)
            selected = (int)matches->count - 1;
        return matches->branches[selected].name;
    }

    if (all && all->count > 0)
    {
        if (selected < 0)
            selected = 0;
        if ((size_t)selected >= all->count)
            selected = (int)all->count - 1;
        return all->branches[selected].name;
    }

    return NULL;
}
