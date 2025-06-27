#pragma once
#include "ztl/zcom.h"
#include "wzlib/gr2d.h"
#include "wzlib/resman.h"
#include "wzlib/namespace.h"
#include "wzlib/property.h"
#include "wzlib/canvas.h"
#include <comutil.h>
#include <cstdint>
#include <memory>

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

inline IWzResManPtr& get_rm() {
    return *reinterpret_cast<IWzResManPtr*>(0x00C6F434);
}

static inline IWzNameSpacePtr& get_root() {
    return *reinterpret_cast<IWzNameSpacePtr*>(0x00C6F43C);
}

static inline IWzNameSpacePtr& get_sub(int32_t nIdx) {
    return reinterpret_cast<IWzNameSpacePtr*>(0x00C6F440)[nIdx];
}

inline IUnknownPtr get_unknown(Ztl_variant_t& v){
    IUnknownPtr result;
    reinterpret_cast<IUnknownPtr*(__cdecl*)(IUnknownPtr*, Ztl_variant_t*)>(0x004176E0)(std::addressof(result), &v); // RVO in client
    return result;
}

inline uint32_t get_int32(Ztl_variant_t& v, uint32_t nDefault) {
    Ztl_variant_t vInt;
    if (V_VT(&v) == VT_EMPTY || V_VT(&v) == VT_ERROR || FAILED(ZComAPI::ZComVariantChangeType(&vInt, &v, 0, VT_I4))) {
        return nDefault;
    } else {
        return V_I4(&vInt);
    }
}

inline int32_t draw_number_by_image(IWzCanvasPtr pCanvas, int32_t nLeft, int32_t nTop, int32_t nNo, IWzPropertyPtr pBase, int32_t nHorzSpace) {
    return reinterpret_cast<int32_t (__cdecl*)(IWzCanvasPtr, int32_t, int32_t, int32_t, IWzPropertyPtr, int32_t)>(0x00965780)(pCanvas, nLeft, nTop, nNo, pBase, nHorzSpace);
}