#include <iostream>
// #include <comdef.h>

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
    std::cout << sizeof(wchar_t) << std::endl; // 8
    std::cout << sizeof(long) << std::endl; // 8
    std::cout << sizeof(int) << std::endl; // 4
    // SYSTEMTIME TheTime;
    // GetSystemTime(&TheTime);
    // std::cout << std::string("Hello, CMake world! ") << std::to_string(TheTime.wYear) << std::endl;
}