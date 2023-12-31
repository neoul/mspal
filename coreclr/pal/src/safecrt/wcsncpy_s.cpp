// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

/***
*wcsncpy_s.c - copy at most n characters of wide-character string
*

*
*Purpose:
*   defines wcsncpy_s() - copy at most n characters of char16_t string
*
*******************************************************************************/

#define _SECURECRT_FILL_BUFFER 1
#define _SECURECRT_FILL_BUFFER_THRESHOLD ((size_t)8)

#include <string.h>
#include <errno.h>
#include <limits.h>
#include "internal_securecrt.h"

#include "mbusafecrt_internal.h"

#define _FUNC_PROLOGUE
#define _FUNC_NAME wcsncpy_s
#if defined(WCHAR_4BYTES)
#define _CHAR char32_t
#else
#define _CHAR char16_t
#endif
#define _DEST _Dst
#define _SIZE _SizeInWords
#define _SRC _Src
#define _COUNT _Count

#include "tcsncpy_s.inl"

