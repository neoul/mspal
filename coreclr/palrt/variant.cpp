// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//

//
// ===========================================================================
// File: variant.cpp
//
// PALRT variant conversion functions
// ===========================================================================

#include "common.h"

/***
*PUBLIC void VariantInit(VARIANT*)
*Purpose:
*  Initialize the given VARIANT to VT_EMPTY.
*
*Entry:
*  None
*
*Exit:
*  return value = void
*
*  pvarg = pointer to initialized VARIANT
*
***********************************************************************/
STDAPI_(void)
VariantInit(VARIANT FAR* pvarg)
{
    V_VT(pvarg) = VT_EMPTY;
}

STDAPI_(HRESULT) VariantClear(
  IN OUT VARIANTARG *pvarg
) {
    if (pvarg == NULL) {
        return E_INVALIDARG;
    }
    if (V_VT(pvarg) == VT_EMPTY) {
        return S_OK;
    }
    switch (V_VT(pvarg)) {
        case VT_DISPATCH:
        case VT_ERROR:
        case VT_VARIANT:
        case VT_UNKNOWN:
        case VT_SAFEARRAY:
        case VT_CARRAY:
        case VT_USERDEFINED:
        case VT_RECORD:
        case VT_FILETIME:
        case VT_BLOB:
        case VT_STREAM:
        case VT_STORAGE:
        case VT_STREAMED_OBJECT:
        case VT_STORED_OBJECT:
        case VT_BLOB_OBJECT:
        case VT_CF:
        case VT_CLSID:
        case VT_VECTOR:
        case VT_ARRAY:
            return E_NOTIMPL;
        // case VT_EMPTY:
        // case VT_NULL:
        // case VT_I2:
        // case VT_I4:
        // case VT_R4:
        // case VT_R8:
        // case VT_CY:
        // case VT_DATE:
        //     V_I8(pvarg) = 0;
        //     break;
        case VT_BSTR:
            SysFreeString(V_BSTR(pvarg));
            V_BSTR(pvarg) = NULL;
            break;
        case VT_BOOL:
            V_I8(pvarg) = 0;
            break;
        case VT_DECIMAL:
            DECIMAL_SETZERO(V_DECIMAL(pvarg));
            break;
        // case VT_I1:
        // case VT_UI1:
        // case VT_UI2:
        // case VT_UI4:
        // case VT_I8:
        // case VT_UI8:
        // case VT_INT:
        // case VT_UINT:
        // case VT_HRESULT:
        //     V_I8(pvarg) = 0;
        //     break;
        case VT_VOID:
        case VT_PTR:
        case VT_LPSTR:
        case VT_LPWSTR:
        case VT_INT_PTR:
        case VT_UINT_PTR:
        case VT_BYREF:
            V_BYREF(pvarg) = NULL;
            break;
        default:
            V_I8(pvarg) = 0;
    }
    VariantInit(pvarg);
    return S_OK;
}

STDAPI_(HRESULT) VariantCopy(
    OUT VARIANTARG *pvargDest,
    IN const VARIANTARG *pvargSrc)
{
    if (pvargDest == NULL || pvargSrc == NULL) {
        return E_INVALIDARG;
    }
    if (V_VT(pvargSrc) == VT_EMPTY) {
        VariantClear(pvargDest);
        return S_OK;
    }
    switch (V_VT(pvargSrc)) {
        case VT_EMPTY:
            return VariantClear(pvargDest);
        case VT_BSTR: {
            VariantClear(pvargDest);
            BSTR s = V_BSTR(pvargSrc);
            size_t blen =  SysStringByteLen(s);
            BSTR d = SysAllocStringByteLen((LPCSTR)s, blen);
            V_BSTR(pvargDest) = d;
            break;
        }
        default:
            // others are not supported yet.
            return E_NOTIMPL;
    }
    V_VT(pvargDest) = V_VT(pvargSrc);
    return S_OK;
}

STDAPI_(HRESULT) VariantChangeType(
  OUT VARIANTARG       *pvargDest,
  IN  const VARIANTARG *pvarSrc,
  IN  USHORT           wFlags,
  IN  VARTYPE          vt
) {
    return E_NOTIMPL;
}

