#pragma once

#include <stdbool.h>
#include <stddef.h>

struct branch
{
    char *name;
};

struct branches
{
    struct branch *branches;
    size_t count;
} typedef branches;

bool isGitRepository();

branches *getGitBranches();

bool gitSwitch(const char *branch);

bool gitCreateBranch(const char *branch);

bool gitDeleteBranch(const char *branch);

const char *gitGetCurrentBranch();

void freeGitBranches(branches *b);