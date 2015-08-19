#if !defined(SGG_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

    typedef int32_t s32;
    typedef int64_t s64;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef float r32;
    typedef double r64;
    typedef u32 b32;

#define internal static
#define persist static
#define global static

#define pi32 3.14159265359f

#if SLOW
// TODO(steven): Complete assertion macro
#define assert(x) if(!(x)) {*(int *)0 = 0;}
#else
#define assert(x)
#endif

#define InvalidCodePath assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))
#define Minimum(x,y) ((x) < (y) ? (x) : (y))
#define Maximum(x,y) ((x) > (y) ? (x) : (y))
// TODO(steven): swap. maybe xor swap?


#ifdef __cplusplus
}
#endif
    
#define SGG_H
#endif
