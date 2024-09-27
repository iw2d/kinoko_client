#pragma once
#include "debug.h"

#ifdef _DEBUG
    #define ATTACH_HOOK(TARGET, DETOUR) \
        AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", #DETOUR, TARGET), false)
#else
    #define ATTACH_HOOK(TARGET, DETOUR) \
        AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR))
#endif


// called in injector.cpp -> DllMain
void AttachSystemHooks();

// called in system.cpp -> CreateMutexA_hook
void AttachClientBypass();
void AttachClientInlink();
void AttachClientHelper();
void AttachStringPoolMod();
void AttachResolutionMod();
void AttachChatBalloonMod();
void AttachPortableChairMod();
void AttachTemporaryStatMod();
void AttachExceptionHandler();

static void AttachClientHooks() {
    AttachClientBypass();
    AttachClientInlink();
    AttachClientHelper();
    AttachStringPoolMod();
    AttachResolutionMod();
    AttachChatBalloonMod();
    AttachPortableChairMod();
    AttachTemporaryStatMod();
    AttachExceptionHandler();
}


template<typename T>
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

void* GetAddress(const char* sModuelName, const char* sProcName);

void Patch1(uintptr_t pAddress, unsigned char uValue);

void Patch4(uintptr_t pAddress, unsigned int uValue);

void PatchStr(uintptr_t uAddress, const char* sValue);

void PatchJmp(uintptr_t pAddress, uintptr_t pDestination);

void PatchNop(uintptr_t pAddress, uintptr_t pDestination);

void PatchRetZero(uintptr_t pAddress);