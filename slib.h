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

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
    
#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float r32;
typedef double r64;
typedef u32 b32;

#define internal static
#define persist static
#define global static

#define pi32 3.14159265359f

#ifndef WIN32
#define LONG "%lld"
#else
#define LONG "%I64u"
#endif

#if SLOW
#define Crash {*(volatile u32 *)0 = 0;}
#define Assert(x) if(!(x)) {                                        \
        fprintf(stderr, "Assert Failed:\n%s\nFILE:%s\nLINE:%d\n", \
                #x, \
                __FILE__, \
                __LINE__); \
        exit(0); \
}
#else
#define Assert(x)
#define Crash
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))
#define Minimum(x,y) ((x) < (y) ? (x) : (y))
#define Maximum(x,y) ((x) > (y) ? (x) : (y))
// TODO(steven): swap. maybe xor swap?


//My own atoi. Theoretically faster? I should test it.
internal u32
sg_atoi(const char *s)
{
    u32 result = 0;

    while(*s)
    {
        result <<= 1;
        result += (result << 2);
        result += (*s++ & 0x0f);
    }

    return result;
}

/*
  This is equivolent to a vector in C++ and is straight from Sean Barrett's
  stretchy-buffer in his stb libs. (Literally, this is exactly his code with only
  name difference)
      
  sg_buffer usage:

  a_struct_of_whatevs *GrowieBuffer

  for(;;)
  {
    a_struct_of_whatevs NewStruct;
    NewStruct.field1 = x;
    NewStruct.field2 = y;

    buffer_push(GrowieBuffer, NewStruct);
  }
*/
    
#define buffer_free(a)       ((a) ? free(buffer_raw(a)),0 : 0)
#define buffer_push(a,v)     (buffer_maybe_grow(a,1), (a)[buffer_used(a)++] = (v))
#define buffer_count(a)      ((a) ? buffer_used(a) : 0)
#define buffer_grow(a,n)     (buffer_maybe_grow(a,n), buffer_used(a)+=(n),&(a)[buffer_used(a)-(n)])
#define buffer_last(a)       ((a)[buffer_used(a)-1])

#define buffer_raw(a)        ((s32 *) (a) - 2)
#define buffer_size(a)       buffer_raw(a)[0]
#define buffer_used(a)       buffer_raw(a)[1]

#define buffer_test(a,n)     ((a)==0 || buffer_used(a)+(n) >= buffer_size(a))
#define buffer_maybe_grow(a,n) (buffer_test(a,(n)) ? buffer_expand(a,n) : 0)
#define buffer_expand(a,n)   ((a) = sg_grow_buffer((a), (n), sizeof(*(a))))
    
internal void *
sg_grow_buffer(void *Array, s32 SizeIncrease, u32 ItemSize)
{
    void *Result = 0;
    
    u32 DoubleCurrentSize = Array ? 2*buffer_size(Array) : 0;
    u32 MinimumNewSize = buffer_count(Array) + SizeIncrease;
    u32 NewSize = ((DoubleCurrentSize > MinimumNewSize)
                   ? DoubleCurrentSize : MinimumNewSize);
    u32 *p = (u32 *)realloc(Array ? buffer_raw(Array) : 0,
                            ItemSize*NewSize + sizeof(u32)*2);
    if(p)
    {
        if(!Array)
        {
            p[1] = 0;
        }
        p[0] = NewSize;

        Result = p+2;
    }
    else
    {
        InvalidCodePath;
    }

    return Result;
}

internal void
Quicksort_(u32 *Array, s32 First, s32 Last)
{
    s32 Pivot, I, J;
    u32 Temp;

    if(First < Last)
    {
        Pivot = First;
        I = First;
        J = Last;

        while(I < J)
        {
            while(Array[I] <= Array[Pivot] && I < Last)
            {
                ++I;
            }
            while(J > Pivot && Array[J] <= Array[Pivot])
            {
                --J;
            }
            if(I < J)
            {
                Temp = Array[I];
                Array[I] = Array[J];
                Array[J] = Temp;
            }
        }

        Temp = Array[Pivot];
        Array[Pivot] = Array[J];
        Array[J] = Temp;
        Quicksort_(Array, First, J-1);
        Quicksort_(Array, J+1, Last);
    }
}

internal void
Quicksort(u32 *Array, u32 Size)
{
    Quicksort_(Array, 0, Size-1);
}
    
#ifdef __cplusplus
}
#endif
    
#define SGG_H
#endif
