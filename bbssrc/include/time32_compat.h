/*
 * time32_compat.h - Force 32-bit time_t for MIPS cross-compilation
 * 
 * This header ensures compatibility with x86 32-bit .PASSWDS files
 * which use 32-bit time_t, long, and pointers.
 * 
 * MUST be included BEFORE any system headers that use time_t.
 */

#ifndef _TIME32_COMPAT_H
#define _TIME32_COMPAT_H

/* Prevent system from defining time_t */
#define __time_t_defined 1
#define _TIME_T
#define __TIME_T_TYPE int

/* Define our own 32-bit time_t */
#include <stdint.h>
typedef int32_t time_t;

/* Also ensure we have 32-bit long if needed */
_Static_assert(sizeof(time_t) == 4, "time_t must be 32-bit for compatibility");
_Static_assert(sizeof(int) == 4, "int must be 32-bit");

#endif /* _TIME32_COMPAT_H */
