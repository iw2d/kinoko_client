#pragma once
#include "zalloc.h"
#include <windows.h>
#include <cstdint>
#include <string>


template <typename T>
class ZXString {
    struct _ZXStringData {
        volatile long nRef;
        int32_t nCap;
        int32_t nByteLen;

        T* pStr() {
            return reinterpret_cast<T*>(&this[1]);
        }
    };
    static_assert(sizeof(_ZXStringData) == 0xC);

protected:
    T* _m_pStr;

public:
    ~ZXString() {
        if (_m_pStr) {
            _Release(_GetData());
        }
    }
    ZXString(const char* s, int32_t n = -1) : _m_pStr(nullptr) {
        Assign(s, n);
    }
    ZXString(const ZXString<T>& s) : _m_pStr(nullptr) {
        *this = s;
    }
    ZXString() : _m_pStr(nullptr) {
    }
    ZXString<T>& operator=(const ZXString<T>& s) {
        if (this == &s) {
            return *this;
        }
        if (s._m_pStr && s._GetData()) {
            auto pData = s._GetData();
            if (pData->nRef == -1) {
                *this = s._m_pStr;
                return *this;
            }
            InterlockedIncrement(&pData->nRef);
            if (_m_pStr) {
                _Release(_GetData());
            }
            _m_pStr = s._m_pStr;
        } else if (_m_pStr) {
            _Release(_GetData());
            _m_pStr = nullptr;
        }
        return *this;
    }
    ZXString<T>& operator=(const T* s) {
        Assign(s, std::char_traits<T>::length(s));
        return *this;
    }

    void Assign(const T* s, int32_t n) {
        if (s) {
            int32_t nLength = n == -1 ? std::char_traits<T>::length(s) : n;
            T* pBuffer = GetBuffer(nLength, 0);
            memcpy(pBuffer, s, nLength * sizeof(T));
            ReleaseBuffer(nLength);
        } else if (_m_pStr) {
            _Release(_GetData());
            _m_pStr = nullptr;
        }
    }
    int32_t GetLength() const {
        if (_m_pStr) {
            return _GetData()->nByteLen / sizeof(T);
        } else {
            return 0;
        }
    }
    int32_t IsEmpty() const {
        return !_m_pStr || !_m_pStr[0];
    }
    T* GetBuffer(int32_t nMinLength, int32_t bRetain) {
        int32_t nCap = 0;
        auto pData = _m_pStr ? _GetData() : nullptr;
        if (pData) {
            if (pData->nRef <= 1 && pData->nCap >= nMinLength) {
                pData->nRef = -1;
                return _m_pStr;
            }
            nCap = (pData->nByteLen) / sizeof(T);
        }
        if (nCap < nMinLength) {
            nCap = nMinLength;
        }
        auto pAlloc = _Alloc(nCap);
        pAlloc->nRef = -1;
        _m_pStr = pAlloc->pStr();
        if (bRetain && pData) {
            memcpy(_m_pStr, pData->pStr(), pData->nByteLen + sizeof(T));
            pAlloc->nByteLen = pData->nByteLen;
        } else {
            pAlloc->nByteLen = 0;
            _m_pStr[0] = 0;
        }
        if (pData) {
            _Release(pData);
        }
        return _m_pStr;
    }
    void ReleaseBuffer(int32_t nLength) {
        auto pData = _GetData();
        pData->nRef = 1;
        if (nLength == -1) {
            pData->nByteLen = std::char_traits<T>::length(pData->pStr()) * sizeof(T);
        } else {
            _m_pStr[nLength] = 0;
            pData->nByteLen = nLength;
        }
    }

    operator const T*() const {
        return _m_pStr;
    }
    const char& operator[](size_t i) const {
        return _m_pStr[i];
    }
    ZXString<T>& operator+=(const ZXString<char>& s) {
        if (s._m_pStr) {
            return _Cat(s._m_pStr, s.GetLength());
        } else {
            return _Cat(nullptr, 0);
        }
    }
    ZXString<T>& operator+=(const T* s) {
        return _Cat(s, std::char_traits<T>::length(s));
    }

protected:
    _ZXStringData* _GetData() const {
        return reinterpret_cast<_ZXStringData*>(_m_pStr) - 1;
    }
    _ZXStringData* _GetData() {
        return reinterpret_cast<_ZXStringData*>(_m_pStr) - 1;
    }
    ZXString<T>& _Cat(const T* s, int32_t n) {
        if (n) {
            if (IsEmpty()) {
                T* pBuffer = GetBuffer(n, 0);
                memcpy(pBuffer, s, n * sizeof(T));
                ReleaseBuffer(n);
            } else {
                int32_t nReq = GetLength() + n;
                int32_t nCap = _GetData()->nCap;
                while (nCap < nReq) {
                    nCap *= 2;
                }
                T* pBuffer = GetBuffer(n, 1);
                memcpy(&pBuffer[GetLength()], s, n * sizeof(T));
                ReleaseBuffer(nReq);
            }
        }
        return *this;
    }

    static void _Release(_ZXStringData* pData) {
        if (InterlockedDecrement(&pData->nRef) <= 0) {
            _Free(pData);
        }
    }
    static _ZXStringData* _Alloc(int32_t nCap) {
        auto pData = static_cast<_ZXStringData*>(ZAllocEx<ZAllocStrSelector<T>>::s_Alloc(sizeof(T) * (nCap + 1) + sizeof(_ZXStringData)));
        pData->nCap = nCap;
        return pData;
    }
    static void _Free(_ZXStringData* pFree) {
        ZAllocEx<ZAllocStrSelector<T>>::s_Free(pFree);
    }
};

static_assert(sizeof(ZXString<char>) == 0x4);