#include <string>
#include <comutil.h>
#ifndef __VARIANT_T_H__
#define __VARIANT_T_H__
#ifdef __cplusplus

#include <Poco/Exception.h>
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

  void Clear()
  {
    _free();
  }

  bool isEmpty() const
  {
    return v.isEmpty();
  }

  bool isString() const
  {
    return v.isString();
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

#endif
#endif // __VARIANT_T_H__