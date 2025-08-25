#pragma once
#include "debug.h"
#include <cstdint>

#ifdef _DEBUG
#define ATTACH_HOOK(TARGET, DETOUR) \
    AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", #DETOUR, TARGET), false)
#else
#define ATTACH_HOOK(TARGET, DETOUR) \
    AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR))
#endif

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

#define MEMBER_ARRAY_AT(T, OFFSET, NAME, N) \
    __declspec(property(get = get_##NAME)) T(&NAME)[N]; \
    __forceinline T(&get_##NAME())[N] { \
        return *reinterpret_cast<T(*)[N]>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }


// called in injector.cpp -> DllMain
void AttachSystemHooks();

// called in system.cpp -> CreateMutexA_hook
void AttachClientBypass();
void AttachClientHelper();
void AttachClientInlink();
void AttachStringPoolMod();
void AttachSystemOptionMod();
void AttachTemporaryStatMod();
void AttachElementalDamageMod();
void AttachExceptionHandler();

inline void AttachClientHooks() {
    AttachClientBypass();
    AttachClientHelper();
    AttachClientInlink();
    AttachStringPoolMod();
    AttachSystemOptionMod();
    AttachTemporaryStatMod();
    AttachElementalDamageMod();
    AttachExceptionHandler();
}


template <typename T>
__forceinline auto CastHook(T fn) -> void* {
    union {
        T fn;
        void* p;
    } u;
    u.fn = fn;
    return u.p;
}

bool AttachHook(void** ppTarget, void* pDetour);

void* VMTHook(void* pInstance, void* pDetour, size_t uIndex);

void* GetAddress(const char* sModuleName, const char* sProcName);

void* GetAddressByPattern(const char* sModuleName, const char* sPattern);

void Patch1(uintptr_t pAddress, uint8_t uValue);

void Patch4(uintptr_t pAddress, uint32_t uValue);

void PatchStr(uintptr_t uAddress, const char* sValue);

void PatchJmp(uintptr_t pAddress, uintptr_t pDestination);

void PatchNop(uintptr_t pAddress, uintptr_t pDestination);

void PatchCall(uintptr_t pAddress, uintptr_t pDestination);

void PatchRetZero(uintptr_t pAddress);