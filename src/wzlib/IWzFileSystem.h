#pragma once
#pragma pack(push, 8)

#include "IWzWritableNameSpace.h"
#include "ztl/ztl.h"
#include <comdef.h>

struct __declspec(uuid("352d8655-51e4-4668-8ce4-0866e2b6a5b5"))
/* interface */ IWzFileSystem;

_COM_SMARTPTR_TYPEDEF(IWzFileSystem, __uuidof(IWzFileSystem));

struct __declspec(uuid("352d8655-51e4-4668-8ce4-0866e2b6a5b5"))
IWzFileSystem : IWzWritableNameSpace
{
    HRESULT Init (
        Ztl_bstr_t sPath );

    virtual HRESULT __stdcall raw_Init (
        /*[in]*/ BSTR sPath ) = 0;
};


inline HRESULT IWzFileSystem::Init ( Ztl_bstr_t sPath ) {
    HRESULT _hr = raw_Init(sPath);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
