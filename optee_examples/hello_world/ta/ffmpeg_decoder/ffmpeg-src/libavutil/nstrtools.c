#include "nstrtools.h"

size_t strspn(const char* cs, const char* ct)
{
  size_t n;
  const char* p;
  for(n=0; *cs; cs++, n++) {
    for(p=ct; *p && *p != *cs; p++)
      ;
    if (!*p)
      break;
  }
  return n;
}
size_t strcspn(const char *s, const char *reject)
{
	const char *p;
	const char *r;
	size_t count = 0;
  /*@ loop invariant \base_addr(s) == \base_addr(p);
	    loop invariant valid_str(p);
			loop invariant s <= p <= s + strlen(s);
      loop invariant 0 <= count <= strlen(s);
			loop invariant count == p - s;
      //loop invariant strlen(s) == strlen(p) + (p - s);
			loop invariant \forall char *z, *t; s <= z < p && reject <= t < reject + strlen(reject) ==> *z != *t;
			loop invariant strcspn(s, reject) == strcspn(p, reject) + count;
			//loop assigns count, p, r;
      loop variant strlen(s) - (p - s);
 */
	for (p = s; *p != '\0'; ++p) {
    /*@ loop invariant \base_addr(reject) == \base_addr(r);
				loop invariant valid_str(r);
				loop invariant reject <= r <= reject + strlen(reject);
				//loop invariant strlen(reject) == strlen(r) + (r - reject);
				loop invariant \forall char *t; reject <= t < r ==> *p != *t;
        loop variant strlen(reject) - (r - reject);
    */
		for (r = reject; *r != '\0'; ++r) {
			if (*p == *r)
			  //@assert *p == *r ==> in_array(reject, *p);
				return count;
		}
		//@assert *r != '\0' ==> !in_array(reject, *p);
		++count;
	}
	return count;
}
llabs (long long int i)
{
  return i < 0 ? -i : i;
}
// size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t)
// {
//     // Temp solution
// 	return (size_t)0;
// }
#include <limits.h>   // ULLONG_MAX

long long strtoll(char* str, char** v, unsigned int base) {
    long long num = 0;
    long long cutoff = LLONG_MAX;
    int tooBig = 0, neg = 0, hasDigits = 0;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
    int radixSet = base, radixLiteral = 0;
#endif
    char* start = str;
    char c = *str;
    if (c == '-') {
        neg = 1;
        c = *++str;
    } else if (c == '+') {
        c = *++str;
    }
    if (c == '0' && base == 0) {
        c = *++str;
        switch (c) {
            case 'x':
                base = 16;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = 1;
#endif
                ++str;
                break;
            case 'o':
                base = 8;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = 1;
#endif
                ++str;
                break;
            case 'b':
                base = 2;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = 1;
#endif
                ++str;
                break;
            default:
#ifdef LEADING_ZERO_AS_OCTAL
                base = 8;
#else
                base = 10;
#endif
                start = str;
                break;
        }
    } else if (base == 0)
        base = 10;
    else {
        switch (base) {
            case 2:
                if (*(str + 1) == 'b') str += 2;
                break;
            case 8:
                if (*(str + 1) == 'o') str += 2;
                break;
            case 16:
                if (*(str + 1) == 'x') str += 2;
                break;
        }
    }
    long long cutlimit = cutoff % base;
    cutoff /= base;
    for (;;) {
        c = *str++;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
        if (c == 'r' && !radixSet) {
            radixSet = 1;
            if (num <= 36 && num >= 2) {
                base = num;
                num = 0;
                hasDigits = 0;
                radixLiteral = 1;
                start = str - 1;
                continue;
            } else {
                --str;
                break;
            }
        } else
#endif
            if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c == '_')
            continue;
        else {
            --str;
            break;
        }
        if (c >= base) {
            --str;
            break;
        }
        hasDigits = 1;
        if (num > cutoff || (num == cutoff && c > cutlimit)) tooBig = 1;
        if (!tooBig) {
            num *= base;
            num += c;
        }
    }
    if (v) {
        if (hasDigits)
            *v = str;
        else {
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
            if (radixLiteral) num = base;
#endif
            *v = start;
        }
    }
    if (tooBig) {
        return LLONG_MAX;
    }
    if (neg) num *= -1;
    return num;
}

long long nstrtoll(char* str, char** v, unsigned int base) {
    return strtoll(str, v, base);
}

void *
bsearch (const void *key, const void *base0,
         size_t nmemb, size_t size,
         int (*compar)(const void *, const void *))
{
	const char *base = (const char *) base0;
	int lim, cmp;
	const void *p;

	for (lim = nmemb; lim != 0; lim >>= 1) {
		p = base + (lim >> 1) * size;
		cmp = (*compar)(key, p);
		if (cmp == 0)
			return (void *)p;
		if (cmp > 0) {	/* key > p: move right */
			base = (const char *)p + size;
			lim--;
		} /* else move left */
	}
	return (NULL);
}

long lrint( double x )
{
	double rounded = round( x );
	long result = (long)rounded;
	
	if (__builtin_fabs(result - x) != 0.5)
		return result;
	else {
		// Exact halfway case
		if (result & 1L) {
			// If the trailing bit is set, we rounded the wrong way
			long step = (result >> 30) | 1L;// x < 0 ? -1 : 1
			return result - step;			// x < 0 ? result + 1 : result - 1
		}
		else {
			return result;
		}
	}
}
