#pragma once
#pragma pack(push, 8)

#include "IWzArchive.h"
#include "ztl/ztl.h"
#include <comdef.h>

enum RESMAN_PARAM : int;
struct __declspec(uuid("57dfe40b-3e20-4dbc-97e8-805a50f381bf"))
/* interface */ IWzResMan;

_COM_SMARTPTR_TYPEDEF(IWzResMan, __uuidof(IWzResMan));

enum RESMAN_PARAM : int
{
    RC_AUTO_SERIALIZE = 1,
    RC_AUTO_SERIALIZE_NO_CACHE = 2,
    RC_NO_AUTO_SERIALIZE = 4,
    RC_DEFAULT_AUTO_SERIALIZE = 0,
    RC_AUTO_SERIALIZE_MASK = 7,
    RC_AUTO_REPARSE = 16,
    RC_NO_AUTO_REPARSE = 32,
    RC_DEFAULT_AUTO_REPARSE = 0,
    RC_AUTO_REPARSE_MASK = 48
};

struct __declspec(uuid("57dfe40b-3e20-4dbc-97e8-805a50f381bf"))
IWzResMan : IUnknown
{
    __declspec(property(get=GetrootNameSpace,put=PutrootNameSpace))
    IUnknownPtr rootNameSpace;

    IUnknownPtr GetrootNameSpace ( );
    void PutrootNameSpace (
        IUnknown * ppVal );
    HRESULT SetResManParam (
        enum RESMAN_PARAM nParam,
        int nRetainTime,
        int nNameSpaceCacheTime );
    IUnknownPtr CreateObject (
        Ztl_bstr_t sUOL );
    Ztl_variant_t GetObjectA (
        Ztl_bstr_t sUOL,
        const Ztl_variant_t & vParam = vtEmpty,
        const Ztl_variant_t & vAux = vtEmpty );
    IUnknownPtr SerializeObject (
        struct IWzArchive * pArchive,
        const Ztl_variant_t & vDispObject = vtEmpty );
    HRESULT FlushCachedObjects (
        int nUsedBefore );
    HRESULT OverrideObject (
        Ztl_bstr_t sOrgUOL,
        Ztl_bstr_t sNewUOL );

    virtual HRESULT __stdcall get_rootNameSpace (
        /*[out,retval]*/ IUnknown * * ppVal ) = 0;
    virtual HRESULT __stdcall put_rootNameSpace (
        /*[in]*/ IUnknown * ppVal ) = 0;
    virtual HRESULT __stdcall raw_SetResManParam (
        /*[in]*/ enum RESMAN_PARAM nParam,
        /*[in]*/ int nRetainTime,
        /*[in]*/ int nNameSpaceCacheTime ) = 0;
    virtual HRESULT __stdcall raw_CreateObject (
        /*[in]*/ BSTR sUOL,
        /*[out,retval]*/ IUnknown * * ppObj ) = 0;
    virtual HRESULT __stdcall raw_GetObjectA (
        /*[in]*/ BSTR sUOL,
        /*[in]*/ VARIANT vParam,
        /*[in]*/ VARIANT vAux,
        /*[out,retval]*/ VARIANT * pvObj ) = 0;
    virtual HRESULT __stdcall raw_SerializeObject (
        /*[in]*/ struct IWzArchive * pArchive,
        /*[in]*/ VARIANT vDispObject,
        /*[out,retval]*/ IUnknown * * ppObj ) = 0;
    virtual HRESULT __stdcall raw_FlushCachedObjects (
        /*[in]*/ int nUsedBefore ) = 0;
    virtual HRESULT __stdcall raw_OverrideObject (
        /*[in]*/ BSTR sOrgUOL,
        /*[in]*/ BSTR sNewUOL ) = 0;
};


inline IUnknownPtr IWzResMan::GetrootNameSpace ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get_rootNameSpace(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void IWzResMan::PutrootNameSpace ( IUnknown * ppVal ) {
    HRESULT _hr = put_rootNameSpace(ppVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzResMan::SetResManParam ( enum RESMAN_PARAM nParam, int nRetainTime, int nNameSpaceCacheTime ) {
    HRESULT _hr = raw_SetResManParam(nParam, nRetainTime, nNameSpaceCacheTime);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IUnknownPtr IWzResMan::CreateObject ( Ztl_bstr_t sUOL ) {
    IUnknown * _result = 0;
    HRESULT _hr = raw_CreateObject(sUOL, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline Ztl_variant_t IWzResMan::GetObjectA ( Ztl_bstr_t sUOL, const Ztl_variant_t & vParam, const Ztl_variant_t & vAux ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_GetObjectA(sUOL, vParam, vAux, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline IUnknownPtr IWzResMan::SerializeObject ( struct IWzArchive * pArchive, const Ztl_variant_t & vDispObject ) {
    IUnknown * _result = 0;
    HRESULT _hr = raw_SerializeObject(pArchive, vDispObject, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline HRESULT IWzResMan::FlushCachedObjects ( int nUsedBefore ) {
    HRESULT _hr = raw_FlushCachedObjects(nUsedBefore);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzResMan::OverrideObject ( Ztl_bstr_t sOrgUOL, Ztl_bstr_t sNewUOL ) {
    HRESULT _hr = raw_OverrideObject(sOrgUOL, sNewUOL);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
