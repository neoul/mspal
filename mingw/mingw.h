#ifndef __MINGW_H__
#define __MINGW_H__

#ifndef __LP64__	/* 32 bit target, 64 bit Mingw target */
#define __LONG32 long
#else			/* 64 bit Cygwin target */
#define __LONG32 int
#endif

#ifdef __GNUC__
#undef __int8
#define __int8 char
#undef __int16
#define __int16 short
#undef __int32
#define __int32 int
#undef __int64
#define __int64 long long
#endif

#undef __MINGW_EXTENSION

#if defined(__GNUC__) || defined(__GNUG__)
#define __MINGW_EXTENSION __extension__
#else
#define __MINGW_EXTENSION
#endif

#endif // __MINGW_H__