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

#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/ODBC/Connector.h>
#include <wpal.h>

using namespace std;

void use_bstr_t() {
    
    _bstr_t bstr1("Hello ");
    cout << "bstr1 " << bstr1 << endl;
    _bstr_t bstr2("World");
    cout << "bstr2 " << bstr2 << endl;
    _bstr_t bstr3 = bstr1 + bstr2;
    cout << "bstr3 " << bstr3 << endl;
}

int main()
{
    setlocale(LC_ALL, "");
    // cout << "BSTR" << " " << sizeof(BSTR) << endl; // 8
    // cout << "tagDEC" << " " << sizeof(tagDEC) << endl; // 1
    cout << "wchar_t" << " " << sizeof(wchar_t) << endl; // 8
    // cout << sizeof(long) << endl; // 8
    // cout << sizeof(int) << endl; // 4
    SYSTEMTIME TheTime;
    GetSystemTime(&TheTime);
    cout << string("Hello, CMake world! ") << to_string(TheTime.wYear) << endl;
    use_bstr_t();
    use_var();
    Poco::Dynamic::Var var("Hello");
    std::cout << "var 2" << var.toString() << std::endl;
}