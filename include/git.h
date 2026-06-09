#pragma once

#include <stdbool.h>

bool isGitRepository();

char **getGitBranches();

bool gitSwitch();