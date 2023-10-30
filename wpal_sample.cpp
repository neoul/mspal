#include <iostream>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include <wpal.h>

// namespace _com_util {
//   inline void CheckError(HRESULT hr) {
//     if(FAILED(hr)) { _com_issue_error(hr); }
//   }
// }

// namespace _com_util {
//   BSTR WINAPI ConvertStringToBSTR(const char *pSrc);
//   char *WINAPI ConvertBSTRToString(BSTR pSrc);
// }

// using namespace std;

// void use_bstr_t() {
//     _bstr_t bstr1("Hello");
//     _bstr_t bstr2("World");
//     _bstr_t bstr3 = bstr1 + bstr2;
//     cout << bstr3 << endl;
// }

int main()
{

    std::cout << "BSTR" << " " << sizeof(BSTR) << std::endl; // 8
    std::cout << "tagDEC" << " " << sizeof(tagDEC) << std::endl; // 1
    std::cout << "wchar_t" << " " << sizeof(wchar_t) << std::endl; // 8
    std::cout << sizeof(long) << std::endl; // 8
    std::cout << sizeof(int) << std::endl; // 4
    SYSTEMTIME TheTime;
    GetSystemTime(&TheTime);
    std::cout << std::string("Hello, CMake world! ") << std::to_string(TheTime.wYear) << std::endl;
}