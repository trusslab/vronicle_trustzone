/*
 * copyright (c) 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_VERSION_H
#define AVUTIL_VERSION_H

// Custom Errno definition
int errno;
#define	EPERM		1		/* Operation not permitted */
#define	ENOENT		2		/* No such file or directory */
#define	ESRCH		3		/* No such process */
#define	EINTR		4		/* Interrupted system call */
#define	EIO		5		/* Input/output error */
#define	ENXIO		6		/* Device not configured */
#define	E2BIG		7		/* Argument list too long */
#define	ENOEXEC		8		/* Exec format error */
#define	EBADF		9		/* Bad file descriptor */
#define	ECHILD		10		/* No child processes */
#define	EDEADLK		11		/* Resource deadlock avoided */
					/* 11 was EAGAIN */
#define	ENOMEM		12		/* Cannot allocate memory */
#define	EACCES		13		/* Permission denied */
#define	EFAULT		14		/* Bad address */
#ifndef _POSIX_SOURCE
#define	ENOTBLK		15		/* Block device required */
#define	EBUSY		16		/* Device busy */
#endif
#define	EEXIST		17		/* File exists */
#define	EXDEV		18		/* Cross-device link */
#define	ENODEV		19		/* Operation not supported by device */
#define	ENOTDIR		20		/* Not a directory */
#define	EISDIR		21		/* Is a directory */
#define	EINVAL		22		/* Invalid argument */
#define	ENFILE		23		/* Too many open files in system */
#define	EMFILE		24		/* Too many open files */
#define	ENOTTY		25		/* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define	ETXTBSY		26		/* Text file busy */
#endif
#define	EFBIG		27		/* File too large */
#define	ENOSPC		28		/* No space left on device */
#define	ESPIPE		29		/* Illegal seek */
#define	EROFS		30		/* Read-only file system */
#define	EMLINK		31		/* Too many links */
#define	EPIPE		32		/* Broken pipe */

/* math software */
#define	EDOM		33		/* Numerical argument out of domain */
#define	ERANGE		34		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		35		/* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	36		/* Operation now in progress */
#define	EALREADY	37		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	38		/* Socket operation on non-socket */
#define	EDESTADDRREQ	39		/* Destination address required */
#define	EMSGSIZE	40		/* Message too long */
#define	EPROTOTYPE	41		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42		/* Protocol not available */
#define	EPROTONOSUPPORT	43		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44		/* Socket type not supported */
#define	EOPNOTSUPP	45		/* Operation not supported on socket */
#define	EPFNOSUPPORT	46		/* Protocol family not supported */
#define	EAFNOSUPPORT	47		/* Address family not supported by protocol family */
#define	EADDRINUSE	48		/* Address already in use */
#define	EADDRNOTAVAIL	49		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	50		/* Network is down */
#define	ENETUNREACH	51		/* Network is unreachable */
#define	ENETRESET	52		/* Network dropped connection on reset */
#define	ECONNABORTED	53		/* Software caused connection abort */
#define	ECONNRESET	54		/* Connection reset by peer */
#define	ENOBUFS		55		/* No buffer space available */
#define	EISCONN		56		/* Socket is already connected */
#define	ENOTCONN	57		/* Socket is not connected */
#define	ESHUTDOWN	58		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	59		/* Too many references: can't splice */
#define	ETIMEDOUT	60		/* Connection timed out */
#define	ECONNREFUSED	61		/* Connection refused */

#define	ELOOP		62		/* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
#define	ENAMETOOLONG	63		/* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define	EHOSTDOWN	64		/* Host is down */
#define	EHOSTUNREACH	65		/* No route to host */
#endif /* _POSIX_SOURCE */
#define	ENOTEMPTY	66		/* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define	EPROCLIM	67		/* Too many processes */
#define	EUSERS		68		/* Too many users */
#define	EDQUOT		69		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		70		/* Stale NFS file handle */
#define	EREMOTE		71		/* Too many levels of remote in path */
#define	EBADRPC		72		/* RPC struct is bad */
#define	ERPCMISMATCH	73		/* RPC version wrong */
#define	EPROGUNAVAIL	74		/* RPC prog. not avail */
#define	EPROGMISMATCH	75		/* Program version wrong */
#define	EPROCUNAVAIL	76		/* Bad procedure for program */
#endif /* _POSIX_SOURCE */

#define	ENOLCK		77		/* No locks available */
#define	ENOSYS		78		/* Function not implemented */

#define	EFTYPE		79		/* Inappropriate file type or format */

// Start of functions to be included for OP-TEE
// #include <stddef.h>
// size_t strspn(const char* cs, const char* ct)
// {
//   size_t n;
//   const char* p;
//   for(n=0; *cs; cs++, n++) {
//     for(p=ct; *p && *p != *cs; p++)
//       ;
//     if (!*p)
//       break;
//   }
//   return n;
// }
// size_t strcspn(const char *s, const char *reject)
// {
// 	const char *p;
// 	const char *r;
// 	size_t count = 0;
//   /*@ loop invariant \base_addr(s) == \base_addr(p);
// 	    loop invariant valid_str(p);
// 			loop invariant s <= p <= s + strlen(s);
//       loop invariant 0 <= count <= strlen(s);
// 			loop invariant count == p - s;
//       //loop invariant strlen(s) == strlen(p) + (p - s);
// 			loop invariant \forall char *z, *t; s <= z < p && reject <= t < reject + strlen(reject) ==> *z != *t;
// 			loop invariant strcspn(s, reject) == strcspn(p, reject) + count;
// 			//loop assigns count, p, r;
//       loop variant strlen(s) - (p - s);
//  */
// 	for (p = s; *p != '\0'; ++p) {
//     /*@ loop invariant \base_addr(reject) == \base_addr(r);
// 				loop invariant valid_str(r);
// 				loop invariant reject <= r <= reject + strlen(reject);
// 				//loop invariant strlen(reject) == strlen(r) + (r - reject);
// 				loop invariant \forall char *t; reject <= t < r ==> *p != *t;
//         loop variant strlen(reject) - (r - reject);
//     */
// 		for (r = reject; *r != '\0'; ++r) {
// 			if (*p == *r)
// 			  //@assert *p == *r ==> in_array(reject, *p);
// 				return count;
// 		}
// 		//@assert *r != '\0' ==> !in_array(reject, *p);
// 		++count;
// 	}
// 	return count;
// }
// llabs (long long int i)
// {
//   return i < 0 ? -i : i;
// }
// size_t
// strftime(s, maxsize, format, t)
// 	char *s;
// 	size_t maxsize;
// 	const char *format;
// 	const struct tm *t;
// {
//     // Temp solution
// 	return 0;
// }
// #include <limits.h>   // ULLONG_MAX

// static long long strtoll(char* str, char** v, unsigned int base) {
//     long long num = 0;
//     long long cutoff = LLONG_MAX;
//     int tooBig = 0, neg = 0, hasDigits = 0;
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//     int radixSet = base, radixLiteral = 0;
// #endif
//     char* start = str;
//     char c = *str;
//     if (c == '-') {
//         neg = 1;
//         c = *++str;
//     } else if (c == '+') {
//         c = *++str;
//     }
//     if (c == '0' && base == 0) {
//         c = *++str;
//         switch (c) {
//             case 'x':
//                 base = 16;
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//                 radixSet = 1;
// #endif
//                 ++str;
//                 break;
//             case 'o':
//                 base = 8;
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//                 radixSet = 1;
// #endif
//                 ++str;
//                 break;
//             case 'b':
//                 base = 2;
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//                 radixSet = 1;
// #endif
//                 ++str;
//                 break;
//             default:
// #ifdef LEADING_ZERO_AS_OCTAL
//                 base = 8;
// #else
//                 base = 10;
// #endif
//                 start = str;
//                 break;
//         }
//     } else if (base == 0)
//         base = 10;
//     else {
//         switch (base) {
//             case 2:
//                 if (*(str + 1) == 'b') str += 2;
//                 break;
//             case 8:
//                 if (*(str + 1) == 'o') str += 2;
//                 break;
//             case 16:
//                 if (*(str + 1) == 'x') str += 2;
//                 break;
//         }
//     }
//     long long cutlimit = cutoff % base;
//     cutoff /= base;
//     for (;;) {
//         c = *str++;
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//         if (c == 'r' && !radixSet) {
//             radixSet = 1;
//             if (num <= 36 && num >= 2) {
//                 base = num;
//                 num = 0;
//                 hasDigits = 0;
//                 radixLiteral = 1;
//                 start = str - 1;
//                 continue;
//             } else {
//                 --str;
//                 break;
//             }
//         } else
// #endif
//             if (c >= '0' && c <= '9')
//             c -= '0';
//         else if (c >= 'a' && c <= 'z')
//             c -= 'a' - 10;
//         else if (c >= 'A' && c <= 'Z')
//             c -= 'A' - 10;
//         else if (c == '_')
//             continue;
//         else {
//             --str;
//             break;
//         }
//         if (c >= base) {
//             --str;
//             break;
//         }
//         hasDigits = 1;
//         if (num > cutoff || (num == cutoff && c > cutlimit)) tooBig = 1;
//         if (!tooBig) {
//             num *= base;
//             num += c;
//         }
//     }
//     if (v) {
//         if (hasDigits)
//             *v = str;
//         else {
// #ifdef ALLOW_SMALLTALK_RADIX_LITERALS
//             if (radixLiteral) num = base;
// #endif
//             *v = start;
//         }
//     }
//     if (tooBig) {
//         errno = ERANGE;
//         return LLONG_MAX;
//     }
//     if (neg) num *= -1;
//     return num;
// }
// End of functions to be included for OP-TEE

#include "macros.h"

/**
 * @addtogroup version_utils
 *
 * Useful to check and match library version in order to maintain
 * backward compatibility.
 *
 * @{
 */

#define AV_VERSION_INT(a, b, c) ((a)<<16 | (b)<<8 | (c))
#define AV_VERSION_DOT(a, b, c) a ##.## b ##.## c
#define AV_VERSION(a, b, c) AV_VERSION_DOT(a, b, c)

/**
 * Extract version components from the full ::AV_VERSION_INT int as returned
 * by functions like ::avformat_version() and ::avcodec_version()
 */
#define AV_VERSION_MAJOR(a) ((a) >> 16)
#define AV_VERSION_MINOR(a) (((a) & 0x00FF00) >> 8)
#define AV_VERSION_MICRO(a) ((a) & 0xFF)

/**
 * @}
 */

/**
 * @file
 * @ingroup lavu
 * Libavutil version macros
 */

/**
 * @defgroup lavu_ver Version and Build diagnostics
 *
 * Macros and function useful to check at compiletime and at runtime
 * which version of libavutil is in use.
 *
 * @{
 */

#define LIBAVUTIL_VERSION_MAJOR  55
#define LIBAVUTIL_VERSION_MINOR  24
#define LIBAVUTIL_VERSION_MICRO 100

#define LIBAVUTIL_VERSION_INT   AV_VERSION_INT(LIBAVUTIL_VERSION_MAJOR, \
                                               LIBAVUTIL_VERSION_MINOR, \
                                               LIBAVUTIL_VERSION_MICRO)
#define LIBAVUTIL_VERSION       AV_VERSION(LIBAVUTIL_VERSION_MAJOR,     \
                                           LIBAVUTIL_VERSION_MINOR,     \
                                           LIBAVUTIL_VERSION_MICRO)
#define LIBAVUTIL_BUILD         LIBAVUTIL_VERSION_INT

#define LIBAVUTIL_IDENT         "Lavu" AV_STRINGIFY(LIBAVUTIL_VERSION)

/**
 * @}
 *
 * @defgroup depr_guards Deprecation guards
 * FF_API_* defines may be placed below to indicate public API that will be
 * dropped at a future version bump. The defines themselves are not part of
 * the public API and may change, break or disappear at any time.
 *
 * @note, when bumping the major version it is recommended to manually
 * disable each FF_API_* in its own commit instead of disabling them all
 * at once through the bump. This improves the git bisect-ability of the change.
 *
 * @{
 */

#ifndef FF_API_VDPAU
#define FF_API_VDPAU                    (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_XVMC
#define FF_API_XVMC                     (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_OPT_TYPE_METADATA
#define FF_API_OPT_TYPE_METADATA        (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_DLOG
#define FF_API_DLOG                     (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_VAAPI
#define FF_API_VAAPI                    (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_FRAME_QP
#define FF_API_FRAME_QP                 (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_PLUS1_MINUS1
#define FF_API_PLUS1_MINUS1             (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_ERROR_FRAME
#define FF_API_ERROR_FRAME              (LIBAVUTIL_VERSION_MAJOR < 56)
#endif
#ifndef FF_API_CRC_BIG_TABLE
#define FF_API_CRC_BIG_TABLE            (LIBAVUTIL_VERSION_MAJOR < 56)
#endif


/**
 * @}
 */

#endif /* AVUTIL_VERSION_H */
