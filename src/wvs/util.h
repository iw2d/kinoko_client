#pragma once
#include "wzlib/gr2d.h"
#include <comutil.h>
#include <cstdint>

#define CHECK_HR(_hr) \
    _com_util::CheckError(_hr)

#define MEMBER_AT(T, OFFSET, NAME) \
    __forceinline auto& NAME() { \
        return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }

#define MEMBER_ARRAY_AT(T, N, OFFSET, NAME) \
    __forceinline T* NAME() { \
        return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }


inline IWzGr2DPtr& get_gr() {
    return *reinterpret_cast<IWzGr2DPtr*>(0x00C6F430);
}
