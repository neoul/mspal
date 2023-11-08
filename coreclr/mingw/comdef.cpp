/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _COMDEF_CPP_
#define _COMDEF_CPP_
#include <comdef.h>
#ifdef __cplusplus


void WINAPI _com_raise_error(HRESULT hr, IErrorInfo *perrinfo = 0) {
#if __EXCEPTIONS
    throw _com_error(hr, perrinfo);
#else
    /* This is designed to use exceptions. If exceptions are disabled, there is not much we can do here. */
    __debugbreak();
#endif
}

void (WINAPI *__com_error_handler)(HRESULT hr,IErrorInfo *perrinfo) = _com_raise_error;
// void WINAPI (*__com_error_handler)(HRESULT,IErrorInfo *) = _com_raise_error;

void WINAPI _set_com_error_handler(void (WINAPI *pHandler)(HRESULT hr,IErrorInfo *perrinfo)) {
  __com_error_handler = pHandler;
}

void WINAPI _com_issue_error(HRESULT hr) {
  __com_error_handler(hr, NULL);
}

#endif /* __cplusplus */

#endif
