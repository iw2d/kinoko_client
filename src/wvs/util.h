#pragma once
#include "wzlib/gr2d.h"
#include <comutil.h>
#include <cstdint>

#define CHECK_HR(_hr) \
    _com_util::CheckError(_hr)

#define MEMBER_AT(T, OFFSET, NAME) \
    __declspec(property(get = get_##NAME, put = set_##NAME)) T NAME; \
    __forceinline const T& get_##NAME() const { \
        return *reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    } \
    __forceinline T& get_##NAME() { \
        return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    } \
    __forceinline void set_##NAME(const T& value) { \
        *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET) = const_cast<T&>(value); \
    } \
    __forceinline void set_##NAME(T& value) { \
        *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET) = value; \
    }

#define MEMBER_ARRAY_AT(T, N, OFFSET, NAME) \
    __declspec(property(get = get_##NAME)) T* NAME; \
    __forceinline T* get_##NAME() { \
        return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }


inline IWzGr2DPtr& get_gr() {
    return *reinterpret_cast<IWzGr2DPtr*>(0x00C6F430);
}
