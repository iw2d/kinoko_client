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
