/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _INC_COMUTIL
#define _INC_COMUTIL
#include <stdlib.h>
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

#include <com_error.h>

namespace _com_util
{
  BSTR ConvertStringToBSTR(const char *pSrc);
  char *ConvertBSTRToString(BSTR pSrc);
}

// declare _variant_t before use.
class _variant_t;

class _bstr_t
{
public:
  _bstr_t() throw();
  _bstr_t(const _bstr_t &s) throw();
  _bstr_t(const char *s);
  _bstr_t(const WCHAR *s);
  _bstr_t(const _variant_t &var);
  _bstr_t(BSTR bstr, bool fCopy);
  ~_bstr_t() throw();
  _bstr_t &operator=(const _bstr_t &s) throw();
  _bstr_t &operator=(const char *s);
  _bstr_t &operator=(const WCHAR *s);
  _bstr_t &operator=(const _variant_t &var);
  _bstr_t &operator+=(const _bstr_t &s);
  _bstr_t operator+(const _bstr_t &s) const;
  friend _bstr_t operator+(const char *s1, const _bstr_t &s2);
  friend _bstr_t operator+(const WCHAR *s1, const _bstr_t &s2);
  friend std::ostream &operator<<(std::ostream &os, const _bstr_t &dt);
  operator const WCHAR *() const throw();
  operator WCHAR *() const throw();
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
    Data_t(const WCHAR *s);
    Data_t(BSTR bstr, bool fCopy);
    Data_t(const _bstr_t &s1, const _bstr_t &s2);
    unsigned __LONG32 AddRef() throw();
    unsigned __LONG32 Release() throw();
    unsigned __LONG32 RefCount() const throw();
    operator const WCHAR *() const throw();
    operator const char *() const;
    const WCHAR *GetWString() const throw();
    WCHAR *&GetWString() throw();
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

inline _bstr_t::_bstr_t() throw() : m_Data(nullptr) {}

inline _bstr_t::_bstr_t(const _bstr_t &s) throw() : m_Data(s.m_Data) { _AddRef(); }

inline _bstr_t::_bstr_t(const char *s) : m_Data(new Data_t(s))
{
  if (!m_Data)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
  }
}

inline _bstr_t::_bstr_t(const WCHAR *s) : m_Data(new Data_t(s))
{
  if (!m_Data)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
  }
}

inline _bstr_t::_bstr_t(BSTR bstr, bool fCopy) : m_Data(new Data_t(bstr, fCopy))
{
  if (!m_Data)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
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
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
    }
  }
  return *this;
}

inline _bstr_t &_bstr_t::operator=(const WCHAR *s)
{
  _COM_ASSERT(!s || static_cast<const WCHAR *>(*this) != s);
  if (!s || static_cast<const WCHAR *>(*this) != s)
  {
    _Free();
    m_Data = new Data_t(s);
    if (!m_Data)
    {
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
    }
  }
  return *this;
}

inline _bstr_t &_bstr_t::operator+=(const _bstr_t &s)
{
  Data_t *newData = new Data_t(*this, s);
  if (!newData)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
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

inline _bstr_t operator+(const WCHAR *s1, const _bstr_t &s2)
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

inline _bstr_t::operator const WCHAR *() const throw() { return (m_Data != nullptr) ? m_Data->GetWString() : nullptr; }
inline _bstr_t::operator WCHAR *() const throw() { return const_cast<WCHAR *>((m_Data != nullptr) ? m_Data->GetWString() : nullptr); }
inline _bstr_t::operator const char *() const { return (m_Data != nullptr) ? m_Data->GetString() : nullptr; }
inline _bstr_t::operator char *() const { return const_cast<char *>((m_Data != nullptr) ? m_Data->GetString() : nullptr); }
inline bool _bstr_t::operator!() const throw() { return (m_Data != nullptr) ? !m_Data->GetWString() : true; }
inline bool _bstr_t::operator==(const _bstr_t &str) const throw() { return _Compare(str) == 0; }
inline bool _bstr_t::operator!=(const _bstr_t &str) const throw() { return _Compare(str) != 0; }
inline bool _bstr_t::operator<(const _bstr_t &str) const throw() { return _Compare(str) < 0; }
inline bool _bstr_t::operator>(const _bstr_t &str) const throw() { return _Compare(str) > 0; }
inline bool _bstr_t::operator<=(const _bstr_t &str) const throw() { return _Compare(str) <= 0; }
inline bool _bstr_t::operator>=(const _bstr_t &str) const throw() { return _Compare(str) >= 0; }
inline BSTR _bstr_t::copy(bool fCopy) const { return (m_Data != nullptr) ? (fCopy ? m_Data->Copy() : m_Data->GetWString()) : nullptr; }
inline unsigned int _bstr_t::length() const throw() { return (m_Data != nullptr) ? m_Data->Length() : 0; }
inline void _bstr_t::Assign(BSTR s)
{
  _COM_ASSERT(!s || !m_Data || m_Data->GetWString() != s);
  if (!s || !m_Data || m_Data->GetWString() != s)
  {
    _Free();
    m_Data = new Data_t(s, TRUE);
    if (!m_Data)
    {
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
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
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
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
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
  }
}

inline BSTR _bstr_t::Detach() throw()
{
  _COM_ASSERT(m_Data != nullptr && m_Data->RefCount() == 1);
  if (m_Data != nullptr && m_Data->RefCount() == 1)
  {
    BSTR b = m_Data->GetWString();
    m_Data->GetWString() = nullptr;
    _Free();
    return b;
  }
  else
  {
    throw _illegal_state_error("Invalid Detach of _bstr_t object");
  }
}

inline void _bstr_t::_AddRef() throw()
{
  if (m_Data != nullptr)
    m_Data->AddRef();
}

inline void _bstr_t::_Free() throw()
{
  if (m_Data != nullptr)
  {
    m_Data->Release();
    m_Data = nullptr;
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

inline _bstr_t::Data_t::Data_t(const char *s) : m_str(nullptr), m_RefCount(1)
{
  m_wstr = _com_util::ConvertStringToBSTR(s);
}

inline _bstr_t::Data_t::Data_t(const WCHAR *s) : m_str(nullptr), m_RefCount(1)
{
  m_wstr = ::SysAllocString(s);
  if (!m_wstr && s != nullptr)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
  }
}

inline _bstr_t::Data_t::Data_t(BSTR bstr, bool fCopy) : m_str(nullptr), m_RefCount(1)
{
  if (fCopy && bstr != nullptr)
  {
    m_wstr = ::SysAllocStringByteLen(reinterpret_cast<char *>(bstr), ::SysStringByteLen(bstr));
    if (!m_wstr)
    {
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
    }
  }
  else
    m_wstr = bstr;
}

inline _bstr_t::Data_t::Data_t(const _bstr_t &s1, const _bstr_t &s2) : m_str(nullptr), m_RefCount(1)
{
  const unsigned int l1 = s1.length();
  const unsigned int l2 = s2.length();
  m_wstr = ::SysAllocStringByteLen(nullptr, (l1 + l2) * sizeof(wchar_t));
  if (!m_wstr)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
    return;
  }
  const WCHAR *wstr1 = static_cast<const WCHAR *>(s1);
  if (wstr1 != nullptr)
  {
    _COM_MEMCPY_S(m_wstr, (l1 + l2 + 1) * sizeof(wchar_t), wstr1, (l1 + 1) * sizeof(wchar_t));
  }
  const WCHAR *wstr2 = static_cast<const WCHAR *>(s2);
  if (wstr2 != nullptr)
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
inline _bstr_t::Data_t::operator const WCHAR *() const throw() { return m_wstr; }
inline _bstr_t::Data_t::operator const char *() const { return GetString(); }
inline const WCHAR *_bstr_t::Data_t::GetWString() const throw() { return m_wstr; }
inline WCHAR *&_bstr_t::Data_t::GetWString() throw() { return m_wstr; }
inline const char *_bstr_t::Data_t::GetString() const
{
  if (!m_str)
    m_str = _com_util::ConvertBSTRToString(m_wstr);
  return m_str;
}
inline BSTR _bstr_t::Data_t::Copy() const
{
  if (m_wstr != nullptr)
  {
    BSTR bstr = ::SysAllocStringByteLen(reinterpret_cast<char *>(m_wstr), ::SysStringByteLen(m_wstr));
    if (!bstr)
    {
      throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
    }
    return bstr;
  }
  return nullptr;
}
inline void _bstr_t::Data_t::Assign(BSTR s)
{
  _Free();
  if (s != nullptr)
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
    return nullptr;
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
  if (m_wstr != nullptr)
    ::SysFreeString(m_wstr);
  if (m_str != nullptr)
    free(m_str);
}

#include <variant_t.h>

inline _bstr_t::_bstr_t(const ::_variant_t &var)
{
  auto cstr = var.toString();
  m_Data = new Data_t(cstr.c_str());
  if (!m_Data)
  {
    throw _out_of_memory_error("Out of memory" + std::string(__FILE__) + std::to_string(__LINE__));
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

#endif /* __cplusplus */

#endif
