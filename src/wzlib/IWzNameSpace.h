#pragma once
#pragma pack(push, 8)

#include "ztl/zcom.h"

struct __declspec(uuid("a0b5164f-f89b-41b8-8264-15371fd02b4c"))
/* interface */ IWzNameSpaceProperty;
struct __declspec(uuid("2aeeeb36-a4e1-4e2b-8f6f-2e7bdec5c53d"))
/* interface */ IWzNameSpace;

_COM_SMARTPTR_TYPEDEF(IWzNameSpaceProperty, __uuidof(IWzNameSpaceProperty));
_COM_SMARTPTR_TYPEDEF(IWzNameSpace, __uuidof(IWzNameSpace));

struct __declspec(uuid("a0b5164f-f89b-41b8-8264-15371fd02b4c"))
IWzNameSpaceProperty : IUnknown
{
    __declspec(property(get=Getitem))
    Ztl_variant_t item[];
    __declspec(property(get=Getname))
    Ztl_bstr_t name;
    __declspec(property(get=Getpath))
    Ztl_bstr_t path;
    __declspec(property(get=Getsize))
    Ztl_variant_t size;
    __declspec(property(get=Getchecksum))
    int checksum;
    __declspec(property(get=Getproperty))
    Ztl_variant_t property[];

    Ztl_variant_t Getitem (
        const Ztl_variant_t & vAux = vtEmpty );
    Ztl_bstr_t Getname ( );
    Ztl_bstr_t Getpath ( );
    Ztl_variant_t Getsize ( );
    int Getchecksum ( );
    Ztl_variant_t Getproperty (
        Ztl_bstr_t sPropName );

    virtual HRESULT __stdcall get_item (
        /*[in]*/ VARIANT vAux,
        /*[out,retval]*/ VARIANT * pvObj ) = 0;
    virtual HRESULT __stdcall get_name (
        /*[out,retval]*/ BSTR * psVal ) = 0;
    virtual HRESULT __stdcall get_path (
        /*[out,retval]*/ BSTR * psVal ) = 0;
    virtual HRESULT __stdcall get_size (
        /*[out,retval]*/ VARIANT * pvVal ) = 0;
    virtual HRESULT __stdcall get_checksum (
        /*[out,retval]*/ int * pnVal ) = 0;
    virtual HRESULT __stdcall get_property (
        /*[in]*/ BSTR sPropName,
        /*[out,retval]*/ VARIANT * pvVal ) = 0;
};

struct __declspec(uuid("2aeeeb36-a4e1-4e2b-8f6f-2e7bdec5c53d"))
IWzNameSpace : IUnknown
{
    __declspec(property(get=Getitem))
    Ztl_variant_t item[];
    __declspec(property(get=Getproperty))
    IWzNameSpacePropertyPtr property[][];
    __declspec(property(get=Get_NewEnum))
    IUnknownPtr _NewEnum;

    Ztl_variant_t Getitem (
        Ztl_bstr_t sPath );
    IWzNameSpacePropertyPtr Getproperty (
        Ztl_bstr_t sPath,
        const Ztl_variant_t & vpPathUsed = vtEmpty );
    IUnknownPtr Get_NewEnum ( );
    HRESULT Mount (
        Ztl_bstr_t sPath,
        struct IWzNameSpace * pDown,
        int nPriority );
    HRESULT Unmount (
        Ztl_bstr_t sPath,
        const Ztl_variant_t & vDown );
    HRESULT _OnMountEvent (
        struct IWzNameSpace * pUp,
        struct IWzNameSpace * pRoot,
        Ztl_bstr_t sLocation,
        int nState );
    Ztl_variant_t _OnGetLocalObject (
        int nIndex,
        Ztl_bstr_t sPath,
        int * pnPathUsed );

    virtual HRESULT __stdcall get_item (
        /*[in]*/ BSTR sPath,
        /*[out,retval]*/ VARIANT * pvObj ) = 0;
    virtual HRESULT __stdcall get_property (
        /*[in]*/ BSTR sPath,
        /*[in]*/ VARIANT vpPathUsed,
        /*[out,retval]*/ struct IWzNameSpaceProperty * * ppVal ) = 0;
    virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * pVal ) = 0;
    virtual HRESULT __stdcall raw_Mount (
        /*[in]*/ BSTR sPath,
        /*[in]*/ struct IWzNameSpace * pDown,
        /*[in]*/ int nPriority ) = 0;
    virtual HRESULT __stdcall raw_Unmount (
        /*[in]*/ BSTR sPath,
        /*[in]*/ VARIANT vDown ) = 0;
    virtual HRESULT __stdcall raw__OnMountEvent (
        /*[in]*/ struct IWzNameSpace * pUp,
        /*[in]*/ struct IWzNameSpace * pRoot,
        /*[in]*/ BSTR sLocation,
        /*[in]*/ int nState ) = 0;
    virtual HRESULT __stdcall raw__OnGetLocalObject (
        /*[in]*/ int nIndex,
        /*[in]*/ BSTR sPath,
        /*[in,out]*/ int * pnPathUsed,
        /*[out,retval]*/ VARIANT * pvRet ) = 0;
};


inline Ztl_variant_t IWzNameSpaceProperty::Getitem ( const Ztl_variant_t & vAux ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_item(vAux, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline Ztl_bstr_t IWzNameSpaceProperty::Getname ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline Ztl_bstr_t IWzNameSpaceProperty::Getpath ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_path(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline Ztl_variant_t IWzNameSpaceProperty::Getsize ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_size(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline int IWzNameSpaceProperty::Getchecksum ( ) {
    int _result = 0;
    HRESULT _hr = get_checksum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline Ztl_variant_t IWzNameSpaceProperty::Getproperty ( Ztl_bstr_t sPropName ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_property(sPropName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline Ztl_variant_t IWzNameSpace::Getitem ( Ztl_bstr_t sPath ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_item(sPath, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline IWzNameSpacePropertyPtr IWzNameSpace::Getproperty ( Ztl_bstr_t sPath, const Ztl_variant_t & vpPathUsed ) {
    struct IWzNameSpaceProperty * _result = 0;
    HRESULT _hr = get_property(sPath, vpPathUsed, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzNameSpacePropertyPtr(_result, false);
}

inline IUnknownPtr IWzNameSpace::Get_NewEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__NewEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline HRESULT IWzNameSpace::Mount ( Ztl_bstr_t sPath, struct IWzNameSpace * pDown, int nPriority ) {
    HRESULT _hr = raw_Mount(sPath, pDown, nPriority);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzNameSpace::Unmount ( Ztl_bstr_t sPath, const Ztl_variant_t & vDown ) {
    HRESULT _hr = raw_Unmount(sPath, vDown);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzNameSpace::_OnMountEvent ( struct IWzNameSpace * pUp, struct IWzNameSpace * pRoot, Ztl_bstr_t sLocation, int nState ) {
    HRESULT _hr = raw__OnMountEvent(pUp, pRoot, sLocation, nState);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline Ztl_variant_t IWzNameSpace::_OnGetLocalObject ( int nIndex, Ztl_bstr_t sPath, int * pnPathUsed ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw__OnGetLocalObject(nIndex, sPath, pnPathUsed, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}


#pragma pack(pop)
