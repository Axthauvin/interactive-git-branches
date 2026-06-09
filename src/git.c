#include "git.h"

#include <stdio.h>
#include <stdlib.h>

bool isGitRepository()
{
    const char *command = "git remote -v";
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

    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);
    if (line)
        free(line);

    return NULL;
}