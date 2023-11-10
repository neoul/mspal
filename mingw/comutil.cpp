/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _COMUTIL_CPP_
#define _COMUTIL_CPP_
#include <comutil.h>
#ifdef __cplusplus

BSTR _com_util::ConvertStringToBSTR(const char *pSrc)
{
  if (pSrc == nullptr)
    return nullptr;
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt2;
  std::u16string utf16 = cvt2.from_bytes(pSrc);
  size_t len = utf16.length();
  size_t total = (len + 1) * sizeof(wchar_t) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr != nullptr)
  {
    ptr[0] = len;
    ptr++;
    BSTR bstr = (BSTR)ptr;
    for (int i = 0; i < len; i++)
    {
      bstr[i] = utf16[i];
    }
    bstr[len] = '\0';
  }
  return (BSTR)ptr;
}

char *_com_util::ConvertBSTRToString(BSTR pSrc)
{
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt2;
  auto utf8 = cvt2.to_bytes((char16_t *)pSrc);
  char *p = (char *)malloc(utf8.length() + 1);
  if (p != nullptr)
  {
    strcpy(p, utf8.c_str());
    p[utf8.length()] = '\0';
  }
  return p;
}

std::string convert_wchars_to_string(const wchar_t *pVal)
{
#if __SIZEOF_WCHAR_T__ == 2
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
  auto utf8 = cvt.to_bytes((char16_t *)pVal);
  return utf8;
#else
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
  auto utf8 = cvt.to_bytes((char32_t *)pVal);
  return utf8;
#endif
}

const wchar_t *allocate_wchars_from_string(const std::string &str)
{
#if __SIZEOF_WCHAR_T__ == 2
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
  auto utf16 = cvt.from_bytes(str);
  size_t len = utf16.size();
  char16_t *new_ws = (char16_t *)malloc((len + 1) * sizeof(char16_t));
  for (int i = 0; i < len; i++)
    new_ws[i] = utf16[i];
  new_ws[len] = 0;
  return reinterpret_cast<const wchar_t *>(new_ws);
#else
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
  auto utf32 = cvt.from_bytes(str);
  size_t len = utf32.size();
  char32_t *new_ws = (char32_t *)malloc((len + 1) * sizeof(char32_t));
  for (int i = 0; i < len; i++)
    new_ws[i] = utf32[i];
  new_ws[len] = 0;
  return reinterpret_cast<const wchar_t *>(new_ws);
#endif
}

#endif /* __cplusplus */

#endif
