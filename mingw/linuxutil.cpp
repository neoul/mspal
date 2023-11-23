#include "linuxutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <cwchar>

long _findfirst(const char *path, _finddata_t *file)
{
    DIRHANDLE *dhandle = (DIRHANDLE *)(malloc(sizeof(DIRHANDLE)));
    if (!dhandle)
        return -1;

    dhandle->dir = opendir(path);
    if (!dhandle->dir)
    {
        free(dhandle);
        return -1;
    }

    dhandle->entry = readdir(dhandle->dir);
    if (!dhandle->entry)
    {
        closedir(dhandle->dir);
        free(dhandle);
        return -1;
    }

    file->name = strdup(dhandle->entry->d_name);
    // Populate other fields of file as necessary

    // Return a pointer to our DIRHANDLE as the "handle"
    return (long)dhandle;
}

int _findnext(long handle, _finddata_t *file)
{
    DIRHANDLE *dhandle = (DIRHANDLE *)handle;

    dhandle->entry = readdir(dhandle->dir);
    if (!dhandle->entry)
        return -1;

    free(file->name); // Free the previous name
    file->name = strdup(dhandle->entry->d_name);
    // Populate other fields of file as necessary

    return 0;
}

int _findclose(long handle)
{
    DIRHANDLE *dhandle = (DIRHANDLE *)handle;
    if (dhandle)
    {
        if (dhandle->dir)
            closedir(dhandle->dir);
        free(dhandle);
    }
    return 0;
}

time_t _time32(time_t *__timer)
{
    return time(__timer);
}

errno_t _itoa_s(int value, char *buffer, size_t size, int radix)
{
    if (buffer == NULL || size == 0 || radix < 2 || radix > 36)
    {
        return EINVAL; // Invalid argument
    }

    // Use snprintf to convert the integer to a string with the specified radix
    int ret = snprintf(buffer, size, (radix == 10) ? "%d" : "%x", value);

    if (ret < 0)
    {
        return EINVAL; // Encoding error
    }
    else if ((size_t)ret >= size)
    {
        return ERANGE; // Buffer too small
    }

    return 0; // Success
}

errno_t strcpy_s(char *dest, rsize_t destsz, const char *src)
{
    if (dest == NULL || src == NULL)
    {
        return EINVAL;
    }
    if (destsz == 0 || destsz > RSIZE_MAX)
    {
        return ERANGE;
    }

    while (*src != '\0' && destsz > 1)
    {
        *dest++ = *src++;
        --destsz;
    }

    if (destsz == 0)
    {
        *dest = '\0'; // Null terminate, could be error condition
        return ERANGE;
    }

    *dest = '\0'; // Null terminate
    return 0;
}

char *_itoa(int value, char *str, int base)
{
    if (str == NULL)
    {
        return NULL; // str must be a valid pointer
    }
    if (base < 2 || base > 36)
    {
        return NULL; // base must be between 2 and 36
    }

    sprintf(str, "%d", value); // For simplicity, using %d for all bases
    return str;
}

int _wtoi(const wchar_t *str)
{
    return static_cast<int>(wcstol(str, nullptr, 10));
}

int64_t _wtoi64(const wchar_t *str)
{
    return wcstoll(str, nullptr, 10);
}

int64_t _wtoll(const wchar_t *str)
{
    return wcstoll(str, nullptr, 10);
}

DWORD GetPrivateProfileStringA(
    LPCSTR lpAppName,
    LPCSTR lpKeyName,
    LPCSTR lpDefault,
    LPSTR lpReturnedString,
    DWORD nSize,
    LPCSTR lpFileName)
{
    FILE *file = fopen(lpFileName, "r");
    if (!file)
    {
        strncpy(lpReturnedString, lpDefault, nSize);
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    BOOL inSection = FALSE;
    DWORD result = 0;

    while ((read = getline(&line, &len, file)) != -1)
    {
        // Trim newline character
        if (read > 0 && line[read - 1] == '\n')
        {
            line[--read] = '\0';
        }

        if (line[0] == '[')
        {
            inSection = (strncmp(line + 1, lpAppName, strlen(lpAppName)) == 0) && (line[strlen(lpAppName) + 1] == ']');
        }
        else if (inSection && strncmp(line, lpKeyName, strlen(lpKeyName)) == 0 && line[strlen(lpKeyName)] == '=')
        {
            char *value = line + strlen(lpKeyName) + 1;
            strncpy(lpReturnedString, value, nSize);
            lpReturnedString[nSize - 1] = '\0';
            result = strlen(lpReturnedString);
            break;
        }
    }

    if (result == 0)
    {
        strncpy(lpReturnedString, lpDefault, nSize);
    }

    fclose(file);
    free(line);
    return result;
}

UINT GetPrivateProfileIntA(
    LPCSTR lpAppName,
    LPCSTR lpKeyName,
    INT nDefault,
    LPCSTR lpFileName)
{
    char valueStr[64];
    if (GetPrivateProfileStringA(lpAppName, lpKeyName, "", valueStr, sizeof(valueStr), lpFileName) > 0)
    {
        return atoi(valueStr);
    }
    return nDefault;
}

DWORD GetModuleFileNameA(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize)
{
    ssize_t count = readlink("/proc/self/exe", lpFilename, nSize);
    if (count != -1)
    {
        lpFilename[count] = '\0';
        return count;
    }
    return 0;
}

BOOL PathRemoveFileSpecA(
    LPSTR pszPath)
{
    char *lastSlash = strrchr(pszPath, '/');
    if (lastSlash && lastSlash != pszPath)
    {
        *lastSlash = '\0';
        return TRUE;
    }
    else if (lastSlash == pszPath)
    {
        // Handle root directory ("/") case
        pszPath[1] = '\0';
        return TRUE;
    }
    return FALSE;
}
