// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include <dn-u16.h>
#include <string.h>

size_t u16_strlen(const char16_t* str)
{
    size_t nChar = 0;
    while (*str++)
        nChar++;
    return nChar;
}

int u16_strcmp(const char16_t* str1, const char16_t* str2)
{
    return u16_strncmp(str1, str2, 0x7fffffff);
}

int u16_strncmp(const char16_t* str1, const char16_t* str2, size_t count)
{
    int diff = 0;
    for (size_t i = 0; i < count; i++)
    {
        diff = str1[i] - str2[i];
        if (diff != 0)
            break;

        // stop if we reach the end of the string
        if(str1[i] == (char16_t)'\0')
            break;
    }
    return diff;
}

char16_t* u16_strcat_s(char16_t* dst, size_t dstLen, const char16_t* src)
{
    if (dst == nullptr || src == nullptr)
    {
        return nullptr;
    }

    char16_t* start = dst;
    char16_t* end = dst + dstLen;

    // find end of source string
    while (*dst)
    {
        dst++;
        if (dst >= end)
            return nullptr;
    }

    // concatenate new string
    size_t srcLength = u16_strlen(src);
    size_t loopCount = 0;
    while (*src && loopCount < srcLength)
    {
        *dst++ = *src++;
        if (dst >= end)
            return nullptr;
        loopCount++;
    }

    // add terminating null
    *dst = (char16_t)'\0';
    return start;
}

char16_t* u16_strcpy_s(char16_t* dst, size_t dstLen, const char16_t* src)
{
    if (dst == nullptr || src == nullptr)
    {
        return nullptr;
    }

    char16_t* start = dst;
    char16_t* end = dst + dstLen;

    // copy source string to destination string
    while (*src)
    {
        *dst++ = *src++;
        if (dst >= end)
            return nullptr;
    }

    // add terminating null
    *dst = (char16_t)'\0';
    return start;
}

char16_t* u16_strncpy_s(char16_t* dst, size_t dstLen, const char16_t* src, size_t count)
{
    ::memset(dst, 0, dstLen * sizeof(char16_t));

    size_t srcLength = u16_strlen(src);
    size_t length = (count < srcLength) ? count : srcLength;
    if (length > dstLen)
        return nullptr;

    ::memcpy(dst, src, length * sizeof(char16_t));
    return dst;
}

const char16_t* u16_strstr(const char16_t *str, const char16_t *strCharSet)
{
    if (str == nullptr || strCharSet == nullptr)
    {
        return nullptr;
    }

    // No characters to examine
    if (u16_strlen(strCharSet) == 0)
        return str;

    const char16_t* ret = nullptr;
    int i;
    while (*str != (char16_t)'\0')
    {
        i = 0;
        while (true)
        {
            if (*(strCharSet + i) == (char16_t)'\0')
            {
                ret = str;
                goto LEAVE;
            }
            else if (*(str + i) == (char16_t)'\0')
            {
                ret = nullptr;
                goto LEAVE;
            }
            else if (*(str + i) != *(strCharSet + i))
            {
                break;
            }
            i++;
        }
        str++;
    }
 LEAVE:
    return ret;
}

const char16_t* u16_strchr(const char16_t* str, char16_t ch)
{
    while (*str)
    {
        if (*str == ch)
            return str;
        str++;
    }

    // Check if the comparand was \000
    if (*str == ch)
        return str;

    return nullptr;
}

const char16_t* u16_strrchr(const char16_t* str, char16_t ch)
{
    const char16_t* last = nullptr;
    while (*str)
    {
        if (*str == ch)
            last = str;
        str++;
    }

    return last;
}

// Forward declare PAL function
extern "C" uint32_t PAL_wcstoul(const char16_t* nptr, char16_t** endptr, int base);
extern "C" uint64_t PAL__wcstoui64(const char16_t* nptr, char16_t** endptr, int base);
extern "C" double PAL_wcstod(const char16_t* nptr, char16_t** endptr);

uint32_t u16_strtoul(const char16_t* nptr, char16_t** endptr, int base)
{
    return PAL_wcstoul(nptr, endptr, base);
}

uint64_t u16_strtoui64(const char16_t* nptr, char16_t** endptr, int base)
{
    return PAL__wcstoui64(nptr, endptr, base);
}

double u16_strtod(const char16_t* nptr, char16_t** endptr)
{
    return PAL_wcstod(nptr, endptr);
}