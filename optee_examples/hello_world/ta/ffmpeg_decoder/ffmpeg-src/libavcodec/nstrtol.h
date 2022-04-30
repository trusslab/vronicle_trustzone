#ifndef NSTRTOL
#define NSTRTOL

// #include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

long strtol (const char *nPtr, char **endPtr, int base);

#endif // for NSTRTOL