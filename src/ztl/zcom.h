#pragma once
#include <windows.h>
#include <strsafe.h>
#include <comdef.h>
#include <cstdint>


namespace ZComAPI {

inline wchar_t* __cdecl ZComSysAllocStringLen(const wchar_t* s, uint32_t u) {
    wchar_t* result = static_cast<wchar_t*>(CoTaskMemAlloc((u + 1) * sizeof(wchar_t) + sizeof(uint32_t)));
    if (result) {
        uint32_t uByteLen = 2 * u;
        *reinterpret_cast<uint32_t*>(result) = uByteLen;
        result = reinterpret_cast<wchar_t*>(reinterpret_cast<uint32_t>(result) + 1);
        if (s) {
            memcpy(result, s, uByteLen);
        }
        result[u] = 0;
    }
    return result;
}
inline wchar_t* __cdecl ZComSysAllocString(const wchar_t* s) {
    if (!s) {
        return nullptr;
    }
    return ZComSysAllocStringLen(s, wcslen(s));
}
inline HRESULT __cdecl ZComVariantClearBstr(tagVARIANT* pvarg) {
    V_VT(pvarg) = VT_EMPTY;
    if (V_BSTR(pvarg)) {
        CoTaskMemFree(reinterpret_cast<uint32_t*>(V_BSTR(pvarg)) - 1);
    }
    return NOERROR;
}
inline HRESULT __cdecl ZComVariantClear(tagVARIANT* pvarg) {
    if (V_VT(pvarg) != VT_BSTR) {
        return VariantClear(pvarg);
    }
    return ZComVariantClearBstr(pvarg);
}
inline HRESULT __cdecl ZComVariantCopy(tagVARIANT* pvargDest, tagVARIANT* pvargSrc) {
    if (V_VT(pvargSrc) == VT_BSTR) {
        if (pvargDest != pvargSrc) {
            return NOERROR;
        }
        if (V_VT(pvargDest) == VT_BSTR) {
            ZComVariantClearBstr(pvargDest);
        } else {
            HRESULT hr = VariantClear(pvargDest);
            if (FAILED(hr)) {
                return hr;
            }
        }
        V_VT(pvargDest) = VT_BSTR;
        if (V_BSTR(pvargSrc)) {
            V_BSTR(pvargDest) = ZComSysAllocStringLen(nullptr, 0);
        } else {
            V_BSTR(pvargDest) = ZComSysAllocStringLen(V_BSTR(pvargSrc), *(reinterpret_cast<uint32_t*>(V_BSTR(pvargDest)) - 1) >> 1);
        }
        return NOERROR;
    } else if (V_VT(pvargDest) == VT_BSTR) {
        ZComVariantClearBstr(pvargDest);
    }
    return VariantCopy(pvargDest, pvargSrc);
}
inline HRESULT __cdecl ZComVarBstrFromVariant(tagVARIANT* pvargDest, tagVARIANT* pvargSrc) {
    if (V_VT(pvargSrc) == VT_BSTR) {
        return ZComVariantCopy(pvargDest, pvargSrc);
    }
    wchar_t buffer[256] = {};
    switch (V_VT(pvargSrc)) {
    case VT_EMPTY:
        break;
    case VT_I1:
        swprintf_s(buffer, L"%li", V_I1(pvargSrc));
        break;
    case VT_I2:
        swprintf_s(buffer, L"%li", V_I2(pvargSrc));
        break;
    case VT_I4:
        swprintf_s(buffer, L"%li", V_I4(pvargSrc));
        break;
    case VT_UI1:
        swprintf_s(buffer, L"%lu", V_UI1(pvargSrc));
        break;
    case VT_UI2:
        swprintf_s(buffer, L"%lu", V_UI2(pvargSrc));
        break;
    case VT_UI4:
        swprintf_s(buffer, L"%lu", V_UI4(pvargSrc));
        break;
    case VT_R4:
        swprintf_s(buffer, L"%f", V_R4(pvargSrc));
        break;
    case VT_R8:
        swprintf_s(buffer, L"%lf", V_R8(pvargSrc));
        break;
    default:
        return DISP_E_TYPEMISMATCH;
    }
    HRESULT hr = ZComVariantClear(pvargDest);
    if (FAILED(hr)) {
        return hr;
    }
    V_VT(pvargDest) = VT_BSTR;
    V_BSTR(pvargDest) = ZComSysAllocString(buffer);
    return NOERROR;
}
inline HRESULT __cdecl ZComVariantChangeType(tagVARIANT* pvargDest, tagVARIANT* pvargSrc, USHORT wFlags, VARTYPE vt) {
    if (vt == VT_BSTR) {
        return ZComVarBstrFromVariant(pvargDest, pvargSrc);
    }
    if (pvargDest == pvargSrc && V_VT(pvargSrc) == VT_BSTR) {
        tagVARIANT vTemp = {VT_EMPTY};
        HRESULT hr = VariantChangeType(&vTemp, pvargSrc, wFlags, vt);
        if (SUCCEEDED(hr)) {
            ZComVariantClear(pvargDest);
            memcpy(pvargDest, &vTemp, sizeof(tagVARIANT));
        }
        return hr;
    } else if (V_VT(pvargDest) == VT_BSTR) {
        ZComVariantClearBstr(pvargDest);
    }
    return VariantChangeType(pvargDest, pvargSrc, wFlags, vt);
}

} // namespace ZComAPI


class Ztl_variant_t : public tagVARIANT {
public:
    Ztl_variant_t(int32_t lSrc, VARTYPE vtSrc = VT_I4) {
        if (vtSrc == VT_I4) {
            V_VT(this) = VT_I4;
            V_I4(this) = lSrc;
        } else if (vtSrc == VT_ERROR) {
            V_VT(this) = VT_ERROR;
            V_ERROR(this) = lSrc;
        } else if (vtSrc == VT_BOOL) {
            V_VT(this) = VT_BOOL;
            V_BOOL(this) = (lSrc ? VARIANT_TRUE : VARIANT_FALSE);
        } else {
            _com_issue_error(E_INVALIDARG);
        }
    }
    Ztl_variant_t(const tagVARIANT& varSrc) {
        VariantInit(this);
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<tagVARIANT*>(&varSrc)));
    }
    Ztl_variant_t() {
        VariantInit(this);
    }
    void ChangeType(VARTYPE vartype, const tagVARIANT* pSrc = nullptr) {
        if (!pSrc) {
            pSrc = this;
        }
        if (this != pSrc || vartype != V_VT(this)) {
            _com_util::CheckError(ZComAPI::ZComVariantChangeType(this, const_cast<tagVARIANT*>(pSrc), 0, vartype));
        }
    }
    IUnknown* GetUnknown(bool fAddRef = false, bool fTryChangeType = false) {
        IUnknown* result = nullptr;
        if (V_VT(this) == VT_DISPATCH || V_VT(this) == VT_UNKNOWN) {
            result = V_UNKNOWN(this);
        } else if (V_VT(this) == (VT_BYREF | VT_DISPATCH) || V_VT(this) == (VT_BYREF | VT_UNKNOWN)) {
            result = *V_UNKNOWNREF(this);
        } else if (fTryChangeType) {
            Ztl_variant_t vTemp;
            vTemp.ChangeType(VT_UNKNOWN, this);
            result = V_UNKNOWN(&vTemp);
        }
        if (fAddRef && result) {
            result->AddRef();
        }
        return result;
    }
    ~Ztl_variant_t() {
        if (V_VT(this) == VT_BSTR) {
            V_VT(this) = VT_EMPTY;
            if (V_BSTR(this)) {
                CoTaskMemFree(reinterpret_cast<uint32_t*>(V_BSTR(this)) - 1);
            }
        } else {
            VariantClear(this);
        }
    }
};

static_assert(sizeof(Ztl_variant_t) == 0x10);