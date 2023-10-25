#include "potato.h"

#define MAX_LINE_LENGTH 1024

bool PathRemoveFileSpecA(char *pszPath)
{
    if (pszPath == nullptr)
    {
        return false;
    }

    char *lastBackslash = strrchr(pszPath, '\\');
    char *lastForwardSlash = strrchr(pszPath, '/');

    char *lastSeparator = (lastBackslash > lastForwardSlash) ? lastBackslash : lastForwardSlash;

    if (lastSeparator != nullptr)
    {
        // Replace the separator with a null terminator to remove the file component
        *lastSeparator = '\0';
        return true;
    }

    return false; // No separator found, path is a filename or in the root directory
}

int GetModuleFileNameA(char *lpFileName, int nSize)
{
    if (nSize == 0)
    {
        return 0; // Buffer size is zero, nothing to copy.
    }

    int len = readlink("/proc/self/exe", lpFileName, nSize - 1);
    if (len != -1)
    {
        lpFileName[len] = '\0'; // Null-terminate the string.
        return len;             // Return the length of the string copied.
    }
    else
    {
        return 0; // Error
    }
}

int GetPrivateProfileIntA(const std::string &section, const std::string &key, int defaultValue, const std::string &fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        return defaultValue;
    }

    std::string line;
    std::string currentSection;
    bool sectionFound = false;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == ';')
        {
            // Skip empty lines and comments
            continue;
        }

        if (line[0] == '[' && line[line.size() - 2] == ']')
        {
            // This is a section header
            currentSection = line.substr(1, line.size() - 3);
            sectionFound = (currentSection == section);
        }
        else if (sectionFound)
        {
            // Check if the line contains the desired key
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string currentKey = line.substr(0, delimiterPos);
                if (currentKey == key)
                {
                    try
                    {
                        // Attempt to convert the value to an integer
                        return std::stoi(line.substr(delimiterPos + 1));
                    }
                    catch (std::invalid_argument &)
                    {
                        // Conversion failed, return the default value
                        return defaultValue;
                    }
                }
            }
        }
    }

    return defaultValue;
}

// int GetPrivateProfileStringA(const char *section, const char *key, const char *defaultValue, char *buffer, int bufferSize, const char *fileName)
// {
//     FILE *file = fopen(fileName, "r");
//     if (file == NULL)
//     {
//         strncpy(buffer, defaultValue, bufferSize);
//         return strlen(defaultValue);
//     }

//     char line[MAX_LINE_LENGTH];
//     char currentSection[256] = ""; // Adjust the size as needed
//     int inDesiredSection = 0;

//     while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
//     {
//         // Remove trailing newline character
//         char *newline = strchr(line, '\n');
//         if (newline)
//             *newline = '\0';

//         if (line[0] == '[' && line[strlen(line) - 1] == ']')
//         {
//             // This is a section header
//             inDesiredSection = (strcmp(line, section) == 0);
//             continue;
//         }

//         if (inDesiredSection)
//         {
//             char currentKey[MAX_LINE_LENGTH];
//             char value[MAX_LINE_LENGTH];

//             if (sscanf(line, "%[^=]=%[^\n]", currentKey, value) == 2)
//             {
//                 if (strcmp(currentKey, key) == 0)
//                 {
//                     fclose(file);
//                     strncpy(buffer, value, bufferSize);
//                     return strlen(value);
//                 }
//             }
//         }
//     }

//     fclose(file);
//     strncpy(buffer, defaultValue, bufferSize);
//     return strlen(defaultValue);
// }

// // Function to parse an INI file and store its contents in a map
// std::map<std::string, std::map<std::string, std::string>> ParseINIFile(const std::string &filename)
// {
//     std::ifstream file(filename);
//     if (!file.is_open())
//     {
//         std::cerr << "Error: Unable to open the INI file." << std::endl;
//         return {};
//     }

//     std::map<std::string, std::map<std::string, std::string>> iniData;
//     std::string currentSection;

//     for (std::string line; std::getline(file, line);)
//     {
//         line = line.substr(0, line.find(';'));             // Remove comments
//         line = line.substr(line.find_first_not_of(" \t")); // Trim leading whitespace
//         if (line.empty())
//             continue;

//         if (line[0] == '[' && line.back() == ']')
//         {
//             currentSection = line.substr(1, line.length() - 2);
//         }
//         else
//         {
//             size_t delimiterPos = line.find('=');
//             if (delimiterPos != std::string::npos)
//             {
//                 std::string key = line.substr(0, delimiterPos);
//                 std::string value = line.substr(delimiterPos + 1);
//                 iniData[currentSection][key] = value;
//             }
//         }
//     }

//     return iniData;
// }

// // Function to get the value of a key within a section
// std::string GetINIValue(const std::map<std::string, std::map<std::string, std::string>> &iniData,
//                         const std::string &section, const std::string &key,
//                         const std::string &defaultValue)
// {
//     auto sectionIter = iniData.find(section);
//     if (sectionIter != iniData.end())
//     {
//         auto keyIter = sectionIter->second.find(key);
//         if (keyIter != sectionIter->second.end())
//         {
//             return keyIter->second;
//         }
//     }
//     return defaultValue;
// }