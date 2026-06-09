#include "git.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isGitRepository()
{
    const char *command = "git remote -v > /dev/null 2>&1";
    int cr = system(command);

    return cr == 0;
}

branches *getGitBranches()
{
    const char *command = "git branch --format='%(refname:short)' > .branches";
    int cr = system(command);

    if (cr != 0)
    {
        fprintf(stderr,
                "Error while reading the branches ? This is not supposed to "
                "happen %s\n",
                command);
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(".branches", "r");

    if (!fp)
    {
        printf("Error while reading the branches ? This is not supposed to "
               "happen\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    branches *branches = (struct branches *)malloc(sizeof(struct branches));
    branches->branches = NULL;
    branches->count = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        line[strcspn(line, "\r\n")] = '\0';
        branches->count++;
        branches->branches = (struct branch *)realloc(
            branches->branches, branches->count * sizeof(struct branch));
        branches->branches[branches->count - 1].name = strdup(line);
    }

    fclose(fp);
    if (line)
        free(line);

    // Delete the temporary file
    remove(".branches");

    return branches;
}

bool gitSwitch(const char *branch)
{
    char command[256];
    snprintf(command, sizeof(command), "git switch %s", branch);
    int cr = system(command);

    return cr == 0;
}