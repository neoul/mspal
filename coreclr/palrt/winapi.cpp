#include "common.h"

// BSTR format:
// [4 bytes (length prefix)], wchar_t[length], L'\0'[\0]
BSTR SysAllocString(const OLECHAR *psz)
{
  if (psz == NULL)
    return NULL;

  size_t len = wcslen(psz);
  size_t total = (len + 1) * sizeof(WCHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    ptr[0] = len;
    ptr++;
    wcscpy_s((WCHAR *)ptr, len, psz);
  }
  return (BSTR)ptr;
}

BSTR SysAllocStringLen(const OLECHAR *strIn, UINT ui)
{
  size_t total = (ui + 1) * sizeof(WCHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    memset(ptr, 0, total);
    ptr[0] = ui;
    ptr++;
    if (strIn != NULL)
    {
      size_t len = wcslen(strIn);
      size_t min = len < ui ? len : ui;
      // wcsncpy((WCHAR *)ptr, strIn, min);
      wcscpy_s((WCHAR *)ptr, min, strIn);
    }
  }
  return (BSTR)ptr;
}

// It does not perform any ANSI-to-Unicode translation.... need dobule check.
// [FIXME] minipal_get_length_utf8_to_utf16, MultiByteToWideChar
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len)
{
  size_t total = len + sizeof(OLECHAR) + sizeof(UINT);
  UINT *ptr = (UINT *)malloc(total);
  if (ptr)
  {
    memset(ptr, 0, total);
    ptr[0] = len;
    ptr++;
    if (psz != NULL)
      strncpy((char *)ptr, psz, len);
  }
  return (BSTR)ptr;
}

// deallocate strings by SysAllocString, SysAllocStringByteLen,
// SysReAllocString, SysAllocStringLen, or SysReAllocStringLen.
void SysFreeString(BSTR bstrString)
{
  if (bstrString != NULL)
  {
    UINT *ptr = (UINT *)bstrString;
    ptr--;
    free(ptr);
  }
}

UINT SysStringLen(BSTR pbstr)
{
  if (pbstr == NULL)
    return 0;

  UINT *ptr = (UINT *)pbstr;
  ptr--;
  return *ptr;
}

UINT SysStringByteLen(BSTR pbstr)
{
  if (pbstr == NULL)
    return 0;

  UINT len = SysStringLen(pbstr);
  UINT byteLen = len * sizeof(OLECHAR);
  return byteLen;
}

HLOCAL LocalAlloc(UINT uFlags, SIZE_T uBytes)
{
  return malloc(uBytes);
}

HLOCAL LocalFree(HLOCAL hMem)
{
  free(hMem);
  return NULL;
}