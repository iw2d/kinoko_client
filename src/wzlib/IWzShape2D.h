#pragma once
#pragma pack(push, 8)

#include "IWzSerialize.h"
#include "ztl/zcom.h"

struct __declspec(uuid("f28bd1ed-3deb-4f92-9eec-10ef5a1c3fb4"))
/* interface */ IWzShape2D;

_COM_SMARTPTR_TYPEDEF(IWzShape2D, __uuidof(IWzShape2D));

struct __declspec(uuid("f28bd1ed-3deb-4f92-9eec-10ef5a1c3fb4"))
IWzShape2D : IWzSerialize
{
    __declspec(property(get=Getitem))
    Ztl_variant_t item[];
    __declspec(property(get=Get_NewEnum))
    IUnknownPtr _NewEnum;
    __declspec(property(get=Getcount))
    unsigned int count;
    __declspec(property(get=Getx,put=Putx))
    int x;
    __declspec(property(get=Gety,put=Puty))
    int y;
    __declspec(property(get=Getx2,put=Putx2))
    int x2;
    __declspec(property(get=Gety2,put=Puty2))
    int y2;

    Ztl_variant_t Getitem (
        const Ztl_variant_t & vIndex = vtEmpty );
    IUnknownPtr Get_NewEnum ( );
    unsigned int Getcount ( );
    int Getx ( );
    void Putx (
        int pVal );
    int Gety ( );
    void Puty (
        int pVal );
    int Getx2 ( );
    void Putx2 (
        int pVal );
    int Gety2 ( );
    void Puty2 (
        int pVal );
    HRESULT Move (
        int nX,
        int nY );
    HRESULT Offset (
        int nDX,
        int nDY );
    HRESULT Scale (
        int nXMul,
        int nXDiv,
        int nYMul,
        int nYDiv,
        int nXOrg,
        int nYOrg );
    HRESULT Insert (
        const Ztl_variant_t & vIndexOrShape,
        const Ztl_variant_t & vShape = vtEmpty );
    Ztl_variant_t Remove (
        const Ztl_variant_t & vIndex );
    HRESULT Init (
        int nX,
        int nY );

    virtual HRESULT __stdcall get_item (
        /*[in]*/ VARIANT vIndex,
        /*[out,retval]*/ VARIANT * pvValue ) = 0;
    virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * pVal ) = 0;
    virtual HRESULT __stdcall get_count (
        /*[out,retval]*/ unsigned int * pVal ) = 0;
    virtual HRESULT __stdcall get_x (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_x (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_y (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_y (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_x2 (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_x2 (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_y2 (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_y2 (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall raw_Move (
        /*[in]*/ int nX,
        /*[in]*/ int nY ) = 0;
    virtual HRESULT __stdcall raw_Offset (
        /*[in]*/ int nDX,
        /*[in]*/ int nDY ) = 0;
    virtual HRESULT __stdcall raw_Scale (
        /*[in]*/ int nXMul,
        /*[in]*/ int nXDiv,
        /*[in]*/ int nYMul,
        /*[in]*/ int nYDiv,
        /*[in]*/ int nXOrg,
        /*[in]*/ int nYOrg ) = 0;
    virtual HRESULT __stdcall raw_Insert (
        /*[in]*/ VARIANT vIndexOrShape,
        /*[in]*/ VARIANT vShape = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Remove (
        /*[in]*/ VARIANT vIndex,
        /*[out,retval]*/ VARIANT * pvRemoved ) = 0;
    virtual HRESULT __stdcall raw_Init (
        /*[in]*/ int nX,
        /*[in]*/ int nY ) = 0;
};


inline Ztl_variant_t IWzShape2D::Getitem ( const Ztl_variant_t & vIndex ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_item(vIndex, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline IUnknownPtr IWzShape2D::Get_NewEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__NewEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline unsigned int IWzShape2D::Getcount ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_count(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzShape2D::Getx ( ) {
    int _result = 0;
    HRESULT _hr = get_x(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzShape2D::Putx ( int pVal ) {
    HRESULT _hr = put_x(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzShape2D::Gety ( ) {
    int _result = 0;
    HRESULT _hr = get_y(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzShape2D::Puty ( int pVal ) {
    HRESULT _hr = put_y(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzShape2D::Getx2 ( ) {
    int _result = 0;
    HRESULT _hr = get_x2(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzShape2D::Putx2 ( int pVal ) {
    HRESULT _hr = put_x2(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzShape2D::Gety2 ( ) {
    int _result = 0;
    HRESULT _hr = get_y2(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzShape2D::Puty2 ( int pVal ) {
    HRESULT _hr = put_y2(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzShape2D::Move ( int nX, int nY ) {
    HRESULT _hr = raw_Move(nX, nY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzShape2D::Offset ( int nDX, int nDY ) {
    HRESULT _hr = raw_Offset(nDX, nDY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzShape2D::Scale ( int nXMul, int nXDiv, int nYMul, int nYDiv, int nXOrg, int nYOrg ) {
    HRESULT _hr = raw_Scale(nXMul, nXDiv, nYMul, nYDiv, nXOrg, nYOrg);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzShape2D::Insert ( const Ztl_variant_t & vIndexOrShape, const Ztl_variant_t & vShape ) {
    HRESULT _hr = raw_Insert(vIndexOrShape, vShape);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline Ztl_variant_t IWzShape2D::Remove ( const Ztl_variant_t & vIndex ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_Remove(vIndex, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline HRESULT IWzShape2D::Init ( int nX, int nY ) {
    HRESULT _hr = raw_Init(nX, nY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
