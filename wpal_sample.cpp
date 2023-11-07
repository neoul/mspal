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
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <codecvt>
#include <typeinfo>
#include <locale>

#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/ODBC/Connector.h>
#include <wpal.h>

using namespace std;
using namespace Poco::Dynamic;

void use_bstr_t()
{

    _bstr_t bstr1("Hello ");
    cout << "bstr1 " << bstr1 << endl;
    _bstr_t bstr2("World");
    cout << "bstr2 " << bstr2 << endl;
    _bstr_t bstr3 = bstr1 + bstr2;
    cout << "bstr3 " << bstr3 << endl;
    auto bstr4 = static_cast<const char *>(bstr1);
}

int main()
{
    setlocale(LC_ALL, "");
    // cout << "BSTR" << " " << sizeof(BSTR) << endl; // 8
    // cout << "tagDEC" << " " << sizeof(tagDEC) << endl; // 1
    cout << "wchar_t"
         << " " << sizeof(wchar_t) << endl; // 8
    cout << "char16_t"
         << " " << sizeof(char16_t) << endl; // 8
    cout << "char32_t"
         << " " << sizeof(char32_t) << endl; // 8
    // cout << sizeof(long) << endl; // 8
    // cout << sizeof(int) << endl; // 4
    SYSTEMTIME TheTime;
    GetSystemTime(&TheTime);
    cout << string("Hello, CMake world! ") << to_string(TheTime.wYear) << endl;
    use_bstr_t();
    use_var();
    Var var("Hello");
    std::cout << "var 2" << var.toString() << std::endl;

    _variant_t v1("안녕");
    const std::string s = v1;
    std::cout << "s " << s << std::endl;
    // std::wcout << "v1 " << static_cast<const wchar_t *>(v1) << std::endl;

    _variant_t v2(s);
    std::cout << "v2 " << static_cast<std::string>(v2) << std::endl;
    _variant_t v3(L"안녕하세요 wchar to string");
    std::cout << "v3 " << static_cast<std::string>(v3) << std::endl;
    v3 = L"안녕!";
    std::cout << "v3 (std::string) " << static_cast<std::string>(v3) << std::endl;
    std::cout << "v3 (const char *) " << static_cast<const char *>(v3) << std::endl;
    // std::wcout << "v3 (const wchar_t *) " << static_cast<const wchar_t *>(v3) << std::endl;
    const wchar_t *v3_wchar = static_cast<const wchar_t *>(v3);
    _variant_t v3_1(v3_wchar);
    std::cout << "v3_1 " << static_cast<std::string>(v3_1) << std::endl;

    _variant_t v4(v3);
    std::cout << "v4 " << static_cast<std::string>(v4) << std::endl;
    _variant_t v5 = v4;
    std::cout << "v5 " << static_cast<std::string>(v5) << std::endl;

    _bstr_t bstr1(L"Basic STRing");
    _bstr_t bstr2(L"Basic STRing 2");
    _variant_t v6(bstr1);
    std::cout << "v6 " << static_cast<std::string>(v6) << std::endl;
    v6 = bstr2;
    std::cout << "v6 " << static_cast<std::string>(v6) << std::endl;

    _variant_t v7 = 15;
    std::cout << "v7 " << static_cast<int>(v7) << std::endl;

    _variant_t v8 = Var(0.314);
    std::cout << "v8 " << static_cast<double>(v8) << std::endl;
    // _variant_t v3 = v1 + v2;
    // cout << "v3 " << v3.toString() << endl;

    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
    // UTF-8 to UTF-16
    // UTF-16은 locale에 따라 다르게 동작...
    std::u16string utf16 = cvt.from_bytes("한글");
    std::cout << "UTF-16 string size: " << utf16.size() << '\n';
    std::cout << "converted() == " << cvt.converted() << '\n';
    // UTF-16 to UTF-8
    auto _utf8 = cvt.to_bytes(utf16);
    std::cout << "new UTF-8 string size: " << _utf8.size() << '\n';
    std::cout << "converted() == " << cvt.converted() << '\n';
}