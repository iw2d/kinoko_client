#pragma once
#include "zlock.h"
#include "zalloc.h"
#include "zcom.h"
#include "tsingleton.h"


class ZAllocHelper {
public:
    ZAllocHelper(int nDummy) {}
};


template<typename T>
class ZRef {
public:
    unsigned char gap0[4];
    T* p = nullptr;
    // manage refcount manually
};
static_assert(sizeof(ZRef<int>) == 0x8);


template<typename T>
class ZXString {
private:
    struct _ZXStringData {
        volatile long nRef = 0;
        int nCap = 0;
        int nByteLen = 0;
    };
    static_assert(sizeof(_ZXStringData) == 0xC);

public:
    T* _m_pStr = nullptr;
    // manage refcount manually
    size_t GetLength() {
        if (this->_m_pStr) {
            auto pData = reinterpret_cast<ZXString<T>::_ZXStringData*>(this->_m_pStr) - 1;
            return pData->nByteLen / sizeof(T);
        } else {
            return 0;
        }
    }
};
static_assert(sizeof(ZXString<char>) == 0x4);


template<typename T>
class ZArray {
public:
    T* a = nullptr;
};
static_assert(sizeof(ZArray<int>) == 0x4);


template<typename T>
class ZList {
public:
    unsigned char gap0[8];
    size_t _m_uCount = 0;
    T* _m_pHead = nullptr;
    T* _m_pTail = nullptr;
};
static_assert(sizeof(ZList<int>) == 0x14);

template<typename T, typename V, typename U>
class ZMap {
public:
    unsigned char gap0[4];
    void** _m_apTable;
    size_t _m_uTableSize;
    size_t _m_uCount;
    size_t _m_uAutoGrowEvery128;
    size_t _m_uAutoGrowLimit;
};
static_assert(sizeof(ZMap<long, long, long>) == 0x18);


class ZException {
public:
    HRESULT m_hr;

    ZException(HRESULT hr) {
        this->m_hr = hr;
    }
};
static_assert(sizeof(ZException) == 0x4);


class ZInetAddr;
class ZSocketBuffer;

class ZSocketBase {
public:
    unsigned int _m_hSocket = INVALID_SOCKET;
};
static_assert(sizeof(ZSocketBase) == 0x4);