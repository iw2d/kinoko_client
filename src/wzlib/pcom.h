#pragma once
#include <windows.h>
#include <comdef.h>
#include <cstdint>


inline FARPROC* _g_ppfnPCOMAPIs = reinterpret_cast<FARPROC*>(0x00C6DB54); // changed from FARPROC _g_apfnPCOMAPIs[5];

template<typename T>
void __cdecl PcCreateObject(const wchar_t* sUOL, T& pObj, IUnknown* pUnkOuter) {
    pObj = nullptr;
    if (!_g_ppfnPCOMAPIs[0]) {
        _com_issue_error(CO_E_NOTINITIALIZED);
    }
    _com_util::CheckError(reinterpret_cast<HRESULT (__cdecl*)(const wchar_t*, const GUID*, T&, void*)>(_g_ppfnPCOMAPIs[0])(sUOL, &__uuidof(T), pObj, pUnkOuter));
}

inline void PcSetRootNameSpace(IUnknown* pNameSpace) {
    _com_util::CheckError(reinterpret_cast<HRESULT (__cdecl*)(IUnknown**, int32_t)>(_g_ppfnPCOMAPIs[4])(&pNameSpace, 1));
}