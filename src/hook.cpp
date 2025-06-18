#include "pch.h"
#include "hook.h"
#include "debug.h"
#include <detours.h>


bool AttachHook(void** ppTarget, void* pDetour) {
    LONG result;
    if (result = DetourTransactionBegin(); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourTransactionBegin failed with : %d", result);
        return false;
    }
    if (result = DetourUpdateThread(GetCurrentThread()); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourUpdateThread failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    if (result = DetourAttach(ppTarget, pDetour); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourAttach failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    if (result = DetourTransactionCommit(); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourTransactionCommit failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    return true;
}

void* VMTHook(void* pInstance, void* pDetour, size_t uIndex) {
    void** vtable = *static_cast<void***>(pInstance);
    void* pTarget = vtable[uIndex];
    AttachHook(&pTarget, pDetour);
    return pTarget;
}

void* GetAddress(const char* sModuleName, const char* sProcName) {
    HMODULE hModule = GetModuleHandleA(sModuleName);
    if (!hModule) {
        hModule = LoadLibraryA(sModuleName);
    }
    FARPROC result = GetProcAddress(hModule, sProcName);
    if (!result) {
        DEBUG_MESSAGE("Could not resolve address for %s in module %s", sProcName, sModuleName);
    }
    return reinterpret_cast<void*>(result);
}


void Patch1(uintptr_t pAddress, uint8_t uValue) {
    DWORD flOldProtect;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), 1, PAGE_EXECUTE_READWRITE, &flOldProtect);
    *reinterpret_cast<uint8_t*>(pAddress) = uValue;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), 1, flOldProtect, &flOldProtect);
}

void Patch4(uintptr_t pAddress, uint32_t uValue) {
    DWORD flOldProtect;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), 4, PAGE_EXECUTE_READWRITE, &flOldProtect);
    *reinterpret_cast<uint32_t*>(pAddress) = uValue;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), 4, flOldProtect, &flOldProtect);
}

void PatchStr(uintptr_t pAddress, const char* sValue) {
    size_t uSize = strlen(sValue);
    DWORD flOldProtect;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), uSize, PAGE_EXECUTE_READWRITE, &flOldProtect);
    CopyMemory(reinterpret_cast<PVOID>(pAddress), sValue, uSize);
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), uSize, flOldProtect, &flOldProtect);
}

void PatchJmp(uintptr_t pAddress, uintptr_t pDestination) {
    Patch1(pAddress, 0xE9);
    Patch4(pAddress + 1, pDestination - pAddress - 5);
}

void PatchNop(uintptr_t pAddress, uintptr_t pDestination) {
    size_t uSize = pDestination - pAddress;
    DWORD flOldProtect;
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), uSize, PAGE_EXECUTE_READWRITE, &flOldProtect);
    FillMemory(reinterpret_cast<PVOID>(pAddress), uSize, 0x90);
    VirtualProtect(reinterpret_cast<LPVOID>(pAddress), uSize, flOldProtect, &flOldProtect);
}

void PatchCall(uintptr_t pAddress, uintptr_t pDestination) {
    Patch1(pAddress, 0xE8);
    Patch4(pAddress + 1, pDestination - pAddress - 5);
}

void PatchRetZero(uintptr_t pAddress) {
    PatchStr(pAddress, "\x33\xC0\xC3");
}