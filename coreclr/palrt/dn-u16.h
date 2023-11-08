// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include <stddef.h>
#include <stdint.h>

//
// Wide character (UTF-16) abstraction layer.
//

size_t u16_strlen(const char16_t* str);
int u16_strcmp(const char16_t* str1, const char16_t* str2);
int u16_strncmp(const char16_t* str1, const char16_t* str2, size_t count);
char16_t* u16_strcat_s(char16_t* dst, size_t dstLen, const char16_t* src);
char16_t* u16_strcpy_s(char16_t* dst, size_t dstLen, const char16_t* src);
char16_t* u16_strncpy_s(char16_t* dst, size_t dstLen, const char16_t* src, size_t count);
const char16_t* u16_strstr(const char16_t* str, const char16_t* strCharSet);
const char16_t* u16_strchr(const char16_t* str, char16_t ch);
const char16_t* u16_strrchr(const char16_t* str, char16_t ch);
uint32_t u16_strtoul(const char16_t* nptr, char16_t** endptr, int base);
uint64_t u16_strtoui64(const char16_t* nptr, char16_t** endptr, int base);
double u16_strtod(const char16_t* nptr, char16_t** endptr);