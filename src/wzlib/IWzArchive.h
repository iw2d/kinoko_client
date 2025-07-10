#pragma once
#pragma pack(push, 8)

#include "ztl/ztl.h"
#include <comdef.h>

struct __declspec(uuid("3c39b9aa-18cc-408c-8716-c4fb2117266e"))
/* interface */ IWzArchive;

_COM_SMARTPTR_TYPEDEF(IWzArchive, __uuidof(IWzArchive));

struct __declspec(uuid("3c39b9aa-18cc-408c-8716-c4fb2117266e"))
IWzArchive : IUnknown
{
    __declspec(property(get=Getloading))
    int loading;
    __declspec(property(get=GetabsoluteUOL,put=PutabsoluteUOL))
    Ztl_bstr_t absoluteUOL;
    __declspec(property(get=Getposition))
    unsigned int position;
    __declspec(property(get=Getcontext,put=Putcontext))
    Ztl_variant_t context;

    int Getloading ( );
    HRESULT Read (
        unsigned char * pv,
        unsigned int cb,
        unsigned int * pcbRead );
    HRESULT Write (
        unsigned char * pBuffer,
        unsigned int cb,
        unsigned int * pcbWritten );
    Ztl_bstr_t GetabsoluteUOL ( );
    void PutabsoluteUOL (
        Ztl_bstr_t pVal );
    unsigned int Getposition ( );
    Ztl_variant_t Getcontext ( );
    void Putcontext (
        const Ztl_variant_t & pvContext );

    virtual HRESULT __stdcall get_loading (
        /*[out,retval]*/ int * pbLoading ) = 0;
    virtual HRESULT __stdcall raw_Read (
        /*[out]*/ unsigned char * pv,
        /*[in]*/ unsigned int cb,
        /*[out]*/ unsigned int * pcbRead ) = 0;
    virtual HRESULT __stdcall raw_Write (
        /*[in]*/ unsigned char * pBuffer,
        /*[in]*/ unsigned int cb,
        /*[out]*/ unsigned int * pcbWritten ) = 0;
    virtual HRESULT __stdcall get_absoluteUOL (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_absoluteUOL (
        /*[in]*/ BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_position (
        /*[out,retval]*/ unsigned int * puPos ) = 0;
    virtual HRESULT __stdcall get_context (
        /*[out,retval]*/ VARIANT * pvContext ) = 0;
    virtual HRESULT __stdcall put_context (
        /*[in]*/ VARIANT pvContext ) = 0;
};


inline int IWzArchive::Getloading ( ) {
    int _result = 0;
    HRESULT _hr = get_loading(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IWzArchive::Read ( unsigned char * pv, unsigned int cb, unsigned int * pcbRead ) {
    HRESULT _hr = raw_Read(pv, cb, pcbRead);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzArchive::Write ( unsigned char * pBuffer, unsigned int cb, unsigned int * pcbWritten ) {
    HRESULT _hr = raw_Write(pBuffer, cb, pcbWritten);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline Ztl_bstr_t IWzArchive::GetabsoluteUOL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_absoluteUOL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline void IWzArchive::PutabsoluteUOL ( Ztl_bstr_t pVal ) {
    HRESULT _hr = put_absoluteUOL(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzArchive::Getposition ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_position(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline Ztl_variant_t IWzArchive::Getcontext ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_context(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline void IWzArchive::Putcontext ( const Ztl_variant_t & pvContext ) {
    HRESULT _hr = put_context(pvContext);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}


#pragma pack(pop)
