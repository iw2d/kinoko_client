#include "pch.h"
#include "hook.h"
#include "debug.h"
#include <detours.h>
#include <psapi.h>


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


bool HexCharToByte(char c, uint8_t* b) {
    if ('0' <= c && c <= '9')
        *b = c - '0';
    else if ('A' <= c && c <= 'F')
        *b = 10 + (c - 'A');
    else if ('a' <= c && c <= 'f')
        *b = 10 + (c - 'a');
    else
        return false;
    return true;
}

size_t ParsePattern(const char* sPattern, uint8_t* abPattern, uint8_t* abMask) {
    size_t i = 0;
    while (*sPattern) {
        if (*sPattern == ' ') {
            sPattern++;
            continue;
        }
        if (sPattern[0] == '?' && sPattern[1] == '?') {
            abMask[i] = 0x00;
        } else {
            uint8_t high, low;
            if (!HexCharToByte(sPattern[0], &high) || !HexCharToByte(sPattern[1], &low)) {
                return 0;
            }
            abPattern[i] = (high << 4) | low;
            abMask[i] = 0xFF;
        }
        sPattern += 2;
        i += 1;
    }
    return i;
}

void* GetAddressByPattern(const char* sModuleName, const char* sPattern) {
    HMODULE hModule = GetModuleHandleA(sModuleName);
    if (!hModule) {
        hModule = LoadLibraryA(sModuleName);
    }
    MODULEINFO mi;
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &mi, sizeof(mi))) {
        DEBUG_MESSAGE("Could not get module information for : %s", sModuleName);
        return nullptr;
    }
    uint8_t* pModuleBase = static_cast<uint8_t*>(mi.lpBaseOfDll);
    size_t uModuleSize = mi.SizeOfImage;

    uint8_t abPattern[1024];
    uint8_t abMask[1024];
    size_t uPatternSize = ParsePattern(sPattern, abPattern, abMask);
    if (uPatternSize == 0) {
        DEBUG_MESSAGE("Could not parse pattern : %s", sPattern);
        return nullptr;
    }

    for (size_t i = 0; i <= uModuleSize - uPatternSize; ++i) {
        size_t j;
        for (j = 0; j < uPatternSize; ++j) {
            if ((pModuleBase[i + j] & abMask[j]) != (abPattern[j] & abMask[j])) {
                break;
            }
        }
        if (j == uPatternSize) {
            return &pModuleBase[i];
        }
    }
    DEBUG_MESSAGE("Could not resolve address for pattern \"%s\" in module %s", sPattern, sModuleName);
    return nullptr;
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