#pragma once
#include <comdef.h>


class Ztl_bstr_t {
public:
    class Data_t {
    public:
        wchar_t* m_wstr;
        char* m_str;
        volatile long m_RefCount;
    };
    static_assert(sizeof(Data_t) == 0xC);

    Data_t* m_Data;

    inline Ztl_bstr_t() {
        this->m_Data = nullptr;
    }
    inline Ztl_bstr_t(const char* s) {
        reinterpret_cast<void (__thiscall*)(Ztl_bstr_t*, const char*)>(0x00404890)(this, s);
    }
    inline Ztl_bstr_t(const wchar_t* s) {
        reinterpret_cast<void (__thiscall*)(Ztl_bstr_t*, const wchar_t*)>(0x004048B0)(this, s);
    }
    inline Ztl_bstr_t(const Ztl_bstr_t& that) {
        reinterpret_cast<void (__thiscall*)(Ztl_bstr_t*, const Ztl_bstr_t&)>(0x00404B90)(this, that);
    }
    inline ~Ztl_bstr_t() {
        reinterpret_cast<void (__thiscall*)(Ztl_bstr_t*)>(0x004048D0)(this);
    }
};
static_assert(sizeof(Ztl_bstr_t) == 0x4);


class Ztl_variant_t : public tagVARIANT {
public:
    inline Ztl_variant_t() {
        VariantInit(this);
    }
    inline Ztl_variant_t(int lSrc, VARTYPE vtSrc) {
        reinterpret_cast<void (__thiscall*)(Ztl_variant_t*, int, VARTYPE)>(0x00401A40)(this, lSrc, vtSrc);
    }
    inline ~Ztl_variant_t() {
        reinterpret_cast<void (__thiscall*)(Ztl_variant_t*)>(0x00401B90)(this);
    }
    inline IUnknown* GetUnknown(bool fAddRef, bool fTryChangeType) {
        return reinterpret_cast<IUnknown* (__thiscall*)(Ztl_variant_t*, bool, bool)>(0x00401A60)(this, fAddRef, fTryChangeType);
    }
};
static_assert(sizeof(Ztl_variant_t) == 0x10);