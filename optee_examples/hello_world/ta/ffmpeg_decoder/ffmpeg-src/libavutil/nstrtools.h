#ifndef NSTRTOOLS
#define NSTRTOOLS

#include <stddef.h>

size_t strspn(const char* cs, const char* ct);
size_t strcspn(const char *s, const char *reject);
llabs (long long int i);
// size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t);
long long strtoll(char* str, char** v, unsigned int base);
long long nstrtoll(char* str, char** v, unsigned int base);
void *bsearch (const void *key, const void *base0,
         size_t nmemb, size_t size,
         int (*compar)(const void *, const void *));
long lrint( double x );

#endif // for NSTRTOOLS