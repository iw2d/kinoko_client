#pragma once
#include "zlock.h"
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
};
static_assert(sizeof(ZRef<int>) == 0x8);


template<typename T>
class ZXString {
public:
    T* _m_pStr = nullptr;
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