#pragma once
#pragma pack(push, 8)

#include "IWzArchive.h"
#include "ztl/zcom.h"

struct __declspec(uuid("35c1f133-7f61-496e-878f-9a1758afa9ea"))
/* interface */ IWzSeekableArchive;

_COM_SMARTPTR_TYPEDEF(IWzSeekableArchive, __uuidof(IWzSeekableArchive));

struct __declspec(uuid("35c1f133-7f61-496e-878f-9a1758afa9ea"))
IWzSeekableArchive : IWzArchive
{
    __declspec(property(put=Putposition))
    unsigned int position;
    __declspec(property(get=Getlength,put=Putlength))
    unsigned int length;
    __declspec(property(get=Getaddress))
    Ztl_variant_t address;

    void Putposition (
        unsigned int _arg1 );
    unsigned int Getlength ( );
    void Putlength (
        unsigned int puLength );
    Ztl_variant_t Getaddress ( );
    HRESULT Rewind ( );
    IWzSeekableArchivePtr ReadLock (
        Ztl_bstr_t sRelUOL,
        unsigned int uSize );

    virtual HRESULT __stdcall put_position (
        /*[in]*/ unsigned int _arg1 ) = 0;
    virtual HRESULT __stdcall get_length (
        /*[out,retval]*/ unsigned int * puLength ) = 0;
    virtual HRESULT __stdcall put_length (
        /*[in]*/ unsigned int puLength ) = 0;
    virtual HRESULT __stdcall get_address (
        /*[out,retval]*/ VARIANT * pv ) = 0;
    virtual HRESULT __stdcall raw_Rewind ( ) = 0;
    virtual HRESULT __stdcall raw_ReadLock (
        /*[in]*/ BSTR sRelUOL,
        /*[in]*/ unsigned int uSize,
        /*[out,retval]*/ struct IWzSeekableArchive * * ppArchive ) = 0;
};


inline void IWzSeekableArchive::Putposition ( unsigned int _arg1 ) {
    HRESULT _hr = put_position(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzSeekableArchive::Getlength ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSeekableArchive::Putlength ( unsigned int puLength ) {
    HRESULT _hr = put_length(puLength);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_variant_t IWzSeekableArchive::Getaddress ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_address(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline HRESULT IWzSeekableArchive::Rewind ( ) {
    HRESULT _hr = raw_Rewind();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IWzSeekableArchivePtr IWzSeekableArchive::ReadLock ( Ztl_bstr_t sRelUOL, unsigned int uSize ) {
    struct IWzSeekableArchive * _result = 0;
    HRESULT _hr = raw_ReadLock(sRelUOL, uSize, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzSeekableArchivePtr(_result, false);
}


#pragma pack(pop)
