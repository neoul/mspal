#include "ini_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DWORD GetPrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault,
                               LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
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

UINT GetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
{
    char valueStr[64];
    if (GetPrivateProfileStringA(lpAppName, lpKeyName, "", valueStr, sizeof(valueStr), lpFileName) > 0)
    {
        return atoi(valueStr);
    }
    return nDefault;
}