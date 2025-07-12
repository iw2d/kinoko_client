#pragma once
#include <comdef.h>
#include <cstdint>


namespace ZComAPI {

inline BSTR ZComSysAllocString(const wchar_t* s) {
    if (s == nullptr) {
        return nullptr;
    }
    uint32_t u = wcslen(s);
    BSTR bstr = static_cast<wchar_t*>(CoTaskMemAlloc(sizeof(wchar_t) * (u + 1) + sizeof(uint32_t)));
    if (bstr) {
        *reinterpret_cast<uint32_t*>(bstr) = sizeof(wchar_t) * u;
        bstr = reinterpret_cast<wchar_t*>(reinterpret_cast<uint32_t*>(bstr) + 1);
        memcpy(bstr, s, sizeof(wchar_t) * (u + 1));
    }
    return bstr;
}

inline BSTR ZComSysAllocStringLen(const wchar_t* s, uint32_t u) {
    BSTR bstr = static_cast<wchar_t*>(CoTaskMemAlloc(sizeof(wchar_t) * (u + 1) + sizeof(uint32_t)));
    if (bstr) {
        *reinterpret_cast<uint32_t*>(bstr) = sizeof(wchar_t) * u;
        bstr = reinterpret_cast<wchar_t*>(reinterpret_cast<uint32_t*>(bstr) + 1);
        if (s) {
            memcpy(bstr, s, sizeof(wchar_t) * u);
        }
        bstr[u] = 0;
    }
    return bstr;
}

inline BSTR ZComSysAllocStringByteLen(const char* s, uint32_t bn) {
    BSTR bstr = static_cast<wchar_t*>(CoTaskMemAlloc(bn + sizeof(wchar_t) + sizeof(uint32_t)));
    if (bstr) {
        *reinterpret_cast<uint32_t*>(bstr) = bn;
        bstr = reinterpret_cast<wchar_t*>(reinterpret_cast<uint32_t*>(bstr) + 1);
        if (s) {
            memcpy(bstr, s, bn);
        }
        bstr[bn / sizeof(wchar_t)] = 0;
    }
    return bstr;
}

inline void ZComSysFreeString(BSTR bstr) {
    if (bstr) {
        CoTaskMemFree(reinterpret_cast<uint32_t*>(bstr) - 1);
    }
}

inline uint32_t ZComSysStringLen(BSTR bstr) {
    if (!bstr) {
        return 0;
    }
    return *(reinterpret_cast<uint32_t*>(bstr) - 1) / sizeof(wchar_t);
}

inline uint32_t ZComSysStringByteLen(BSTR bstr) {
    if (!bstr) {
        return 0;
    }
    return *(reinterpret_cast<uint32_t*>(bstr) - 1);
}

inline void ZComVariantInit(VARIANT* pvarg) {
    VariantInit(pvarg);
}

inline HRESULT ZComVariantClear(VARIANT* pvarg) {
    if (V_VT(pvarg) != VT_BSTR) {
        return VariantClear(pvarg);
    }
    V_VT(pvarg) = VT_EMPTY;
    if (V_BSTR(pvarg)) {
        ZComSysFreeString(V_BSTR(pvarg));
    }
    return S_OK;
}

inline HRESULT ZComVariantCopy(VARIANT* pvargDest, VARIANT* pvargSrc) {
    if (V_VT(pvargSrc) == VT_BSTR) {
        if (pvargDest == pvargSrc) {
            return S_OK;
        }
        HRESULT hr = ZComVariantClear(pvargDest);
        if (FAILED(hr)) {
            return hr;
        }
        V_VT(pvargDest) = VT_BSTR;
        BSTR bstr = V_BSTR(pvargSrc);
        if (bstr) {
            V_BSTR(pvargDest) = ZComSysAllocStringLen(bstr, ZComSysStringLen(bstr));
        } else {
            V_BSTR(pvargDest) = ZComSysAllocStringLen(nullptr, 0);
        }
    } else if (V_VT(pvargDest) == VT_BSTR) {
        V_VT(pvargDest) = VT_EMPTY;
        if (V_BSTR(pvargDest)) {
            ZComSysFreeString(V_BSTR(pvargDest));
        }
    }
    return VariantCopy(pvargDest, pvargSrc);
}

inline HRESULT ZComVarBstrFromVariant(VARIANT* pvargDest, VARIANT* pvarSrc) {
    if (V_VT(pvarSrc) == VT_BSTR) {
        return ZComVariantCopy(pvargDest, pvarSrc);
    }
    wchar_t buf[256];
    memset(buf, 0, sizeof(buf));
    switch (V_VT(pvarSrc)) {
    case VT_EMPTY:
        break;
    case VT_I1:
        swprintf_s(buf, 256, L"%li", V_I1(pvarSrc));
        break;
    case VT_I2:
        swprintf_s(buf, 256, L"%li", V_I2(pvarSrc));
        break;
    case VT_I4:
        swprintf_s(buf, 256, L"%li", V_I4(pvarSrc));
        break;
    case VT_UI1:
        swprintf_s(buf, 256, L"%lu", V_UI1(pvarSrc));
        break;
    case VT_UI2:
        swprintf_s(buf, 256, L"%lu", V_UI2(pvarSrc));
        break;
    case VT_UI4:
        swprintf_s(buf, 256, L"%lu", V_UI4(pvarSrc));
        break;
    case VT_R4:
        swprintf_s(buf, 256, L"%f", V_R4(pvarSrc));
        break;
    case VT_R8:
        swprintf_s(buf, 256, L"%lf", V_R8(pvarSrc));
        break;
    default:
        return DISP_E_TYPEMISMATCH;
    }
    HRESULT hr = ZComVariantClear(pvargDest);
    if (FAILED(hr)) {
        return hr;
    }
    V_VT(pvargDest) = VT_BSTR;
    V_BSTR(pvargDest) = ZComSysAllocString(buf);
    return S_OK;
}

inline HRESULT ZComVariantChangeType(VARIANT* pvargDest, VARIANT* pvarSrc, USHORT wFlags, VARTYPE vt) {
    if (vt == VT_BSTR) {
        return ZComVarBstrFromVariant(pvargDest, pvarSrc);
    }
    if (pvargDest == pvarSrc && V_VT(pvarSrc) == VT_BSTR) {
        VARIANT vTemp;
        V_VT(&vTemp) = VT_EMPTY;
        HRESULT hr = VariantChangeType(&vTemp, pvarSrc, wFlags, vt);
        if (SUCCEEDED(hr)) {
            ZComVariantClear(pvargDest);
            *pvargDest = vTemp;
        }
        return hr;
    } else {
        if (V_VT(pvargDest) == VT_BSTR) {
            V_VT(pvargDest) = VT_EMPTY;
            if (V_BSTR(pvargDest)) {
                ZComSysFreeString(V_BSTR(pvargDest));
            }
        }
        return VariantChangeType(pvargDest, pvarSrc, wFlags, vt);
    }
}

} // namespace ZComAPI


namespace _com_util {

inline BSTR __stdcall ZtlConvertStringToBSTR(const char* pSrc) {
    if (!pSrc) {
        return nullptr;
    }
    int n = MultiByteToWideChar(0, 0, pSrc, -1, nullptr, 0) - 1;
    BSTR bstr = static_cast<wchar_t*>(CoTaskMemAlloc(sizeof(wchar_t) * (n + 1) + sizeof(uint32_t)));
    if (bstr) {
        *reinterpret_cast<uint32_t*>(bstr) = sizeof(wchar_t) * n;
        bstr = reinterpret_cast<wchar_t*>(reinterpret_cast<uint32_t*>(bstr) + 1);
        bstr[n] = 0;
        MultiByteToWideChar(0, 0, pSrc, -1, bstr, 0x3FFFFFFF);
    }
    return bstr;
}

inline char* __stdcall ZtlConvertBSTRToString(BSTR pSrc) {
    if (!pSrc) {
        return nullptr;
    }
    int n = WideCharToMultiByte(0, 0, pSrc, -1, 0, 0, nullptr, nullptr);
    char* s = new char[n];
    WideCharToMultiByte(0, 0, pSrc, -1, s, 0x3FFFFFFF, nullptr, nullptr);
    return s;
}

} // namespace _com_util


class Ztl_bstr_t;
class Ztl_variant_t;


class Ztl_bstr_t {
public:
    Ztl_bstr_t() throw();
    Ztl_bstr_t(const Ztl_bstr_t& s) throw();
    Ztl_bstr_t(const char* s);
    Ztl_bstr_t(const wchar_t* s);
    Ztl_bstr_t(const Ztl_variant_t& var);
    Ztl_bstr_t(BSTR bstr, bool fCopy);
    ~Ztl_bstr_t() throw();
    Ztl_bstr_t& operator=(const Ztl_bstr_t& s) throw();
    Ztl_bstr_t& operator=(const char* s);
    Ztl_bstr_t& operator=(const wchar_t* s);
    Ztl_bstr_t& operator=(const Ztl_variant_t& var);
    Ztl_bstr_t& operator+=(const Ztl_bstr_t& s);
    Ztl_bstr_t operator+(const Ztl_bstr_t& s) const;
    friend Ztl_bstr_t operator+(const char* s1, const Ztl_bstr_t& s2);
    friend Ztl_bstr_t operator+(const wchar_t* s1, const Ztl_bstr_t& s2);
    operator const wchar_t*() const throw();
    operator wchar_t*() const throw();
    operator const char*() const;
    operator char*() const;
    bool operator!() const throw();
    bool operator==(const Ztl_bstr_t& str) const throw();
    bool operator!=(const Ztl_bstr_t& str) const throw();
    bool operator<(const Ztl_bstr_t& str) const throw();
    bool operator>(const Ztl_bstr_t& str) const throw();
    bool operator<=(const Ztl_bstr_t& str) const throw();
    bool operator>=(const Ztl_bstr_t& str) const throw();
    BSTR copy(bool fCopy = true) const;
    unsigned int length() const throw();
    void Assign(BSTR s);
    BSTR& GetBSTR();
    BSTR* GetAddress();
    void Attach(BSTR s);
    BSTR Detach() throw();

private:
    class Data_t {
    public:
        Data_t(const char* s);
        Data_t(const wchar_t* s);
        Data_t(BSTR bstr, bool fCopy);
        Data_t(const Ztl_bstr_t& s1, const Ztl_bstr_t& s2);
        unsigned long AddRef() throw();
        unsigned long Release() throw();
        unsigned long RefCount() const throw();
        operator const wchar_t*() const throw();
        operator const char*() const;
        const wchar_t* GetWString() const throw();
        wchar_t*& GetWString() throw();
        const char* GetString() const;
        BSTR Copy() const;
        void Assign(BSTR s);
        void Attach(BSTR s) throw();
        unsigned int Length() const throw();
        int Compare(const Data_t& str) const throw();

    private:
        BSTR m_wstr;
        mutable char* m_str;
        unsigned long m_RefCount;
        Data_t() throw();
        Data_t(const Data_t& s) throw();
        ~Data_t() throw();
        void _Free() throw();
    };

    Data_t* m_Data;

    void _AddRef() throw();
    void _Free() throw();
    int _Compare(const Ztl_bstr_t& str) const throw();
};

static_assert(sizeof(Ztl_bstr_t) == 0x4);


inline Ztl_bstr_t::Ztl_bstr_t() throw() : m_Data(NULL) {}

inline Ztl_bstr_t::Ztl_bstr_t(const Ztl_bstr_t& s) throw() : m_Data(s.m_Data) {
    _AddRef();
}

inline Ztl_bstr_t::Ztl_bstr_t(const char* s) : m_Data(new Data_t(s)) {
    if (!m_Data) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline Ztl_bstr_t::Ztl_bstr_t(const wchar_t* s) : m_Data(new Data_t(s)) {
    if (!m_Data) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline Ztl_bstr_t::Ztl_bstr_t(BSTR bstr, bool fCopy) : m_Data(new Data_t(bstr, fCopy)) {
    if (!m_Data) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline Ztl_bstr_t::~Ztl_bstr_t() throw() {
    _Free();
}

inline Ztl_bstr_t& Ztl_bstr_t::operator=(const Ztl_bstr_t& s) throw() {
    if (this != &s) {
        _Free();
        m_Data = s.m_Data;
        _AddRef();
    }
    return *this;
}

inline Ztl_bstr_t& Ztl_bstr_t::operator=(const char* s) {
    if (!s || static_cast<const char*>(*this) != s) {
        _Free();
        m_Data = new Data_t(s);
        if (!m_Data) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return *this;
}

inline Ztl_bstr_t& Ztl_bstr_t::operator=(const wchar_t* s) {
    if (!s || static_cast<const wchar_t*>(*this) != s) {
        _Free();
        m_Data = new Data_t(s);
        if (!m_Data) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return *this;
}

inline Ztl_bstr_t& Ztl_bstr_t::operator+=(const Ztl_bstr_t& s) {
    Data_t* newData = new Data_t(*this, s);
    if (!newData) {
        _com_issue_error(E_OUTOFMEMORY);
    } else {
        _Free();
        m_Data = newData;
    }
    return *this;
}

inline Ztl_bstr_t Ztl_bstr_t::operator+(const Ztl_bstr_t& s) const {
    Ztl_bstr_t b = *this;
    b += s;
    return b;
}

inline Ztl_bstr_t operator+(const char* s1, const Ztl_bstr_t& s2) {
    Ztl_bstr_t b = s1;
    b += s2;
    return b;
}

inline Ztl_bstr_t operator+(const wchar_t* s1, const Ztl_bstr_t& s2) {
    Ztl_bstr_t b = s1;
    b += s2;
    return b;
}

inline Ztl_bstr_t::operator const wchar_t*() const throw() {
    return (m_Data != NULL) ? m_Data->GetWString() : NULL;
}

inline Ztl_bstr_t::operator wchar_t*() const throw() {
    return const_cast<wchar_t*>((m_Data != NULL) ? m_Data->GetWString() : NULL);
}

inline Ztl_bstr_t::operator const char*() const {
    return (m_Data != NULL) ? m_Data->GetString() : NULL;
}

inline Ztl_bstr_t::operator char*() const {
    return const_cast<char*>((m_Data != NULL) ? m_Data->GetString() : NULL);
}

inline bool Ztl_bstr_t::operator!() const throw() {
    return (m_Data != NULL) ? !m_Data->GetWString() : true;
}

inline bool Ztl_bstr_t::operator==(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) == 0;
}

inline bool Ztl_bstr_t::operator!=(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) != 0;
}

inline bool Ztl_bstr_t::operator<(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) < 0;
}

inline bool Ztl_bstr_t::operator>(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) > 0;
}

inline bool Ztl_bstr_t::operator<=(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) <= 0;
}

inline bool Ztl_bstr_t::operator>=(const Ztl_bstr_t& str) const throw() {
    return _Compare(str) >= 0;
}

inline BSTR Ztl_bstr_t::copy(bool fCopy) const {
    return (m_Data != NULL) ? (fCopy ? m_Data->Copy() : m_Data->GetWString()) : NULL;
}

inline unsigned int Ztl_bstr_t::length() const throw() {
    return (m_Data != NULL) ? m_Data->Length() : 0;
}

inline void Ztl_bstr_t::Assign(BSTR s) {
    if (!s || !m_Data || m_Data->GetWString() != s) {
        _Free();
        m_Data = new Data_t(s, TRUE);
        if (!m_Data) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
}

inline BSTR& Ztl_bstr_t::GetBSTR() {
    if (!m_Data) {
        m_Data = new Data_t(0, FALSE);
        if (!m_Data) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return m_Data->GetWString();
}

inline BSTR* Ztl_bstr_t::GetAddress() {
    Attach(0);
    return &m_Data->GetWString();
}

inline void Ztl_bstr_t::Attach(BSTR s) {
    _Free();
    m_Data = new Data_t(s, FALSE);
    if (!m_Data) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline BSTR Ztl_bstr_t::Detach() throw() {
    if (m_Data != NULL && m_Data->RefCount() == 1) {
        BSTR b = m_Data->GetWString();
        m_Data->GetWString() = NULL;
        _Free();
        return b;
    } else {
        _com_issue_error(E_POINTER);
        return NULL;
    }
}

inline void Ztl_bstr_t::_AddRef() throw() {
    if (m_Data != NULL)
        m_Data->AddRef();
}

inline void Ztl_bstr_t::_Free() throw() {
    if (m_Data != NULL) {
        m_Data->Release();
        m_Data = NULL;
    }
}

inline int Ztl_bstr_t::_Compare(const Ztl_bstr_t& str) const throw() {
    if (m_Data == str.m_Data)
        return 0;
    if (!m_Data)
        return -1;
    if (!str.m_Data)
        return 1;
    return m_Data->Compare(*str.m_Data);
}

inline Ztl_bstr_t::Data_t::Data_t(const char* s) : m_str(NULL), m_RefCount(1) {
    m_wstr = _com_util::ZtlConvertStringToBSTR(s);
}

inline Ztl_bstr_t::Data_t::Data_t(const wchar_t* s) : m_str(NULL), m_RefCount(1) {
    m_wstr = ZComAPI::ZComSysAllocString(s);
    if (!m_wstr && s != NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline Ztl_bstr_t::Data_t::Data_t(BSTR bstr, bool fCopy) : m_str(NULL), m_RefCount(1) {
    if (fCopy && bstr != NULL) {
        m_wstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr), ZComAPI::ZComSysStringByteLen(bstr));
        if (!m_wstr) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    } else
        m_wstr = bstr;
}

inline Ztl_bstr_t::Data_t::Data_t(const Ztl_bstr_t& s1, const Ztl_bstr_t& s2) : m_str(NULL), m_RefCount(1) {
    const unsigned int l1 = s1.length();
    const unsigned int l2 = s2.length();
    m_wstr = ZComAPI::ZComSysAllocStringByteLen(NULL, (l1 + l2) * sizeof(wchar_t));
    if (!m_wstr) {
        _com_issue_error(E_OUTOFMEMORY);
        return;
    }
    const wchar_t* wstr1 = static_cast<const wchar_t*>(s1);
    if (wstr1 != NULL) {
        memcpy_s(m_wstr, (l1 + l2 + 1) * sizeof(wchar_t), wstr1, (l1 + 1) * sizeof(wchar_t));
    }
    const wchar_t* wstr2 = static_cast<const wchar_t*>(s2);
    if (wstr2 != NULL) {
        memcpy_s(m_wstr + l1, (l2 + 1) * sizeof(wchar_t), wstr2, (l2 + 1) * sizeof(wchar_t));
    }
}

inline unsigned long Ztl_bstr_t::Data_t::AddRef() throw() {
    InterlockedIncrement(reinterpret_cast<LONG*>(&m_RefCount));
    return m_RefCount;
}

inline unsigned long Ztl_bstr_t::Data_t::Release() throw() {
    unsigned long cRef = InterlockedDecrement(&m_RefCount);
    if (cRef == 0)
        delete this;
    return cRef;
}

inline unsigned long Ztl_bstr_t::Data_t::RefCount() const throw() {
    return m_RefCount;
}

inline Ztl_bstr_t::Data_t::operator const wchar_t*() const throw() {
    return m_wstr;
}

inline Ztl_bstr_t::Data_t::operator const char*() const {
    return GetString();
}

inline const wchar_t* Ztl_bstr_t::Data_t::GetWString() const throw() {
    return m_wstr;
}

inline wchar_t*& Ztl_bstr_t::Data_t::GetWString() throw() {
    return m_wstr;
}

inline const char* Ztl_bstr_t::Data_t::GetString() const {
    if (!m_str)
        m_str = _com_util::ZtlConvertBSTRToString(m_wstr);
    return m_str;
}

inline BSTR Ztl_bstr_t::Data_t::Copy() const {
    if (m_wstr != NULL) {
        BSTR bstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(m_wstr), ZComAPI::ZComSysStringByteLen(m_wstr));
        if (!bstr) {
            _com_issue_error(E_OUTOFMEMORY);
        }
        return bstr;
    }
    return NULL;
}

inline void Ztl_bstr_t::Data_t::Assign(BSTR s) {
    _Free();
    if (s != NULL) {
        m_wstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(s), ZComAPI::ZComSysStringByteLen(s));
        m_str = 0;
    }
}

inline void Ztl_bstr_t::Data_t::Attach(BSTR s) throw() {
    _Free();
    m_wstr = s;
    m_str = 0;
    m_RefCount = 1;
}

inline unsigned int Ztl_bstr_t::Data_t::Length() const throw() {
    return m_wstr ? ZComAPI::ZComSysStringLen(m_wstr) : 0;
}

inline int Ztl_bstr_t::Data_t::Compare(const Ztl_bstr_t::Data_t& str) const throw() {
    if (!m_wstr)
        return str.m_wstr ? -1 : 0;
    if (!str.m_wstr)
        return 1;
    const unsigned int l1 = ZComAPI::ZComSysStringLen(m_wstr);
    const unsigned int l2 = ZComAPI::ZComSysStringLen(str.m_wstr);
    unsigned int len = l1;
    if (len > l2)
        len = l2;
    BSTR bstr1 = m_wstr;
    BSTR bstr2 = str.m_wstr;
    while (len-- > 0) {
        if (*bstr1++ != *bstr2++)
            return bstr1[-1] - bstr2[-1];
    }
    return (l1 < l2) ? -1 : ((l1 == l2) ? 0 : 1);
}

inline Ztl_bstr_t::Data_t::~Data_t() throw() {
    _Free();
}

inline void Ztl_bstr_t::Data_t::_Free() throw() {
    if (m_wstr != NULL)
        ZComAPI::ZComSysFreeString(m_wstr);
    if (m_str != NULL)
        delete[] m_str;
}


class Ztl_variant_t : public tagVARIANT {
public:
    Ztl_variant_t() throw();
    Ztl_variant_t(const VARIANT& varSrc);
    Ztl_variant_t(const VARIANT* pSrc);
    Ztl_variant_t(const Ztl_variant_t& varSrc);
    Ztl_variant_t(VARIANT& varSrc, bool fCopy);
    Ztl_variant_t(short sSrc, VARTYPE vtSrc = VT_I2);
    Ztl_variant_t(long lSrc, VARTYPE vtSrc = VT_I4);
    Ztl_variant_t(float fltSrc) throw();
    Ztl_variant_t(double dblSrc, VARTYPE vtSrc = VT_R8);
    Ztl_variant_t(const CY& cySrc) throw();
    Ztl_variant_t(const Ztl_bstr_t& bstrSrc);
    Ztl_variant_t(const wchar_t* pSrc);
    Ztl_variant_t(const char* pSrc);
    Ztl_variant_t(IDispatch* pSrc, bool fAddRef = true) throw();
    Ztl_variant_t(bool boolSrc) throw();
    Ztl_variant_t(IUnknown* pSrc, bool fAddRef = true) throw();
    Ztl_variant_t(const DECIMAL& decSrc) throw();
    Ztl_variant_t(BYTE bSrc) throw();
    Ztl_variant_t(char cSrc) throw();
    Ztl_variant_t(unsigned short usSrc) throw();
    Ztl_variant_t(unsigned long ulSrc) throw();
    Ztl_variant_t(int iSrc) throw();
    Ztl_variant_t(unsigned int uiSrc) throw();
    ~Ztl_variant_t() throw();
    operator short() const;
    operator long() const;
    operator float() const;
    operator double() const;
    operator CY() const;
    operator Ztl_bstr_t() const;
    operator IDispatch*() const;
    operator bool() const;
    operator IUnknown*() const;
    operator DECIMAL() const;
    operator BYTE() const;
    operator VARIANT() const throw();
    operator char() const;
    operator unsigned short() const;
    operator unsigned long() const;
    operator int() const;
    operator unsigned int() const;
    Ztl_variant_t& operator=(const VARIANT& varSrc);
    Ztl_variant_t& operator=(const VARIANT* pSrc);
    Ztl_variant_t& operator=(const Ztl_variant_t& varSrc);
    Ztl_variant_t& operator=(short sSrc);
    Ztl_variant_t& operator=(long lSrc);
    Ztl_variant_t& operator=(float fltSrc);
    Ztl_variant_t& operator=(double dblSrc);
    Ztl_variant_t& operator=(const CY& cySrc);
    Ztl_variant_t& operator=(const Ztl_bstr_t& bstrSrc);
    Ztl_variant_t& operator=(const wchar_t* pSrc);
    Ztl_variant_t& operator=(const char* pSrc);
    Ztl_variant_t& operator=(IDispatch* pSrc);
    Ztl_variant_t& operator=(bool boolSrc);
    Ztl_variant_t& operator=(IUnknown* pSrc);
    Ztl_variant_t& operator=(const DECIMAL& decSrc);
    Ztl_variant_t& operator=(BYTE bSrc);
    Ztl_variant_t& operator=(char cSrc);
    Ztl_variant_t& operator=(unsigned short usSrc);
    Ztl_variant_t& operator=(unsigned long ulSrc);
    Ztl_variant_t& operator=(int iSrc);
    Ztl_variant_t& operator=(unsigned int uiSrc);
    bool operator==(const VARIANT& varSrc) const throw();
    bool operator==(const VARIANT* pSrc) const throw();
    bool operator!=(const VARIANT& varSrc) const throw();
    bool operator!=(const VARIANT* pSrc) const throw();
    void Clear();
    void Attach(VARIANT& varSrc);
    VARIANT Detach();
    VARIANT& GetVARIANT() throw();
    VARIANT* GetAddress();
    void ChangeType(VARTYPE vartype, const Ztl_variant_t* pSrc = NULL);
    void SetString(const char* pSrc);

    IUnknown* GetUnknown(bool fAddRef = false, bool fTryChangeType = false) const {
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
};

static_assert(sizeof(Ztl_variant_t) == 0x10);


inline Ztl_variant_t::Ztl_variant_t() throw() {
    ZComAPI::ZComVariantInit(this);
}

inline Ztl_variant_t::Ztl_variant_t(const VARIANT& varSrc) {
    ZComAPI::ZComVariantInit(this);
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(&varSrc)));
}

inline Ztl_variant_t::Ztl_variant_t(const VARIANT* pSrc) {
    if (!pSrc) {
        _com_issue_error(E_POINTER);
    } else {
        ZComAPI::ZComVariantInit(this);
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(pSrc)));
    }
}

inline Ztl_variant_t::Ztl_variant_t(const Ztl_variant_t& varSrc) {
    ZComAPI::ZComVariantInit(this);
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc))));
}

inline Ztl_variant_t::Ztl_variant_t(VARIANT& varSrc, bool fCopy) {
    if (fCopy) {
        ZComAPI::ZComVariantInit(this);
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, &varSrc));
    } else {
        _COM_MEMCPY_S(this, sizeof(varSrc), &varSrc, sizeof(varSrc));
        V_VT(&varSrc) = VT_EMPTY;
    }
}

inline Ztl_variant_t::Ztl_variant_t(short sSrc, VARTYPE vtSrc) {
    if ((vtSrc != VT_I2) && (vtSrc != VT_BOOL)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }
    if (vtSrc == VT_BOOL) {
        V_VT(this) = VT_BOOL;
        V_BOOL(this) = (sSrc ? VARIANT_TRUE : VARIANT_FALSE);
    } else {
        V_VT(this) = VT_I2;
        V_I2(this) = sSrc;
    }
}

inline Ztl_variant_t::Ztl_variant_t(long lSrc, VARTYPE vtSrc) {
    if ((vtSrc != VT_I4) && (vtSrc != VT_ERROR) && (vtSrc != VT_BOOL)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }
    if (vtSrc == VT_ERROR) {
        V_VT(this) = VT_ERROR;
        V_ERROR(this) = lSrc;
    } else if (vtSrc == VT_BOOL) {
        V_VT(this) = VT_BOOL;
        V_BOOL(this) = (lSrc ? VARIANT_TRUE : VARIANT_FALSE);
    } else {
        V_VT(this) = VT_I4;
        V_I4(this) = lSrc;
    }
}

inline Ztl_variant_t::Ztl_variant_t(float fltSrc) throw() {
    V_VT(this) = VT_R4;
    V_R4(this) = fltSrc;
}

inline Ztl_variant_t::Ztl_variant_t(double dblSrc, VARTYPE vtSrc) {
    if ((vtSrc != VT_R8) && (vtSrc != VT_DATE)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }
    if (vtSrc == VT_DATE) {
        V_VT(this) = VT_DATE;
        V_DATE(this) = dblSrc;
    } else {
        V_VT(this) = VT_R8;
        V_R8(this) = dblSrc;
    }
}

inline Ztl_variant_t::Ztl_variant_t(const CY& cySrc) throw() {
    V_VT(this) = VT_CY;
    V_CY(this) = cySrc;
}

inline Ztl_variant_t::Ztl_variant_t(const Ztl_bstr_t& bstrSrc) {
    V_VT(this) = VT_BSTR;
    BSTR bstr = static_cast<wchar_t*>(bstrSrc);
    if (!bstr)
        V_BSTR(this) = NULL;
    else {
        V_BSTR(this) = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr), ZComAPI::ZComSysStringByteLen(bstr));
        if (!(V_BSTR(this))) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
}

inline Ztl_variant_t::Ztl_variant_t(const wchar_t* pSrc) {
    V_VT(this) = VT_BSTR;
    V_BSTR(this) = ZComAPI::ZComSysAllocString(pSrc);
    if (!(V_BSTR(this)) && pSrc != NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

inline Ztl_variant_t::Ztl_variant_t(const char* pSrc) {
    V_VT(this) = VT_BSTR;
    V_BSTR(this) = _com_util::ConvertStringToBSTR(pSrc);
}

inline Ztl_variant_t::Ztl_variant_t(IDispatch* pSrc, bool fAddRef) throw() {
    V_VT(this) = VT_DISPATCH;
    V_DISPATCH(this) = pSrc;
    if (fAddRef && V_DISPATCH(this) != NULL)
        V_DISPATCH(this)->AddRef();
}

inline Ztl_variant_t::Ztl_variant_t(bool boolSrc) throw() {
    V_VT(this) = VT_BOOL;
    V_BOOL(this) = (boolSrc ? VARIANT_TRUE : VARIANT_FALSE);
}

inline Ztl_variant_t::Ztl_variant_t(IUnknown* pSrc, bool fAddRef) throw() {
    V_VT(this) = VT_UNKNOWN;
    V_UNKNOWN(this) = pSrc;
    if (fAddRef && V_UNKNOWN(this) != NULL)
        V_UNKNOWN(this)->AddRef();
}

inline Ztl_variant_t::Ztl_variant_t(const DECIMAL& decSrc) throw() {
    V_DECIMAL(this) = decSrc;
    V_VT(this) = VT_DECIMAL;
}

inline Ztl_variant_t::Ztl_variant_t(BYTE bSrc) throw() {
    V_VT(this) = VT_UI1;
    V_UI1(this) = bSrc;
}

inline Ztl_variant_t::Ztl_variant_t(char cSrc) throw() {
    V_VT(this) = VT_I1;
    V_I1(this) = cSrc;
}

inline Ztl_variant_t::Ztl_variant_t(unsigned short usSrc) throw() {
    V_VT(this) = VT_UI2;
    V_UI2(this) = usSrc;
}

inline Ztl_variant_t::Ztl_variant_t(unsigned long ulSrc) throw() {
    V_VT(this) = VT_UI4;
    V_UI4(this) = ulSrc;
}

inline Ztl_variant_t::Ztl_variant_t(int iSrc) throw() {
    V_VT(this) = VT_INT;
    V_INT(this) = iSrc;
}

inline Ztl_variant_t::Ztl_variant_t(unsigned int uiSrc) throw() {
    V_VT(this) = VT_UINT;
    V_UINT(this) = uiSrc;
}

inline Ztl_variant_t::operator short() const {
    if (V_VT(this) == VT_I2)
        return V_I2(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_I2, this);
    return V_I2(&varDest);
}

inline Ztl_variant_t::operator long() const {
    if (V_VT(this) == VT_I4)
        return V_I4(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_I4, this);
    return V_I4(&varDest);
}

inline Ztl_variant_t::operator float() const {
    if (V_VT(this) == VT_R4)
        return V_R4(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_R4, this);
    return V_R4(&varDest);
}

inline Ztl_variant_t::operator double() const {
    if (V_VT(this) == VT_R8)
        return V_R8(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_R8, this);
    return V_R8(&varDest);
}

inline Ztl_variant_t::operator CY() const {
    if (V_VT(this) == VT_CY)
        return V_CY(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_CY, this);
    return V_CY(&varDest);
}

inline Ztl_variant_t::operator Ztl_bstr_t() const {
    if (V_VT(this) == VT_BSTR)
        return V_BSTR(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_BSTR, this);
    return V_BSTR(&varDest);
}

inline Ztl_variant_t::operator IDispatch*() const {
    if (V_VT(this) == VT_DISPATCH) {
        if (V_DISPATCH(this) != NULL)
            V_DISPATCH(this)->AddRef();
        return V_DISPATCH(this);
    }
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_DISPATCH, this);
    if (V_DISPATCH(&varDest) != NULL)
        V_DISPATCH(&varDest)->AddRef();
    return V_DISPATCH(&varDest);
}

inline Ztl_variant_t::operator bool() const {
    if (V_VT(this) == VT_BOOL)
        return V_BOOL(this) ? true : false;
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_BOOL, this);
    return (V_BOOL(&varDest) == VARIANT_TRUE) ? true : false;
}

inline Ztl_variant_t::operator IUnknown*() const {
    if (V_VT(this) == VT_UNKNOWN) {
        if (V_UNKNOWN(this) != NULL)
            V_UNKNOWN(this)->AddRef();
        return V_UNKNOWN(this);
    }
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_UNKNOWN, this);
    if (V_UNKNOWN(&varDest) != NULL)
        V_UNKNOWN(&varDest)->AddRef();
    return V_UNKNOWN(&varDest);
}

inline Ztl_variant_t::operator DECIMAL() const {
    if (V_VT(this) == VT_DECIMAL)
        return V_DECIMAL(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_DECIMAL, this);
    return V_DECIMAL(&varDest);
}

inline Ztl_variant_t::operator BYTE() const {
    if (V_VT(this) == VT_UI1)
        return V_UI1(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_UI1, this);
    return V_UI1(&varDest);
}

inline Ztl_variant_t::operator VARIANT() const throw() {
    return *(VARIANT*)this;
}

inline Ztl_variant_t::operator char() const {
    if (V_VT(this) == VT_I1)
        return V_I1(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_I1, this);
    return V_I1(&varDest);
}

inline Ztl_variant_t::operator unsigned short() const {
    if (V_VT(this) == VT_UI2)
        return V_UI2(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_UI2, this);
    return V_UI2(&varDest);
}

inline Ztl_variant_t::operator unsigned long() const {
    if (V_VT(this) == VT_UI4)
        return V_UI4(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_UI4, this);
    return V_UI4(&varDest);
}

inline Ztl_variant_t::operator int() const {
    if (V_VT(this) == VT_INT)
        return V_INT(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_INT, this);
    return V_INT(&varDest);
}

inline Ztl_variant_t::operator unsigned int() const {
    if (V_VT(this) == VT_UINT)
        return V_UINT(this);
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_UINT, this);
    return V_UINT(&varDest);
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const VARIANT& varSrc) {
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(&varSrc)));
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const VARIANT* pSrc) {
    if (!pSrc) {
        _com_issue_error(E_POINTER);
    } else {
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(pSrc)));
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const Ztl_variant_t& varSrc) {
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc))));
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(short sSrc) {
    if (V_VT(this) == VT_I2)
        V_I2(this) = sSrc;
    else if (V_VT(this) == VT_BOOL)
        V_BOOL(this) = (sSrc ? VARIANT_TRUE : VARIANT_FALSE);
    else {
        Clear();
        V_VT(this) = VT_I2;
        V_I2(this) = sSrc;
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(long lSrc) {
    if (V_VT(this) == VT_I4)
        V_I4(this) = lSrc;
    else if (V_VT(this) == VT_ERROR)
        V_ERROR(this) = lSrc;
    else if (V_VT(this) == VT_BOOL)
        V_BOOL(this) = (lSrc ? VARIANT_TRUE : VARIANT_FALSE);
    else {
        Clear();
        V_VT(this) = VT_I4;
        V_I4(this) = lSrc;
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(float fltSrc) {
    if (V_VT(this) != VT_R4) {
        Clear();
        V_VT(this) = VT_R4;
    }
    V_R4(this) = fltSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(double dblSrc) {
    if (V_VT(this) == VT_R8) {
        V_R8(this) = dblSrc;
    } else if (V_VT(this) == VT_DATE) {
        V_DATE(this) = dblSrc;
    } else {
        Clear();
        V_VT(this) = VT_R8;
        V_R8(this) = dblSrc;
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const CY& cySrc) {
    if (V_VT(this) != VT_CY) {
        Clear();
        V_VT(this) = VT_CY;
    }
    V_CY(this) = cySrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const Ztl_bstr_t& bstrSrc) {
    Clear();
    V_VT(this) = VT_BSTR;
    if (!bstrSrc) {
        V_BSTR(this) = NULL;
    } else {
        BSTR bstr = static_cast<wchar_t*>(bstrSrc);
        V_BSTR(this) = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr), ZComAPI::ZComSysStringByteLen(bstr));
        if (!(V_BSTR(this))) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const wchar_t* pSrc) {
    Clear();
    V_VT(this) = VT_BSTR;
    if (!pSrc) {
        V_BSTR(this) = NULL;
    } else {
        V_BSTR(this) = ZComAPI::ZComSysAllocString(pSrc);
        if (!(V_BSTR(this))) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const char* pSrc) {
    Clear();
    V_VT(this) = VT_BSTR;
    V_BSTR(this) = _com_util::ConvertStringToBSTR(pSrc);
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(IDispatch* pSrc) {
    Clear();
    V_VT(this) = VT_DISPATCH;
    V_DISPATCH(this) = pSrc;
    if (V_DISPATCH(this) != NULL) {
        V_DISPATCH(this)->AddRef();
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(bool boolSrc) {
    if (V_VT(this) != VT_BOOL) {
        Clear();
        V_VT(this) = VT_BOOL;
    }
    V_BOOL(this) = (boolSrc ? VARIANT_TRUE : VARIANT_FALSE);
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(IUnknown* pSrc) {
    Clear();
    V_VT(this) = VT_UNKNOWN;
    V_UNKNOWN(this) = pSrc;
    if (V_UNKNOWN(this) != NULL) {
        V_UNKNOWN(this)->AddRef();
    }
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(const DECIMAL& decSrc) {
    if (V_VT(this) != VT_DECIMAL) {
        Clear();
    }
    V_DECIMAL(this) = decSrc;
    V_VT(this) = VT_DECIMAL;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(BYTE bSrc) {
    if (V_VT(this) != VT_UI1) {
        Clear();
        V_VT(this) = VT_UI1;
    }
    V_UI1(this) = bSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(char cSrc) {
    if (V_VT(this) != VT_I1) {
        Clear();
        V_VT(this) = VT_I1;
    }
    V_I1(this) = cSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(unsigned short usSrc) {
    if (V_VT(this) != VT_UI2) {
        Clear();
        V_VT(this) = VT_UI2;
    }
    V_UI2(this) = usSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(unsigned long ulSrc) {
    if (V_VT(this) != VT_UI4) {
        Clear();
        V_VT(this) = VT_UI4;
    }
    V_UI4(this) = ulSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(int iSrc) {
    if (V_VT(this) != VT_INT) {
        Clear();
        V_VT(this) = VT_INT;
    }
    V_INT(this) = iSrc;
    return *this;
}

inline Ztl_variant_t& Ztl_variant_t::operator=(unsigned int uiSrc) {
    if (V_VT(this) != VT_UINT) {
        Clear();
        V_VT(this) = VT_UINT;
    }
    V_UINT(this) = uiSrc;
    return *this;
}

inline bool Ztl_variant_t::operator==(const VARIANT& varSrc) const throw() {
    return *this == &varSrc;
}

inline bool Ztl_variant_t::operator==(const VARIANT* pSrc) const throw() {
    if (!pSrc) {
        return false;
    }
    if (this == pSrc) {
        return true;
    }
    if (V_VT(this) != V_VT(pSrc)) {
        return false;
    }
    switch (V_VT(this)) {
    case VT_EMPTY:
    case VT_NULL:
        return true;
    case VT_I2:
        return V_I2(this) == V_I2(pSrc);
    case VT_I4:
        return V_I4(this) == V_I4(pSrc);
    case VT_R4:
        return V_R4(this) == V_R4(pSrc);
    case VT_R8:
        return V_R8(this) == V_R8(pSrc);
    case VT_CY:
        return memcmp(&(V_CY(this)), &(V_CY(pSrc)), sizeof(CY)) == 0;
    case VT_DATE:
        return V_DATE(this) == V_DATE(pSrc);
    case VT_BSTR:
        return (ZComAPI::ZComSysStringByteLen(V_BSTR(this)) == ZComAPI::ZComSysStringByteLen(V_BSTR(pSrc))) &&
               (memcmp(V_BSTR(this), V_BSTR(pSrc), ZComAPI::ZComSysStringByteLen(V_BSTR(this))) == 0);
    case VT_DISPATCH:
        return V_DISPATCH(this) == V_DISPATCH(pSrc);
    case VT_ERROR:
        return V_ERROR(this) == V_ERROR(pSrc);
    case VT_BOOL:
        return V_BOOL(this) == V_BOOL(pSrc);
    case VT_UNKNOWN:
        return V_UNKNOWN(this) == V_UNKNOWN(pSrc);
    case VT_DECIMAL:
        return memcmp(&(V_DECIMAL(this)), &(V_DECIMAL(pSrc)), sizeof(DECIMAL)) == 0;
    case VT_UI1:
        return V_UI1(this) == V_UI1(pSrc);
    case VT_I1:
        return V_I1(this) == V_I1(pSrc);
    case VT_UI2:
        return V_UI2(this) == V_UI2(pSrc);
    case VT_UI4:
        return V_UI4(this) == V_UI4(pSrc);
    case VT_INT:
        return V_INT(this) == V_INT(pSrc);
    case VT_UINT:
        return V_UINT(this) == V_UINT(pSrc);
    case VT_I8:
        return V_I8(this) == V_I8(pSrc);
    case VT_UI8:
        return V_UI8(this) == V_UI8(pSrc);
    default:
        _com_issue_error(E_INVALIDARG);
    }
    return false;
}

inline bool Ztl_variant_t::operator!=(const VARIANT& varSrc) const throw() {
    return !(*this == &varSrc);
}

inline bool Ztl_variant_t::operator!=(const VARIANT* pSrc) const throw() {
    return !(*this == pSrc);
}

inline void Ztl_variant_t::Clear() {
    _com_util::CheckError(ZComAPI::ZComVariantClear(this));
}

inline void Ztl_variant_t::Attach(VARIANT& varSrc) {
    Clear();
    _COM_MEMCPY_S(this, sizeof(varSrc), &varSrc, sizeof(varSrc));
    V_VT(&varSrc) = VT_EMPTY;
}

inline VARIANT Ztl_variant_t::Detach() {
    VARIANT varResult = *this;
    V_VT(this) = VT_EMPTY;
    return varResult;
}

inline VARIANT& Ztl_variant_t::GetVARIANT() throw() {
    return *(VARIANT*)this;
}

inline VARIANT* Ztl_variant_t::GetAddress() {
    Clear();
    return (VARIANT*)this;
}

inline void Ztl_variant_t::ChangeType(VARTYPE vartype, const Ztl_variant_t* pSrc) {
    if (!pSrc)
        pSrc = this;
    if ((this != pSrc) || (vartype != V_VT(this))) {
        _com_util::CheckError(ZComAPI::ZComVariantChangeType(static_cast<VARIANT*>(this), const_cast<VARIANT*>(static_cast<const VARIANT*>(pSrc)), 0, vartype));
    }
}

inline void Ztl_variant_t::SetString(const char* pSrc) {
    operator=(pSrc);
}

inline Ztl_variant_t::~Ztl_variant_t() throw() {
    ZComAPI::ZComVariantClear(this);
}

inline Ztl_bstr_t::Ztl_bstr_t(const Ztl_variant_t& var) : m_Data(NULL) {
    if (V_VT(&var) == VT_BSTR) {
        *this = V_BSTR(&var);
        return;
    }
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_BSTR, &var);
    *this = V_BSTR(&varDest);
}

inline Ztl_bstr_t& Ztl_bstr_t::operator=(const Ztl_variant_t& var) {
    if (V_VT(&var) == VT_BSTR) {
        *this = V_BSTR(&var);
        return *this;
    }
    Ztl_variant_t varDest;
    varDest.ChangeType(VT_BSTR, &var);
    *this = V_BSTR(&varDest);
    return *this;
}

inline Ztl_variant_t vtEmpty;