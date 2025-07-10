#pragma once
#pragma pack(push, 8)

#include "IWzSerialize.h"
#include "ztl/ztl.h"
#include <comdef.h>

struct __declspec(uuid("f945bf59-d1ec-45e8-8bd9-3dd11ac1a48a"))
/* interface */ IWzUOL;

_COM_SMARTPTR_TYPEDEF(IWzUOL, __uuidof(IWzUOL));

struct __declspec(uuid("f945bf59-d1ec-45e8-8bd9-3dd11ac1a48a"))
IWzUOL : IWzSerialize
{
    __declspec(property(get=GetUOL,put=PutUOL))
    Ztl_bstr_t UOL;
    __declspec(property(get=GetfilePath))
    Ztl_bstr_t filePath;
    __declspec(property(get=GetclassName))
    Ztl_bstr_t className;
    __declspec(property(get=GetlocalUOL))
    Ztl_bstr_t localUOL;
    __declspec(property(get=GetcurrentDirectory,put=PutcurrentDirectory))
    Ztl_bstr_t currentDirectory;

    HRESULT Init (
        Ztl_bstr_t sUOL,
        const Ztl_variant_t & vCurrentDirectory = vtEmpty );
    Ztl_bstr_t GetUOL ( );
    void PutUOL (
        Ztl_bstr_t pVal );
    Ztl_bstr_t GetfilePath ( );
    Ztl_bstr_t GetclassName ( );
    Ztl_bstr_t GetlocalUOL ( );
    Ztl_bstr_t GetcurrentDirectory ( );
    void PutcurrentDirectory (
        Ztl_bstr_t pVal );

    virtual HRESULT __stdcall raw_Init (
        /*[in]*/ BSTR sUOL,
        /*[in]*/ VARIANT vCurrentDirectory = vtEmpty ) = 0;
    virtual HRESULT __stdcall get_UOL (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_UOL (
        /*[in]*/ BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_filePath (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall get_className (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall get_localUOL (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall get_currentDirectory (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_currentDirectory (
        /*[in]*/ BSTR pVal ) = 0;
};


inline HRESULT IWzUOL::Init ( Ztl_bstr_t sUOL, const Ztl_variant_t & vCurrentDirectory ) {
    HRESULT _hr = raw_Init(sUOL, vCurrentDirectory);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline Ztl_bstr_t IWzUOL::GetUOL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_UOL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline void IWzUOL::PutUOL ( Ztl_bstr_t pVal ) {
    HRESULT _hr = put_UOL(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_bstr_t IWzUOL::GetfilePath ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_filePath(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline Ztl_bstr_t IWzUOL::GetclassName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_className(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline Ztl_bstr_t IWzUOL::GetlocalUOL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_localUOL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline Ztl_bstr_t IWzUOL::GetcurrentDirectory ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_currentDirectory(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline void IWzUOL::PutcurrentDirectory ( Ztl_bstr_t pVal ) {
    HRESULT _hr = put_currentDirectory(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}


#pragma pack(pop)
