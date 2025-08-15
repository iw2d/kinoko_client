#pragma once
#pragma pack(push, 8)

#include "IWzShape2D.h"
#include "ztl/zcom.h"

struct __declspec(uuid("4cfb57c7-eae3-40b3-ac98-4b2750e3642a"))
/* interface */ IWzVector2D;

_COM_SMARTPTR_TYPEDEF(IWzVector2D, __uuidof(IWzVector2D));

struct __declspec(uuid("4cfb57c7-eae3-40b3-ac98-4b2750e3642a"))
IWzVector2D : IWzShape2D
{
    __declspec(property(get=GetcurrentTime,put=PutcurrentTime))
    int currentTime;
    __declspec(property(get=Getorigin,put=Putorigin))
    Ztl_variant_t origin;
    __declspec(property(get=Getrx,put=Putrx))
    int rx;
    __declspec(property(get=Getry,put=Putry))
    int ry;
    __declspec(property(get=Geta))
    double a;
    __declspec(property(get=Getra,put=Putra))
    double ra;
    __declspec(property(get=GetflipX,put=PutflipX))
    int flipX;
    __declspec(property(get=GetlooseLevel,put=PutlooseLevel))
    unsigned int looseLevel;

    int GetcurrentTime ( );
    void PutcurrentTime (
        int pnTime );
    Ztl_variant_t Getorigin ( );
    void Putorigin (
        const Ztl_variant_t & pvOrigin );
    int Getrx ( );
    void Putrx (
        int pVal );
    int Getry ( );
    void Putry (
        int pVal );
    double Geta ( );
    double Getra ( );
    void Putra (
        double pVal );
    int GetflipX ( );
    void PutflipX (
        int pVal );
    HRESULT _GetSnapshot (
        int * px,
        int * py,
        int * prx,
        int * pry,
        int * pxOrg,
        int * pyOrg,
        double * pa,
        double * paOrg,
        const Ztl_variant_t & vTime = vtEmpty );
    HRESULT RelMove (
        int nX,
        int nY,
        const Ztl_variant_t & nTime = vtEmpty,
        const Ztl_variant_t & nType = vtEmpty );
    HRESULT RelOffset (
        int nDX,
        int nDY,
        const Ztl_variant_t & nTime = vtEmpty,
        const Ztl_variant_t & nType = vtEmpty );
    HRESULT Ratio (
        struct IWzVector2D * pOrigin,
        int nOriginWidth,
        int nOriginHeight,
        int nScaleWidth,
        int nScaleHeight );
    HRESULT WrapClip (
        const Ztl_variant_t & pOrigin,
        int nWrapLeft,
        int nWrapTop,
        unsigned int uWrapWidth,
        unsigned int uWrapHeight,
        const Ztl_variant_t & bClip = vtEmpty );
    HRESULT Rotate (
        double dAngleAmount,
        const Ztl_variant_t & nTime = vtEmpty );
    unsigned int GetlooseLevel ( );
    void PutlooseLevel (
        unsigned int puLevel );
    HRESULT Fly (
        VARIANT * aVar,
        int nArgs );

    virtual HRESULT __stdcall get_currentTime (
        /*[out,retval]*/ int * pnTime ) = 0;
    virtual HRESULT __stdcall put_currentTime (
        /*[in]*/ int pnTime ) = 0;
    virtual HRESULT __stdcall get_origin (
        /*[out,retval]*/ VARIANT * pvOrigin ) = 0;
    virtual HRESULT __stdcall put_origin (
        /*[in]*/ VARIANT pvOrigin ) = 0;
    virtual HRESULT __stdcall get_rx (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_rx (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_ry (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_ry (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_a (
        /*[out,retval]*/ double * pVal ) = 0;
    virtual HRESULT __stdcall get_ra (
        /*[out,retval]*/ double * pVal ) = 0;
    virtual HRESULT __stdcall put_ra (
        /*[in]*/ double pVal ) = 0;
    virtual HRESULT __stdcall get_flipX (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_flipX (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall raw__GetSnapshot (
        /*[out]*/ int * px,
        /*[out]*/ int * py,
        /*[out]*/ int * prx,
        /*[out]*/ int * pry,
        /*[out]*/ int * pxOrg,
        /*[out]*/ int * pyOrg,
        /*[out]*/ double * pa,
        /*[out]*/ double * paOrg,
        /*[in]*/ VARIANT vTime = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_RelMove (
        /*[in]*/ int nX,
        /*[in]*/ int nY,
        /*[in]*/ VARIANT nTime = vtEmpty,
        /*[in]*/ VARIANT nType = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_RelOffset (
        /*[in]*/ int nDX,
        /*[in]*/ int nDY,
        /*[in]*/ VARIANT nTime = vtEmpty,
        /*[in]*/ VARIANT nType = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Ratio (
        /*[in]*/ struct IWzVector2D * pOrigin,
        /*[in]*/ int nOriginWidth,
        /*[in]*/ int nOriginHeight,
        /*[in]*/ int nScaleWidth,
        /*[in]*/ int nScaleHeight ) = 0;
    virtual HRESULT __stdcall raw_WrapClip (
        /*[in]*/ VARIANT pOrigin,
        /*[in]*/ int nWrapLeft,
        /*[in]*/ int nWrapTop,
        /*[in]*/ unsigned int uWrapWidth,
        /*[in]*/ unsigned int uWrapHeight,
        /*[in]*/ VARIANT bClip = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Rotate (
        /*[in]*/ double dAngleAmount,
        /*[in]*/ VARIANT nTime = vtEmpty ) = 0;
    virtual HRESULT __stdcall get_looseLevel (
        /*[out,retval]*/ unsigned int * puLevel ) = 0;
    virtual HRESULT __stdcall put_looseLevel (
        /*[in]*/ unsigned int puLevel ) = 0;
    virtual HRESULT __stdcall raw_Fly (
        /*[in]*/ VARIANT * aVar,
        /*[in]*/ int nArgs ) = 0;
};


inline int IWzVector2D::GetcurrentTime ( ) {
    int _result = 0;
    HRESULT _hr = get_currentTime(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::PutcurrentTime ( int pnTime ) {
    HRESULT _hr = put_currentTime(pnTime);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_variant_t IWzVector2D::Getorigin ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_origin(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline void IWzVector2D::Putorigin ( const Ztl_variant_t & pvOrigin ) {
    HRESULT _hr = put_origin(pvOrigin);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzVector2D::Getrx ( ) {
    int _result = 0;
    HRESULT _hr = get_rx(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::Putrx ( int pVal ) {
    HRESULT _hr = put_rx(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzVector2D::Getry ( ) {
    int _result = 0;
    HRESULT _hr = get_ry(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::Putry ( int pVal ) {
    HRESULT _hr = put_ry(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline double IWzVector2D::Geta ( ) {
    double _result = 0;
    HRESULT _hr = get_a(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline double IWzVector2D::Getra ( ) {
    double _result = 0;
    HRESULT _hr = get_ra(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::Putra ( double pVal ) {
    HRESULT _hr = put_ra(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzVector2D::GetflipX ( ) {
    int _result = 0;
    HRESULT _hr = get_flipX(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::PutflipX ( int pVal ) {
    HRESULT _hr = put_flipX(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzVector2D::_GetSnapshot ( int * px, int * py, int * prx, int * pry, int * pxOrg, int * pyOrg, double * pa, double * paOrg, const Ztl_variant_t & vTime ) {
    HRESULT _hr = raw__GetSnapshot(px, py, prx, pry, pxOrg, pyOrg, pa, paOrg, vTime);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzVector2D::RelMove ( int nX, int nY, const Ztl_variant_t & nTime, const Ztl_variant_t & nType ) {
    HRESULT _hr = raw_RelMove(nX, nY, nTime, nType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzVector2D::RelOffset ( int nDX, int nDY, const Ztl_variant_t & nTime, const Ztl_variant_t & nType ) {
    HRESULT _hr = raw_RelOffset(nDX, nDY, nTime, nType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzVector2D::Ratio ( struct IWzVector2D * pOrigin, int nOriginWidth, int nOriginHeight, int nScaleWidth, int nScaleHeight ) {
    HRESULT _hr = raw_Ratio(pOrigin, nOriginWidth, nOriginHeight, nScaleWidth, nScaleHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzVector2D::WrapClip ( const Ztl_variant_t & pOrigin, int nWrapLeft, int nWrapTop, unsigned int uWrapWidth, unsigned int uWrapHeight, const Ztl_variant_t & bClip ) {
    HRESULT _hr = raw_WrapClip(pOrigin, nWrapLeft, nWrapTop, uWrapWidth, uWrapHeight, bClip);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzVector2D::Rotate ( double dAngleAmount, const Ztl_variant_t & nTime ) {
    HRESULT _hr = raw_Rotate(dAngleAmount, nTime);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline unsigned int IWzVector2D::GetlooseLevel ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_looseLevel(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzVector2D::PutlooseLevel ( unsigned int puLevel ) {
    HRESULT _hr = put_looseLevel(puLevel);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzVector2D::Fly ( VARIANT * aVar, int nArgs ) {
    HRESULT _hr = raw_Fly(aVar, nArgs);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
