#pragma once
#include "tsingleton.h"
#include "zlock.h"
#include "zalloc.h"
#include "zcoll.h"
#include "zstr.h"
#include "zcom.h"
#include "zsock.h"
#include <new>


template <typename T>
T* construct(T* p) {
    return new (p) T();
}

template <typename T>
void destruct(T* p) {
    if (p) {
        p->~T();
    }
}


class ZException {
public:
    HRESULT m_hr;

    ZException(HRESULT hr) {
        m_hr = hr;
    }
};
static_assert(sizeof(ZException) == 0x4);