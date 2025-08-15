#pragma once
#pragma pack(push, 8)

#include "IWzNameSpace.h"
#include "ztl/zcom.h"

struct __declspec(uuid("ee7659a2-23cd-4e44-86e5-d82a680e83f7"))
/* interface */ IWzWritableNameSpace;

_COM_SMARTPTR_TYPEDEF(IWzWritableNameSpace, __uuidof(IWzWritableNameSpace));

struct __declspec(uuid("ee7659a2-23cd-4e44-86e5-d82a680e83f7"))
IWzWritableNameSpace : IWzNameSpace
{
    IWzNameSpacePtr CreateChildNameSpace (
        Ztl_bstr_t sName );
    HRESULT AddObject (
        Ztl_bstr_t sName,
        const Ztl_variant_t & vObj,
        VARIANT * pvObj );
    HRESULT Unlink (
        Ztl_bstr_t sName );

    virtual HRESULT __stdcall raw_CreateChildNameSpace (
        /*[in]*/ BSTR sName,
        /*[out,retval]*/ struct IWzNameSpace * * ppNameSpace ) = 0;
    virtual HRESULT __stdcall raw_AddObject (
        /*[in]*/ BSTR sName,
        /*[in]*/ VARIANT vObj,
        /*[out]*/ VARIANT * pvObj ) = 0;
    virtual HRESULT __stdcall raw_Unlink (
        /*[in]*/ BSTR sName ) = 0;
};


inline IWzNameSpacePtr IWzWritableNameSpace::CreateChildNameSpace ( Ztl_bstr_t sName ) {
    struct IWzNameSpace * _result = 0;
    HRESULT _hr = raw_CreateChildNameSpace(sName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzNameSpacePtr(_result, false);
}

inline HRESULT IWzWritableNameSpace::AddObject ( Ztl_bstr_t sName, const Ztl_variant_t & vObj, VARIANT * pvObj ) {
    HRESULT _hr = raw_AddObject(sName, vObj, pvObj);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzWritableNameSpace::Unlink ( Ztl_bstr_t sName ) {
    HRESULT _hr = raw_Unlink(sName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
