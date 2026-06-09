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

char **getGitBranches()
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

    char **branches = NULL;
    size_t branches_count = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        branches_count++;
        branches = (char **)realloc(branches, branches_count * sizeof(char *));
        branches[branches_count - 1] = strdup(line);
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
    snprintf(command, sizeof(command), "git switch %s > /dev/null 2>&1",
             branch);
    int cr = system(command);

    return cr == 0;
}