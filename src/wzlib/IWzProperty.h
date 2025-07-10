#pragma once
#pragma pack(push, 8)

#include "IWzSerialize.h"
#include "ztl/ztl.h"
#include <comdef.h>

struct __declspec(uuid("986515d9-0a0b-4929-8b4f-718682177b92"))
/* interface */ IWzProperty;

_COM_SMARTPTR_TYPEDEF(IWzProperty, __uuidof(IWzProperty));

struct __declspec(uuid("986515d9-0a0b-4929-8b4f-718682177b92"))
IWzProperty : IWzSerialize
{
    __declspec(property(get=Getitem,put=Putitem))
    Ztl_variant_t item[];
    __declspec(property(get=Get_NewEnum))
    IUnknownPtr _NewEnum;
    __declspec(property(get=Getcount))
    unsigned int count;

    Ztl_variant_t Getitem (
        Ztl_bstr_t sPath );
    void Putitem (
        Ztl_bstr_t sPath,
        const Ztl_variant_t & pvValue );
    IUnknownPtr Get_NewEnum ( );
    unsigned int Getcount ( );
    HRESULT Add (
        Ztl_bstr_t sName,
        const Ztl_variant_t & vValue,
        const Ztl_variant_t & bNoReplace = vtEmpty );
    HRESULT Remove (
        Ztl_bstr_t sName );
    HRESULT Import (
        Ztl_bstr_t sText );

    virtual HRESULT __stdcall get_item (
        /*[in]*/ BSTR sPath,
        /*[out,retval]*/ VARIANT * pvValue ) = 0;
    virtual HRESULT __stdcall put_item (
        /*[in]*/ BSTR sPath,
        /*[in]*/ VARIANT pvValue ) = 0;
    virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * pVal ) = 0;
    virtual HRESULT __stdcall get_count (
        /*[out,retval]*/ unsigned int * pVal ) = 0;
    virtual HRESULT __stdcall raw_Add (
        /*[in]*/ BSTR sName,
        /*[in]*/ VARIANT vValue,
        /*[in]*/ VARIANT bNoReplace = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Remove (
        /*[in]*/ BSTR sName ) = 0;
    virtual HRESULT __stdcall raw_Import (
        /*[in]*/ BSTR sText ) = 0;
};


inline Ztl_variant_t IWzProperty::Getitem ( Ztl_bstr_t sPath ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_item(sPath, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline void IWzProperty::Putitem ( Ztl_bstr_t sPath, const Ztl_variant_t & pvValue ) {
    HRESULT _hr = put_item(sPath, pvValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IUnknownPtr IWzProperty::Get_NewEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__NewEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline unsigned int IWzProperty::Getcount ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_count(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IWzProperty::Add ( Ztl_bstr_t sName, const Ztl_variant_t & vValue, const Ztl_variant_t & bNoReplace ) {
    HRESULT _hr = raw_Add(sName, vValue, bNoReplace);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzProperty::Remove ( Ztl_bstr_t sName ) {
    HRESULT _hr = raw_Remove(sName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzProperty::Import ( Ztl_bstr_t sText ) {
    HRESULT _hr = raw_Import(sText);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
