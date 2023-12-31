#include "common.h"
#include <string>
#include <locale>
#include <codecvt>
#include <dn-u16.h>

// BSTR format:
// [4 bytes (length prefix)], wchar_t[length], wchar_t L'\0'[\0]
// SysString need more space for _SAFECRT__FILL_STRING
// #if __SIZEOF_WCHAR_T__ == 2
// #else
BSTR SysAllocString(const OLECHAR *psz)
{
  if (psz == nullptr)
    return nullptr;

  size_t len = u16_strlen(psz);
  size_t total = (len + 2) * sizeof(WCHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    ptr[0] = len;
    ptr++;
    u16_strcpy_s((WCHAR *)ptr, len+1, psz);
  }
  return (BSTR)ptr;
}

BSTR SysAllocStringLen(const OLECHAR *strIn, UINT ui)
{
  size_t total = (ui + 2) * sizeof(WCHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    memset(ptr, 0, total);
    ptr[0] = ui;
    ptr++;
    if (strIn != nullptr)
    {
      size_t len = u16_strlen(strIn);
      size_t min = len < ui ? len : ui;
      u16_strcpy_s((WCHAR *)ptr, min + 1, strIn);
    }
  }
  return (BSTR)ptr;
}

// It does not perform any ANSI-to-Unicode translation.... need dobule check.
// [FIXME] minipal_get_length_utf8_to_utf16, MultiByteToWideChar
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len)
{
  size_t total = (len + 1) + sizeof(OLECHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    memset(ptr, 0, total);
    ptr[0] = len;
    ptr++;
    if (psz != nullptr)
      strncpy((char *)ptr, psz, len + 1);
  }
  return (BSTR)ptr;
}

// deallocate strings by SysAllocString, SysAllocStringByteLen,
// SysReAllocString, SysAllocStringLen, or SysReAllocStringLen.
void SysFreeString(BSTR bstrString)
{
  if (bstrString != nullptr)
  {
    UINT *ptr = (UINT *)bstrString;
    ptr--;
    free(ptr);
  }
}

UINT SysStringLen(BSTR pbstr)
{
  if (pbstr == nullptr)
    return 0;

  UINT *ptr = (UINT *)pbstr;
  ptr--;
  return *ptr;
}

UINT SysStringByteLen(BSTR pbstr)
{
  if (pbstr == nullptr)
    return 0;

  UINT len = SysStringLen(pbstr);
  UINT byteLen = len * sizeof(OLECHAR);
  return byteLen;
}
// #endif

HLOCAL LocalAlloc(UINT uFlags, SIZE_T uBytes)
{
  return malloc(uBytes + 4);
}

HLOCAL LocalFree(HLOCAL hMem)
{
  free(hMem);
  return nullptr;
}