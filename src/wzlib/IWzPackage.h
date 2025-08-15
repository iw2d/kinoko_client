#pragma once
#pragma pack(push, 8)

#include "IWzSeekableArchive.h"
#include "IWzNameSpace.h"
#include "ztl/zcom.h"

struct __declspec(uuid("e610818b-038d-4522-9232-30fcd5f4737c"))
/* interface */ IWzPackage;

_COM_SMARTPTR_TYPEDEF(IWzPackage, __uuidof(IWzPackage));

struct __declspec(uuid("e610818b-038d-4522-9232-30fcd5f4737c"))
IWzPackage : IWzNameSpace
{
    HRESULT Init (
        Ztl_bstr_t sKey,
        Ztl_bstr_t sBaseUOL,
        struct IWzSeekableArchive * pArchive );

    virtual HRESULT __stdcall raw_Init (
        /*[in]*/ BSTR sKey,
        /*[in]*/ BSTR sBaseUOL,
        /*[in]*/ struct IWzSeekableArchive * pArchive ) = 0;
};


inline HRESULT IWzPackage::Init ( Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, struct IWzSeekableArchive * pArchive ) {
    HRESULT _hr = raw_Init(sKey, sBaseUOL, pArchive);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
