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

    remove(".branches");

    const char *current_branch = gitGetCurrentBranch();

    if (current_branch)
    {
        for (size_t i = 0; i < branches->count; i++)
        {
            if (strcmp(branches->branches[i].name, current_branch) == 0)
            {
                if (i > 0)
                {
                    char *temp = branches->branches[0].name;
                    branches->branches[0].name = branches->branches[i].name;
                    branches->branches[i].name = temp;
                }
                break;
            }
        }
    }

    return branches;
}

bool gitSwitch(const char *branch)
{
    char command[256];
    snprintf(command, sizeof(command), "git switch %s", branch);
    int cr = system(command);

    return cr == 0;
}

bool gitCreateBranch(const char *branch)
{
    char command[256];
    snprintf(command, sizeof(command), "git branch %s", branch);
    int cr = system(command);

    if (cr == 0)
    {
        return gitSwitch(branch);
    }
    return false;
}

bool gitDeleteBranch(const char *branch)
{
    char command[256];
    snprintf(command, sizeof(command), "git branch -D %s > /dev/null 2>&1",
             branch);
    int cr = system(command);

    return cr == 0;
}

const char *gitGetCurrentBranch()
{
    const char *command = "git branch --show-current";
    FILE *fp = popen(command, "r");
    if (!fp)
        return NULL;

    static char branch_name[256];
    if (fgets(branch_name, sizeof(branch_name), fp) != NULL)
    {
        branch_name[strcspn(branch_name, "\r\n")] = '\0';
        pclose(fp);
        return branch_name;
    }
    pclose(fp);
    return NULL;
}

void freeGitBranches(branches *b)
{
    if (b)
    {
        if (b->branches)
        {
            for (size_t i = 0; i < b->count; i++)
            {
                free(b->branches[i].name);
            }
            free(b->branches);
        }
        free(b);
    }
}