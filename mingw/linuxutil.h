#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <pal_mstypes.h>

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

time_t _time32(time_t *__timer);

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