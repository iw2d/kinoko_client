#pragma once
#include <new>


template <typename T>
T* construct(T* p) {
    return new (p) T();
}


class ZException {
public:
    HRESULT m_hr;

    ZException(HRESULT hr) {
        m_hr = hr;
    }
};
static_assert(sizeof(ZException) == 0x4);