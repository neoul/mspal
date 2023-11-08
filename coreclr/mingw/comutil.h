/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _INC_COMUTIL
#define _INC_COMUTIL
#include <cstdlib>
#include <iostream>
#include <codecvt>
#include <string>
#include <typeinfo>
#include <locale>

// #include <pal.h>
#include <palrt.h>
#include <mingw.h> // It is needed to overwrite some definitions.

#include <ole2.h>
#include <stdio.h>

#ifndef _COM_ASSERT
#define _COM_ASSERT(x) ((void)0)
#endif

#define _COM_MEMCPY_S(dest, destsize, src, count) memcpy(dest, src, count)

/* Use of wsprintf might be impossible, if strsafe.h is included. */
#ifndef __STDC_SECURE_LIB__
#define _COM_PRINTF_S_1(dest, destsize, format, arg1) wsprintf(dest, format, arg1)
#elif defined(UNICODE)
#define _COM_PRINTF_S_1(dest, destsize, format, arg1) swprintf_s(dest, destsize, format, arg1)
#else
#define _COM_PRINTF_S_1(dest, destsize, format, arg1) sprintf_s(dest, destsize, format, arg1)
#endif

#ifdef __cplusplus

// #pragma push_macro("new")
// #undef new

#ifndef WINAPI
#if defined(_ARM_)
#define WINAPI
#else
#define WINAPI __stdcall
#endif
#endif

class _com_error;

void WINAPI _com_issue_error(HRESULT);

class _bstr_t;
class _variant_t;

namespace _com_util
{
  inline void CheckError(HRESULT hr)
  {
    if (FAILED(hr))
    {
      _com_issue_error(hr);
    }
  }

  BSTR ConvertStringToBSTR(const char *pSrc);
  char *ConvertBSTRToString(BSTR pSrc);
}

class _bstr_t
{
public:
  _bstr_t() throw();
  _bstr_t(const _bstr_t &s) throw();
  _bstr_t(const char *s);
  _bstr_t(const wchar_t *s);
  _bstr_t(const ::_variant_t &var);
  _bstr_t(BSTR bstr, bool fCopy);
  ~_bstr_t() throw();
  _bstr_t &operator=(const _bstr_t &s) throw();
  _bstr_t &operator=(const char *s);
  _bstr_t &operator=(const wchar_t *s);
  _bstr_t &operator=(const _variant_t &var);
  _bstr_t &operator+=(const _bstr_t &s);
  _bstr_t operator+(const _bstr_t &s) const;
  friend _bstr_t operator+(const char *s1, const _bstr_t &s2);
  friend _bstr_t operator+(const wchar_t *s1, const _bstr_t &s2);
  friend std::ostream &operator<<(std::ostream &os, const _bstr_t &dt);
  operator const wchar_t *() const throw();
  operator wchar_t *() const throw();
  operator const char *() const;
  operator char *() const;
  bool operator!() const throw();
  bool operator==(const _bstr_t &str) const throw();
  bool operator!=(const _bstr_t &str) const throw();
  bool operator<(const _bstr_t &str) const throw();
  bool operator>(const _bstr_t &str) const throw();
  bool operator<=(const _bstr_t &str) const throw();
  bool operator>=(const _bstr_t &str) const throw();
  BSTR copy(bool fCopy = true) const;
  unsigned int length() const throw();
  void Assign(BSTR s);
  BSTR &GetBSTR();
  BSTR *GetAddress();
  void Attach(BSTR s);
  BSTR Detach() throw();

private:
  class Data_t
  {
  public:
    Data_t(const char *s);
    Data_t(const wchar_t *s);
    Data_t(BSTR bstr, bool fCopy);
    Data_t(const _bstr_t &s1, const _bstr_t &s2);
    unsigned __LONG32 AddRef() throw();
    unsigned __LONG32 Release() throw();
    unsigned __LONG32 RefCount() const throw();
    operator const wchar_t *() const throw();
    operator const char *() const;
    const wchar_t *GetWString() const throw();
    wchar_t *&GetWString() throw();
    const char *GetString() const;
    BSTR Copy() const;
    void Assign(BSTR s);
    void Attach(BSTR s) throw();
    unsigned int Length() const throw();
    int Compare(const Data_t &str) const throw();
    void *operator new(size_t sz);

  private:
    BSTR m_wstr;
    mutable char *m_str;
    unsigned __LONG32 m_RefCount;
    Data_t() throw();
    Data_t(const Data_t &s) throw();
    ~Data_t() throw();
    void _Free() throw();
  };

private:
  Data_t *m_Data;

private:
  void _AddRef() throw();
  void _Free() throw();
  int _Compare(const _bstr_t &str) const throw();
};

inline _bstr_t::_bstr_t() throw() : m_Data(NULL) {}

inline _bstr_t::_bstr_t(const _bstr_t &s) throw() : m_Data(s.m_Data) { _AddRef(); }

inline _bstr_t::_bstr_t(const char *s) : m_Data(new Data_t(s))
{
  if (!m_Data)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline _bstr_t::_bstr_t(const wchar_t *s) : m_Data(new Data_t(s))
{
  if (!m_Data)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline _bstr_t::_bstr_t(BSTR bstr, bool fCopy) : m_Data(new Data_t(bstr, fCopy))
{
  if (!m_Data)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline _bstr_t::~_bstr_t() throw() { _Free(); }

inline _bstr_t &_bstr_t::operator=(const _bstr_t &s) throw()
{
  if (this != &s)
  {
    _Free();
    m_Data = s.m_Data;
    _AddRef();
  }
  return *this;
}

inline _bstr_t &_bstr_t::operator=(const char *s)
{
  _COM_ASSERT(!s || static_cast<const char *>(*this) != s);
  if (!s || static_cast<const char *>(*this) != s)
  {
    _Free();
    m_Data = new Data_t(s);
    if (!m_Data)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
  }
  return *this;
}

inline _bstr_t &_bstr_t::operator=(const wchar_t *s)
{
  _COM_ASSERT(!s || static_cast<const wchar_t *>(*this) != s);
  if (!s || static_cast<const wchar_t *>(*this) != s)
  {
    _Free();
    m_Data = new Data_t(s);
    if (!m_Data)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
  }
  return *this;
}

inline _bstr_t &_bstr_t::operator+=(const _bstr_t &s)
{
  Data_t *newData = new Data_t(*this, s);
  if (!newData)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
  else
  {
    _Free();
    m_Data = newData;
  }
  return *this;
}

inline _bstr_t _bstr_t::operator+(const _bstr_t &s) const
{
  _bstr_t b = *this;
  b += s;
  return b;
}

inline _bstr_t operator+(const char *s1, const _bstr_t &s2)
{
  _bstr_t b = s1;
  b += s2;
  return b;
}

inline _bstr_t operator+(const wchar_t *s1, const _bstr_t &s2)
{
  _bstr_t b = s1;
  b += s2;
  return b;
}

inline std::ostream &operator<<(std::ostream &os, const _bstr_t &s)
{
  auto temp_s = static_cast<const char *>(s);
  os << temp_s;
  return os;
}

inline _bstr_t::operator const wchar_t *() const throw() { return (m_Data != NULL) ? m_Data->GetWString() : NULL; }
inline _bstr_t::operator wchar_t *() const throw() { return const_cast<wchar_t *>((m_Data != NULL) ? m_Data->GetWString() : NULL); }
inline _bstr_t::operator const char *() const { return (m_Data != NULL) ? m_Data->GetString() : NULL; }
inline _bstr_t::operator char *() const { return const_cast<char *>((m_Data != NULL) ? m_Data->GetString() : NULL); }
inline bool _bstr_t::operator!() const throw() { return (m_Data != NULL) ? !m_Data->GetWString() : true; }
inline bool _bstr_t::operator==(const _bstr_t &str) const throw() { return _Compare(str) == 0; }
inline bool _bstr_t::operator!=(const _bstr_t &str) const throw() { return _Compare(str) != 0; }
inline bool _bstr_t::operator<(const _bstr_t &str) const throw() { return _Compare(str) < 0; }
inline bool _bstr_t::operator>(const _bstr_t &str) const throw() { return _Compare(str) > 0; }
inline bool _bstr_t::operator<=(const _bstr_t &str) const throw() { return _Compare(str) <= 0; }
inline bool _bstr_t::operator>=(const _bstr_t &str) const throw() { return _Compare(str) >= 0; }
inline BSTR _bstr_t::copy(bool fCopy) const { return (m_Data != NULL) ? (fCopy ? m_Data->Copy() : m_Data->GetWString()) : NULL; }
inline unsigned int _bstr_t::length() const throw() { return (m_Data != NULL) ? m_Data->Length() : 0; }
inline void _bstr_t::Assign(BSTR s)
{
  _COM_ASSERT(!s || !m_Data || m_Data->GetWString() != s);
  if (!s || !m_Data || m_Data->GetWString() != s)
  {
    _Free();
    m_Data = new Data_t(s, TRUE);
    if (!m_Data)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
  }
}

inline BSTR &_bstr_t::GetBSTR()
{
  if (!m_Data)
  {
    m_Data = new Data_t(0, FALSE);
    if (!m_Data)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
  }
  return m_Data->GetWString();
}

inline BSTR *_bstr_t::GetAddress()
{
  Attach(0);
  return &m_Data->GetWString();
}

inline void _bstr_t::Attach(BSTR s)
{
  _Free();
  m_Data = new Data_t(s, FALSE);
  if (!m_Data)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline BSTR _bstr_t::Detach() throw()
{
  _COM_ASSERT(m_Data != NULL && m_Data->RefCount() == 1);
  if (m_Data != NULL && m_Data->RefCount() == 1)
  {
    BSTR b = m_Data->GetWString();
    m_Data->GetWString() = NULL;
    _Free();
    return b;
  }
  else
  {
    _com_issue_error(E_POINTER);
    return NULL;
  }
}

inline void _bstr_t::_AddRef() throw()
{
  if (m_Data != NULL)
    m_Data->AddRef();
}

inline void _bstr_t::_Free() throw()
{
  if (m_Data != NULL)
  {
    m_Data->Release();
    m_Data = NULL;
  }
}

inline int _bstr_t::_Compare(const _bstr_t &str) const throw()
{
  if (m_Data == str.m_Data)
    return 0;
  if (!m_Data)
    return -1;
  if (!str.m_Data)
    return 1;
  return m_Data->Compare(*str.m_Data);
}

inline _bstr_t::Data_t::Data_t(const char *s) : m_str(NULL), m_RefCount(1)
{
  m_wstr = _com_util::ConvertStringToBSTR(s);
}

inline _bstr_t::Data_t::Data_t(const wchar_t *s) : m_str(NULL), m_RefCount(1)
{
  m_wstr = ::SysAllocString(s);
  if (!m_wstr && s != NULL)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline _bstr_t::Data_t::Data_t(BSTR bstr, bool fCopy) : m_str(NULL), m_RefCount(1)
{
  if (fCopy && bstr != NULL)
  {
    m_wstr = ::SysAllocStringByteLen(reinterpret_cast<char *>(bstr), ::SysStringByteLen(bstr));
    if (!m_wstr)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
  }
  else
    m_wstr = bstr;
}

inline _bstr_t::Data_t::Data_t(const _bstr_t &s1, const _bstr_t &s2) : m_str(NULL), m_RefCount(1)
{
  const unsigned int l1 = s1.length();
  const unsigned int l2 = s2.length();
  m_wstr = ::SysAllocStringByteLen(NULL, (l1 + l2) * sizeof(wchar_t));
  if (!m_wstr)
  {
    _com_issue_error(E_OUTOFMEMORY);
    return;
  }
  const wchar_t *wstr1 = static_cast<const wchar_t *>(s1);
  if (wstr1 != NULL)
  {
    _COM_MEMCPY_S(m_wstr, (l1 + l2 + 1) * sizeof(wchar_t), wstr1, (l1 + 1) * sizeof(wchar_t));
  }
  const wchar_t *wstr2 = static_cast<const wchar_t *>(s2);
  if (wstr2 != NULL)
  {
    _COM_MEMCPY_S(m_wstr + l1, (l2 + 1) * sizeof(wchar_t), wstr2, (l2 + 1) * sizeof(wchar_t));
  }
}

inline unsigned __LONG32 _bstr_t::Data_t::AddRef() throw()
{
#if 0
  InterlockedIncrement(reinterpret_cast<LONG *>(&m_RefCount));
#else
  m_RefCount++;
#endif
  return m_RefCount;
}

inline unsigned __LONG32 _bstr_t::Data_t::Release() throw()
{
#if 0
  unsigned __LONG32 cRef = InterlockedDecrement(reinterpret_cast<LONG *>(&m_RefCount));
#else
  unsigned __LONG32 cRef = --m_RefCount;
#endif
  if (cRef == 0)
    delete this;
  return cRef;
}

inline unsigned __LONG32 _bstr_t::Data_t::RefCount() const throw() { return m_RefCount; }
inline _bstr_t::Data_t::operator const wchar_t *() const throw() { return m_wstr; }
inline _bstr_t::Data_t::operator const char *() const { return GetString(); }
inline const wchar_t *_bstr_t::Data_t::GetWString() const throw() { return m_wstr; }
inline wchar_t *&_bstr_t::Data_t::GetWString() throw() { return m_wstr; }
inline const char *_bstr_t::Data_t::GetString() const
{
  if (!m_str)
    m_str = _com_util::ConvertBSTRToString(m_wstr);
  return m_str;
}
inline BSTR _bstr_t::Data_t::Copy() const
{
  if (m_wstr != NULL)
  {
    BSTR bstr = ::SysAllocStringByteLen(reinterpret_cast<char *>(m_wstr), ::SysStringByteLen(m_wstr));
    if (!bstr)
    {
      _com_issue_error(E_OUTOFMEMORY);
    }
    return bstr;
  }
  return NULL;
}
inline void _bstr_t::Data_t::Assign(BSTR s)
{
  _Free();
  if (s != NULL)
  {
    m_wstr = ::SysAllocStringByteLen(reinterpret_cast<char *>(s), ::SysStringByteLen(s));
    m_str = 0;
  }
}
inline void _bstr_t::Data_t::Attach(BSTR s) throw()
{
  _Free();
  m_wstr = s;
  m_str = 0;
  m_RefCount = 1;
}
inline unsigned int _bstr_t::Data_t::Length() const throw() { return m_wstr ? ::SysStringLen(m_wstr) : 0; }
inline int _bstr_t::Data_t::Compare(const _bstr_t::Data_t &str) const throw()
{
  if (!m_wstr)
    return str.m_wstr ? -1 : 0;
  if (!str.m_wstr)
    return 1;
  const unsigned int l1 = ::SysStringLen(m_wstr);
  const unsigned int l2 = ::SysStringLen(str.m_wstr);
  unsigned int len = l1;
  if (len > l2)
    len = l2;
  BSTR bstr1 = m_wstr;
  BSTR bstr2 = str.m_wstr;
  while (len-- > 0)
  {
    if (*bstr1++ != *bstr2++)
      return bstr1[-1] - bstr2[-1];
  }
  return (l1 < l2) ? -1 : (l1 == l2) ? 0
                                     : 1;
}

#ifdef _COM_OPERATOR_NEW_THROWS
inline void *_bstr_t::Data_t::operator new(size_t sz)
{
  try
  {
    return ::operator new(sz);
  }
  catch (...)
  {
    return NULL;
  }
}
#else
inline void *_bstr_t::Data_t::operator new(size_t sz)
{
  return ::operator new(sz);
}
#endif

inline _bstr_t::Data_t::~Data_t() throw() { _Free(); }
inline void _bstr_t::Data_t::_Free() throw()
{
  if (m_wstr != NULL)
    ::SysFreeString(m_wstr);
  if (m_str != NULL)
    free(m_str);
}

#include <Poco/Dynamic/Var.h>

std::string convert_wchars_to_string(const wchar_t *pVal);
const wchar_t *allocate_wchars_from_string(const std::string &str);

class _variant_t
{
public:
  template <typename T>
  _variant_t(const T &val) : v(val) {}
  _variant_t(const char *pVal) : v(pVal) {}
  // This stores strings by UTF-8 encoding (multi-bytes string)
  _variant_t(const wchar_t *pVal)
  {
    v = convert_wchars_to_string(pVal);
  }
  _variant_t(const _bstr_t &bstrSrc)
  {
    v = static_cast<const char *>(bstrSrc);
  }
  _variant_t(const Poco::Dynamic::Var &var) : v(var) {}
  _variant_t(const _variant_t &var) : v(var.v) {}
  _variant_t() : v() {}
  ~_variant_t()
  {
    _free();
  }

  template <typename T>
  _variant_t &operator=(const T &other)
  {
    _free();
    v = other;
    return *this;
  }
  _variant_t &operator=(const char *pVal)
  {
    _free();
    v = pVal;
    return *this;
  }
  _variant_t &operator=(const wchar_t *pVal)
  {
    _free();
    v = convert_wchars_to_string(pVal);
    return *this;
  }
  _variant_t &operator=(const _bstr_t &bstrSrc)
  {
    _free();
    v = static_cast<const char *>(bstrSrc);
    return *this;
  }
  _variant_t &operator=(const Poco::Dynamic::Var &other)
  {
    _free();
    v = other;
    return *this;
  }
  _variant_t &operator=(const _variant_t &other)
  {
    _free();
    v = other.v;
    return *this;
  }

  template <typename T>
  explicit operator T() const
  {
    if (v.isEmpty())
      return T();
    try
    {
      return v.extract<T>();
    }
    catch (...)
    {
      return v.convert<T>();
    }
  }

  explicit operator std::string() throw()
  {
    return getString();
  }
  explicit operator const wchar_t *() throw()
  {
    if (wcs == nullptr)
    {
      auto vstr = getString();
      wcs = allocate_wchars_from_string(vstr);
    }
    return wcs;
  }
  explicit operator const char *() throw()
  {
    auto vstr = getString();
    if (cs == nullptr)
    {
      cs = new char[vstr.size() + 1];
      memcpy((void *)cs, vstr.c_str(), vstr.size() + 1);
    }
    return cs;
  }

  std::wstring toWString() const
  {
    auto val = getString();
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    auto wval = converter.from_bytes(val);
    return wval;
  }

  template <typename T>
  T value() const
  {
    if (v.isEmpty())
      return T();
    try
    {
      return v.extract<T>();
    }
    catch (...)
    {
      return v.convert<T>();
    }
  }

  std::string toString() const
  {
    return getString();
  }

  void clear()
  {
    _free();
  }

  bool isEmpty() const
  {
    return v.isEmpty();
  }

  Poco::Dynamic::Var var()
  {
    Poco::Dynamic::Var out = v;
    return out;
  }

private:
  Poco::Dynamic::Var v;
  const wchar_t *wcs = nullptr;
  const char *cs = nullptr;

  void _free()
  {
    v.clear();
    if (wcs != nullptr)
    {
      free((void *)wcs);
      wcs = nullptr;
    }
    if (cs != nullptr)
    {
      delete[] cs;
      cs = nullptr;
    }
  }

  std::string getString() const
  {
    if (v.isEmpty())
      return std::string();
    try
    {
      return v.extract<std::string>();
    }
    catch (...)
    {
      return v.convert<std::string>();
    }
  }
};

inline _bstr_t::_bstr_t(const ::_variant_t &var)
{
  auto cstr = var.toString();
  m_Data = new Data_t(cstr.c_str());
  if (!m_Data)
  {
    _com_issue_error(E_OUTOFMEMORY);
  }
}

inline _bstr_t &_bstr_t::operator=(const _variant_t &var)
{

  std::string cstr = var.value<std::string>();
  m_Data = new Data_t(cstr.c_str());
  return *this;
}

extern _variant_t vtMissing;

#ifndef _USE_RAW
#define bstr_t _bstr_t
#define variant_t _variant_t
#endif

// #pragma pop_macro("new")

/* We use _com_issue_error here, but we only provide its inline version in comdef.h,
 * so we need to make sure that it's included as well. */
#include <comdef.h>

#endif /* __cplusplus */

#endif
