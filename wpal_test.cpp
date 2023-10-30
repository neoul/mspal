#include <iostream>

// #include <wpal.h>

typedef wchar_t WCHAR;
typedef WCHAR *BSTR;

namespace _com_util
{

  // it will convert utf8 (multi-bytes characters) to utf16 or utf32 depends on sizeof(wchar_t)
  BSTR ConvertStringToBSTR(const char *pSrc)
  {
    if (pSrc == NULL)
      return NULL;
    size_t len = mbstowcs(NULL, pSrc, 0);
    if (len == (size_t)-1)
      return NULL;
    size_t total = (len + 1) * sizeof(WCHAR) + sizeof(unsigned int);
    std::cout << total << " " << sizeof(WCHAR) << std::endl;
    unsigned int *ptr = (unsigned int *)malloc(total);
    if (ptr != NULL)
    {
      ptr[0] = len;
      ptr++;
      if (pSrc != NULL)
        mbstowcs((BSTR)ptr, pSrc, len);
    }
    std::cout << len << " " << sizeof(WCHAR) << std::endl;
    return (BSTR)ptr;
  }

  char *ConvertBSTRToString(BSTR pSrc)
  {
    size_t len = wcstombs(NULL, pSrc, 0);
    if (len < 0)
      return NULL;
    char *mbs = (char *)malloc(len + 1);
    if (mbs == NULL)
      return NULL;
    if (wcstombs(mbs, pSrc, len + 1) == (size_t)-1)
      return NULL;
    mbs[len] = '\0';
    return mbs;
  }
}

#ifndef _COM_ASSERT
#define _COM_ASSERT(x) ((void)0)
#endif

class _bstr_t
{
public:
  _bstr_t(const char *s);
  ~_bstr_t() throw();
  friend std::ostream &operator<<(std::ostream &os, const _bstr_t &dt);
  operator char *() const;

private:
  class Data_t
  {
  public:
    Data_t(const char *s);
    operator const char *() const;
    void *operator new(size_t sz);

  private:
    BSTR m_wstr;
    mutable char *m_str;
    unsigned int m_RefCount;
    Data_t() throw();
    Data_t(const Data_t &s) throw();
    ~Data_t() throw();
    void _Free() throw();
  };

private:
  Data_t *m_Data;
};

inline _bstr_t::_bstr_t(const char *s) : m_Data(new Data_t(s))
{
  if (!m_Data)
  {
    throw std::string("E_OUTOFMEMORY!");
  }
}

inline std::ostream &operator<<(std::ostream &os, const _bstr_t &s)
{
  os << static_cast<const char *>(s);
  return os;
}
inline _bstr_t::Data_t::Data_t(const char *s) : m_str(NULL), m_RefCount(1)
{
  m_wstr = _com_util::ConvertStringToBSTR(s);
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

// inline _bstr_t::Data_t::~Data_t() throw() { _Free(); }
// inline void _bstr_t::Data_t::_Free() throw()
// {
//   if (m_wstr != NULL)
//     ::SysFreeString(m_wstr);
//   if (m_str != NULL)
//     delete[] m_str;
// }

void use_bstr_t()
{
  _bstr_t *bstr1 = new _bstr_t("Hello");
  _bstr_t *bstr2 = new _bstr_t("World");
  // _bstr_t bstr1("Hello");
  // // std::cout << "hello" << std::endl;
  // _bstr_t bstr2("World");
  // // std::cout << "world" << std::endl;
  std::cout << *bstr1 << std::endl;
}

int main()
{
  // std::cout << "BSTR"
  //           << " " << sizeof(BSTR) << std::endl; // 8
  // std::cout << "tagDEC"
  //           << " " << sizeof(tagDEC) << std::endl; // 1
  // std::cout << "wchar_t"
  //           << " " << sizeof(wchar_t) << std::endl; // 8
  // std::cout << "_bstr_t"
  //           << " " << sizeof(_bstr_t) << std::endl; // 1
  // std::cout << sizeof(long) << std::endl;           // 8
  // std::cout << sizeof(int) << std::endl;            // 4
  // SYSTEMTIME TheTime;
  // GetSystemTime(&TheTime);
  // std::cout << std::string("Hello, CMake world! ") << std::to_string(TheTime.wYear) << std::endl;
  use_bstr_t();
}