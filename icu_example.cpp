#include <unicode/unistr.h>   // ICU's UnicodeString
#include <unicode/utypes.h>   // Basic ICU types
#include <unicode/translit.h> // For Transliterator
#include <iostream>
#include <memory>
#include <unicode/ucnv.h>
#include <unicode/utrans.h>
#include <string>
#include <vector>

// Define types for compatibility with Windows API
using LCID = uint32_t;
using DWORD = uint32_t;
using LPCWSTR = const wchar_t *;
using LPWSTR = wchar_t *;

// Flags (mock values for Linux)
constexpr DWORD LCMAP_FULLWIDTH = 0x00800000;
constexpr DWORD LCMAP_KATAKANA = 0x00200000;

int LCMapStringW(
    LCID Locale,
    DWORD dwMapFlags,
    LPCWSTR lpSrcStr,
    int cchSrc,
    LPWSTR lpDestStr,
    int cchDest)
{
    // Convert input wide string to ICU's UnicodeString
    icu::UnicodeString srcStr(reinterpret_cast<const UChar *>(lpSrcStr), cchSrc);

    // Create a transliterator based on the flags
    UErrorCode status = U_ZERO_ERROR;
    std::unique_ptr<icu::Transliterator> trans;

    if (dwMapFlags & LCMAP_FULLWIDTH)
    {
        trans.reset(icu::Transliterator::createInstance("Halfwidth-Fullwidth", UTRANS_FORWARD, status));
    }
    else if (dwMapFlags & LCMAP_KATAKANA)
    {
        trans.reset(icu::Transliterator::createInstance("Hiragana-Katakana", UTRANS_FORWARD, status));
    }

    if (U_FAILURE(status) || !trans)
    {
        return 0; // Transliteration creation failed
    }

    // Perform the transliteration
    trans->transliterate(srcStr);

    // Copy result to destination string
    srcStr.extract(reinterpret_cast<UChar *>(lpDestStr), cchDest, status);

    // return 0;
    return U_SUCCESS(status) ? srcStr.length() : 0;
}

std::string convert_wchars_to_string(const wchar_t *pVal);

int main()
{
    // const wchar_t source[] = L"あいうえお"; // Example source in Hiragana
    const wchar_t source[] = L"は゛ば"; // Example source in Hiragana
    wchar_t destination[10];

    int result = LCMapStringW(0, LCMAP_FULLWIDTH, destination, 10, destination, 10);
    if (result > 0)
    {

        // std::wcout << L"Translated: " << destination << std::endl;
        std::cout << "length: " << wcslen(destination) << std::endl;
        std::cout << "Translated: " << convert_wchars_to_string(destination) << std::endl;
    }
    else
    {
        std::cerr << "Error in translation" << std::endl;
    }

    result = LCMapStringW(0, LCMAP_KATAKANA, source, 5, destination, 10);
    if (result > 0)
    {

        // std::wcout << L"Translated: " << destination << std::endl;
        std::cout << "length: " << wcslen(destination) << std::endl;
        std::cout << "Translated: " << convert_wchars_to_string(destination) << std::endl;
    }
    else
    {
        std::cerr << "Error in translation" << std::endl;
    }

    return 0;
}