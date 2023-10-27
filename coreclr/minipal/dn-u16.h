// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include <stddef.h>
#include <stdint.h>

//
// Wide character (UTF-16) abstraction layer.
//

size_t wchar_strlen(const WCHAR* str);
int wchar_strcmp(const WCHAR* str1, const WCHAR* str2);
int wchar_strncmp(const WCHAR* str1, const WCHAR* str2, size_t count);
WCHAR* wchar_strcat_s(WCHAR* dst, size_t dstLen, const WCHAR* src);
WCHAR* wchar_strcpy_s(WCHAR* dst, size_t dstLen, const WCHAR* src);
WCHAR* wchar_strncpy_s(WCHAR* dst, size_t dstLen, const WCHAR* src, size_t count);
const WCHAR* wchar_strstr(const WCHAR* str, const WCHAR* strCharSet);
const WCHAR* wchar_strchr(const WCHAR* str, WCHAR ch);
const WCHAR* wchar_strrchr(const WCHAR* str, WCHAR ch);
uint32_t wchar_strtoul(const WCHAR* nptr, WCHAR** endptr, int base);
uint64_t wchar_strtoui64(const WCHAR* nptr, WCHAR** endptr, int base);
double wchar_strtod(const WCHAR* nptr, WCHAR** endptr);