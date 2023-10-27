// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include <Windows.h>
#include <wchar.h>

#include <dn-u16.h>

size_t wchar_strlen(const WCHAR* str)
{
    return ::wcslen(str);
}

int wchar_strcmp(const WCHAR* str1, const WCHAR* str2)
{
    return ::wcscmp(str1, str2);
}

int wchar_strncmp(const WCHAR* str1, const WCHAR* str2, size_t count)
{
    return ::wcsncmp(str1, str2, count);
}

WCHAR* wchar_strcpy_s(WCHAR* dst, size_t dstLen, const WCHAR* src)
{
    if (0 != ::wcscpy_s(dst, dstLen, src))
        return nullptr;
    return dst;
}

WCHAR* wchar_strcat_s(WCHAR* dst, size_t dstLen, const WCHAR* src)
{
    if (0 != ::wcscat_s(dst, dstLen, src))
        return nullptr;
    return dst;
}

WCHAR* wchar_strncpy_s(WCHAR* dst, size_t dstLen, const WCHAR* src, size_t count)
{
    if (0 != ::wcsncpy_s(dst, dstLen, src, count))
        return nullptr;
    return dst;
}

const WCHAR* wchar_strstr(const WCHAR *str, const WCHAR *strCharSet)
{
    return ::wcsstr(str, strCharSet);
}

const WCHAR* wchar_strchr(const WCHAR* str, WCHAR ch)
{
    return ::wcschr(str, ch);
}

const WCHAR* wchar_strrchr(const WCHAR* str, WCHAR ch)
{
    return ::wcsrchr(str, ch);
}

uint32_t wchar_strtoul(const WCHAR* nptr, WCHAR** endptr, int base)
{
    return ::wcstoul(nptr, endptr, base);
}

uint64_t wchar_strtoui64(const WCHAR* nptr, WCHAR** endptr, int base)
{
    return ::_wcstoui64(nptr, endptr, base);
}

double wchar_strtod(const WCHAR* nptr, WCHAR** endptr)
{
    return ::wcstod(nptr, endptr);
}