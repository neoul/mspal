#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <fstream>
#include <pal.h>

bool PathRemoveFileSpecA(char *pszPath);
int GetModuleFileNameA(char *lpFileName, int nSize);
int GetPrivateProfileIntA(const std::string &section, const std::string &key, int defaultValue, const std::string &fileName);
int GetPrivateProfileStringA(const char *fileName, const char *section, const char *key, const char *defaultValue, char *buffer, int bufferSize);

std::map<std::string, std::map<std::string, std::string>> ParseINIFile(const std::string &filename);
std::string GetINIValue(const std::map<std::string, std::map<std::string, std::string>> &iniData,
                        const std::string &section, const std::string &key,
                        const std::string &defaultValue);