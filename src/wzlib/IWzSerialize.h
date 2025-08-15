#pragma once
#pragma pack(push, 8)

#include "IWzArchive.h"
#include "ztl/zcom.h"

struct __declspec(uuid("01110bc6-d843-4ff1-918c-9cf64632fb15"))
/* interface */ IWzSerialize;

_COM_SMARTPTR_TYPEDEF(IWzSerialize, __uuidof(IWzSerialize));

struct __declspec(uuid("01110bc6-d843-4ff1-918c-9cf64632fb15"))
IWzSerialize : IUnknown
{
    __declspec(property(get=GetpersistentUOL))
    Ztl_bstr_t persistentUOL;

    Ztl_bstr_t GetpersistentUOL ( );
    HRESULT Serialize (
        struct IWzArchive * pArchive );

    virtual HRESULT __stdcall get_persistentUOL (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall raw_Serialize (
        /*[in]*/ struct IWzArchive * pArchive ) = 0;
};


inline Ztl_bstr_t IWzSerialize::GetpersistentUOL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_persistentUOL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline HRESULT IWzSerialize::Serialize ( struct IWzArchive * pArchive ) {
    HRESULT _hr = raw_Serialize(pArchive);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
