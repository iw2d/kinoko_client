#pragma once
#include "ztl/ztl.h"
#include <cstdlib>
#include <cstdint>


template <typename T>
struct TSecData {
    T data;
    uint8_t bKey;
    uint8_t FakePtr1;
    uint8_t FakePtr2;
    uint16_t wChecksum;
};

static_assert(sizeof(TSecData<uint8_t>) == 0x6);
static_assert(sizeof(TSecData<uint32_t>) == 0xC);


template <typename T>
class TSecType {
protected:
    uint32_t FakePtr1;
    uint32_t FakePtr2;
    TSecData<T>* m_secdata;

public:
    explicit TSecType(const T op) : m_secdata(new TSecData<T>()) {
        FakePtr1 = rand();
        FakePtr2 = rand();
        m_secdata->FakePtr1 = FakePtr1;
        m_secdata->FakePtr2 = FakePtr2;
        SetData(op);
    }
    TSecType() : TSecType(0) {
    }
    T GetData() const {
        T tmp;
        uint8_t bKey = m_secdata->bKey;
        uint16_t wChecksum = 0x9A65;
        for (size_t i = 0; i < sizeof(T); ++i) {
            if (!bKey) {
                bKey = 42;
            }
            uint8_t bEnc = reinterpret_cast<uint8_t*>(&m_secdata->data)[i];
            reinterpret_cast<uint8_t*>(&tmp)[i] = bEnc ^ bKey;
            bKey = bEnc + 42 + bKey;
            wChecksum = (wChecksum << 3) | (bKey + (wChecksum >> 13));
        }
        if (wChecksum != m_secdata->wChecksum || (FakePtr1 & 0xFF) != m_secdata->FakePtr1 || (FakePtr1 & 0xFF) != m_secdata->FakePtr1) {
            throw ZException(5);
        }
        return tmp;
    }
    void SetData(const T data) {
        m_secdata->bKey = rand();
        m_secdata->wChecksum = 0x9A65;
        uint8_t bKey = m_secdata->bKey;
        for (size_t i = 0; i < sizeof(T); ++i) {
            if (!bKey) {
                bKey = 42;
            }
            uint8_t bVal = reinterpret_cast<const uint8_t*>(&data)[i];
            reinterpret_cast<uint8_t*>(&m_secdata->data)[i] = bKey ^ bVal;
            bKey = (bKey ^ bVal) + 42 + bKey;
            m_secdata->wChecksum = (m_secdata->wChecksum << 3) | (bKey + (m_secdata->wChecksum >> 13));
        }
    }
};

static_assert(sizeof(TSecType<uint8_t>) == 0xC);
static_assert(sizeof(TSecType<uint32_t>) == 0xC);


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
#ifdef _DEBUG
    assert(checksum == cs);
#endif
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