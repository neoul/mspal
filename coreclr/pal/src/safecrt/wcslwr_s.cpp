// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

/***
*wcslwr_s.cpp - contains _wcslwr_s() routine
*
*
*Purpose:
*   _wcslwr_s converts, in place, any upper case letters in input string to
*   lowercase.
*
*******************************************************************************/

#include <wctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "internal_securecrt.h"

#include "mbusafecrt_internal.h"

#if defined(WCHAR_4BYTES)
DLLEXPORT errno_t __cdecl _wcslwr_s(char32_t *string, size_t sz)
#else
DLLEXPORT errno_t __cdecl _wcslwr_s(char16_t *string, size_t sz)
#endif
{
    _VALIDATE_RETURN_ERRCODE(string != NULL, EINVAL);
    size_t length = PAL_wcsnlen(string, sz);
    if (length >= sz)
    {
        _RETURN_DEST_NOT_NULL_TERMINATED(string, sz);
    }

    for (int i = 0; string[i] != 0; i++)
    {
#if defined(WCHAR_4BYTES)
DLLEXPORT errno_t __cdecl _wcslwr_s(char32_t *string, size_t sz)
        string[i] = (char32_t)towlower(string[i]);
#else
        string[i] = (char16_t)towlower(string[i]);
#endif
    }

    _FILL_STRING(string, sz, length + 1);

    return 0;
}
