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


template<typename T>
unsigned int __fastcall ZtlSecureTear(T* at, T t) {
    typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type V;
    constexpr unsigned int iteration = sizeof(T) / sizeof(V);
    constexpr unsigned int rotation = sizeof(T) < 4 ? 0 : 5;
    unsigned int checksum = 0xBAADF00D;
    V* v1 = reinterpret_cast<V*>(&at[0]);
    V* v2 = reinterpret_cast<V*>(&at[1]);
    V* value = reinterpret_cast<V*>(&t);
    for (size_t i = 0; i < iteration; ++i) {
        v1[i] = rand();
        v2[i] = _rotr(value[i] ^ v1[i], rotation);
        checksum = v2[i] + _rotr(v1[i] ^ checksum, 5);
    }
    return checksum;
}

template<typename T>
T __fastcall ZtlSecureFuse(T* at, unsigned int cs) {
    typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type V;
    constexpr unsigned int iteration = sizeof(T) / sizeof(V);
    constexpr unsigned int rotation = sizeof(T) < 4 ? 0 : 5;
    unsigned int checksum = 0xBAADF00D;
    V* v1 = reinterpret_cast<V*>(&at[0]);
    V* v2 = reinterpret_cast<V*>(&at[1]);
    V value[iteration] = { 0 };
    for (size_t i = 0; i < iteration; ++i) {
        value[i] = v1[i] ^ _rotl(v2[i], rotation);
        checksum = v2[i] + _rotr(v1[i] ^ checksum, 5);
    }
    assert(checksum == cs);
    return *reinterpret_cast<T*>(&value[0]);
}

template<typename T>
struct ZtlSecure {
    T at[2];
    unsigned int cs;

    inline void Tear(T t) {
        cs = ZtlSecureTear<T>(at, t);
    }
    inline T Fuse() {
        return ZtlSecureFuse<T>(at, cs);
    }
};