#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <pal_mstypes.h>
#include <pal.h>

#ifndef RSIZE_MAX
#define RSIZE_MAX (SIZE_MAX >> 1)
#endif

typedef int errno_t;
typedef size_t rsize_t;

typedef struct _TIME_ZONE_INFORMATION
{
    LONG Bias;
    WCHAR StandardName[32];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

typedef struct
{
    char *name;      // Filename
    off_t size;      // File size
    unsigned attrib; // File attributes
    // Add other fields as necessary
} _finddata_t;

typedef struct
{
    DIR *dir;
    struct dirent *entry;
} DIRHANDLE;

long _findfirst(const char *path, _finddata_t *file);
int _findnext(long handle, _finddata_t *file);
int _findclose(long handle);

DWORD GetTimeZoneInformation(TIME_ZONE_INFORMATION *pTimeZoneInformation);

time_t _time32(time_t *__timer);
errno_t _itoa_s(int value, char *buffer, size_t size, int radix);
errno_t strcpy_s(char *dest, rsize_t destsz, const char *src);
char *_itoa(int value, char *str, int base);

DWORD GetPrivateProfileStringA(
    LPCSTR lpAppName,
    LPCSTR lpKeyName,
    LPCSTR lpDefault,
    LPSTR lpReturnedString,
    DWORD nSize,
    LPCSTR lpFileName);

UINT GetPrivateProfileIntA(
    LPCSTR lpAppName,
    LPCSTR lpKeyName,
    INT nDefault,
    LPCSTR lpFileName);

DWORD GetModuleFileNameA(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize);

BOOL PathRemoveFileSpecA(
    LPSTR pszPath);