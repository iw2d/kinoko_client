#pragma once
#pragma pack(push, 8)

#include "IWzCanvas.h"
#include "IWzVector2D.h"
#include "ztl/zcom.h"

enum LAYER_BLENDTYPE : int;
enum GR2D_ANITYPE : int;
struct __declspec(uuid("6dc8c7ce-8e81-4420-b4f6-4b60b7d5fcdf"))
/* interface */ IWzGr2DLayer;

_COM_SMARTPTR_TYPEDEF(IWzGr2DLayer, __uuidof(IWzGr2DLayer));

enum LAYER_BLENDTYPE : int
{
    LB_NORMAL = 0,
    LB_ADD = 1,
    LB_INVERSE = 2,
    LB_ISOLATED = 4
};

enum GR2D_ANITYPE : int
{
    GA_NORMAL = 0,
    GA_STOP = 0,
    GA_FIRST = 16,
    GA_REPEAT = 32,
    GA_WAIT = 256
};

struct __declspec(uuid("6dc8c7ce-8e81-4420-b4f6-4b60b7d5fcdf"))
IWzGr2DLayer : IWzVector2D
{
    __declspec(property(get=Getz,put=Putz))
    int z;
    __declspec(property(get=Getwidth,put=Putwidth))
    int width;
    __declspec(property(get=Getheight,put=Putheight))
    int height;
    __declspec(property(get=Getlt))
    IWzVector2DPtr lt;
    __declspec(property(get=Getrb))
    IWzVector2DPtr rb;
    __declspec(property(get=Getflip,put=Putflip))
    int flip;
    __declspec(property(get=Getcolor,put=Putcolor))
    unsigned int color;
    __declspec(property(get=Getalpha))
    IWzVector2DPtr alpha;
    __declspec(property(get=GetredTone))
    IWzVector2DPtr redTone;
    __declspec(property(get=GetgreenBlueTone))
    IWzVector2DPtr greenBlueTone;
    __declspec(property(get=Getblend,put=Putblend))
    enum LAYER_BLENDTYPE blend;
    __declspec(property(get=Getoverlay,put=Putoverlay))
    Ztl_variant_t overlay;
    __declspec(property(get=Getcanvas))
    IWzCanvasPtr canvas[];
    __declspec(property(get=GetanimationState))
    int animationState;
    __declspec(property(get=Getvisible,put=Putvisible))
    int visible;

    int Getz ( );
    void Putz (
        int pnZ );
    int Getwidth ( );
    void Putwidth (
        int pnWidth );
    int Getheight ( );
    void Putheight (
        int pnHeight );
    IWzVector2DPtr Getlt ( );
    IWzVector2DPtr Getrb ( );
    HRESULT InterLockedOffset (
        int xlt,
        int ylt,
        int xrb,
        int yrb );
    int Getflip ( );
    void Putflip (
        int pnFlip );
    unsigned int Getcolor ( );
    void Putcolor (
        unsigned int puColor );
    IWzVector2DPtr Getalpha ( );
    IWzVector2DPtr GetredTone ( );
    IWzVector2DPtr GetgreenBlueTone ( );
    enum LAYER_BLENDTYPE Getblend ( );
    void Putblend (
        enum LAYER_BLENDTYPE pnBlend );
    Ztl_variant_t Getoverlay ( );
    void Putoverlay (
        const Ztl_variant_t & pvLayer );
    IWzCanvasPtr Getcanvas (
        const Ztl_variant_t & vIndex = vtEmpty );
    Ztl_variant_t InsertCanvas (
        struct IWzCanvas * pCanvas,
        const Ztl_variant_t & vDelay = vtEmpty,
        const Ztl_variant_t & vAlpha0 = vtEmpty,
        const Ztl_variant_t & vAlpha1 = vtEmpty,
        const Ztl_variant_t & vZoom0 = vtEmpty,
        const Ztl_variant_t & vZoom1 = vtEmpty );
    IWzCanvasPtr RemoveCanvas (
        const Ztl_variant_t & vIndex = vtEmpty );
    HRESULT ShiftCanvas (
        const Ztl_variant_t & vValue = vtEmpty );
    HRESULT Animate (
        enum GR2D_ANITYPE nType,
        const Ztl_variant_t & vDelayRate = vtEmpty,
        const Ztl_variant_t & vRepeat = vtEmpty );
    int GetanimationState ( );
    int Getvisible ( );
    void Putvisible (
        int pnVisible );

    virtual HRESULT __stdcall get_z (
        /*[out,retval]*/ int * pnZ ) = 0;
    virtual HRESULT __stdcall put_z (
        /*[in]*/ int pnZ ) = 0;
    virtual HRESULT __stdcall get_width (
        /*[out,retval]*/ int * pnWidth ) = 0;
    virtual HRESULT __stdcall put_width (
        /*[in]*/ int pnWidth ) = 0;
    virtual HRESULT __stdcall get_height (
        /*[out,retval]*/ int * pnHeight ) = 0;
    virtual HRESULT __stdcall put_height (
        /*[in]*/ int pnHeight ) = 0;
    virtual HRESULT __stdcall get_lt (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_rb (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall raw_InterLockedOffset (
        /*[in]*/ int xlt,
        /*[in]*/ int ylt,
        /*[in]*/ int xrb,
        /*[in]*/ int yrb ) = 0;
    virtual HRESULT __stdcall get_flip (
        /*[out,retval]*/ int * pnFlip ) = 0;
    virtual HRESULT __stdcall put_flip (
        /*[in]*/ int pnFlip ) = 0;
    virtual HRESULT __stdcall get_color (
        /*[out,retval]*/ unsigned int * puColor ) = 0;
    virtual HRESULT __stdcall put_color (
        /*[in]*/ unsigned int puColor ) = 0;
    virtual HRESULT __stdcall get_alpha (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_redTone (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_greenBlueTone (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_blend (
        /*[out,retval]*/ enum LAYER_BLENDTYPE * pnBlend ) = 0;
    virtual HRESULT __stdcall put_blend (
        /*[in]*/ enum LAYER_BLENDTYPE pnBlend ) = 0;
    virtual HRESULT __stdcall get_overlay (
        /*[out,retval]*/ VARIANT * pvLayer ) = 0;
    virtual HRESULT __stdcall put_overlay (
        /*[in]*/ VARIANT pvLayer ) = 0;
    virtual HRESULT __stdcall get_canvas (
        /*[in]*/ VARIANT vIndex,
        /*[out,retval]*/ struct IWzCanvas * * ppCanvas ) = 0;
    virtual HRESULT __stdcall raw_InsertCanvas (
        /*[in]*/ struct IWzCanvas * pCanvas,
        /*[in]*/ VARIANT vDelay,
        /*[in]*/ VARIANT vAlpha0,
        /*[in]*/ VARIANT vAlpha1,
        /*[in]*/ VARIANT vZoom0,
        /*[in]*/ VARIANT vZoom1,
        /*[out,retval]*/ VARIANT * pvIndex ) = 0;
    virtual HRESULT __stdcall raw_RemoveCanvas (
        /*[in]*/ VARIANT vIndex,
        /*[out,retval]*/ struct IWzCanvas * * ppCanvas ) = 0;
    virtual HRESULT __stdcall raw_ShiftCanvas (
        /*[in]*/ VARIANT vValue = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Animate (
        /*[in]*/ enum GR2D_ANITYPE nType,
        /*[in]*/ VARIANT vDelayRate = vtEmpty,
        /*[in]*/ VARIANT vRepeat = vtEmpty ) = 0;
    virtual HRESULT __stdcall get_animationState (
        /*[out,retval]*/ int * pnState ) = 0;
    virtual HRESULT __stdcall get_visible (
        /*[out,retval]*/ int * pnVisible ) = 0;
    virtual HRESULT __stdcall put_visible (
        /*[in]*/ int pnVisible ) = 0;
};


inline int IWzGr2DLayer::Getz ( ) {
    int _result = 0;
    HRESULT _hr = get_z(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putz ( int pnZ ) {
    HRESULT _hr = put_z(pnZ);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzGr2DLayer::Getwidth ( ) {
    int _result = 0;
    HRESULT _hr = get_width(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putwidth ( int pnWidth ) {
    HRESULT _hr = put_width(pnWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzGr2DLayer::Getheight ( ) {
    int _result = 0;
    HRESULT _hr = get_height(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putheight ( int pnHeight ) {
    HRESULT _hr = put_height(pnHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzVector2DPtr IWzGr2DLayer::Getlt ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_lt(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline IWzVector2DPtr IWzGr2DLayer::Getrb ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_rb(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline HRESULT IWzGr2DLayer::InterLockedOffset ( int xlt, int ylt, int xrb, int yrb ) {
    HRESULT _hr = raw_InterLockedOffset(xlt, ylt, xrb, yrb);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzGr2DLayer::Getflip ( ) {
    int _result = 0;
    HRESULT _hr = get_flip(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putflip ( int pnFlip ) {
    HRESULT _hr = put_flip(pnFlip);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzGr2DLayer::Getcolor ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_color(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putcolor ( unsigned int puColor ) {
    HRESULT _hr = put_color(puColor);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzVector2DPtr IWzGr2DLayer::Getalpha ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_alpha(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline IWzVector2DPtr IWzGr2DLayer::GetredTone ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_redTone(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline IWzVector2DPtr IWzGr2DLayer::GetgreenBlueTone ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_greenBlueTone(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline enum LAYER_BLENDTYPE IWzGr2DLayer::Getblend ( ) {
    enum LAYER_BLENDTYPE _result;
    HRESULT _hr = get_blend(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putblend ( enum LAYER_BLENDTYPE pnBlend ) {
    HRESULT _hr = put_blend(pnBlend);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_variant_t IWzGr2DLayer::Getoverlay ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_overlay(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline void IWzGr2DLayer::Putoverlay ( const Ztl_variant_t & pvLayer ) {
    HRESULT _hr = put_overlay(pvLayer);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzCanvasPtr IWzGr2DLayer::Getcanvas ( const Ztl_variant_t & vIndex ) {
    struct IWzCanvas * _result = 0;
    HRESULT _hr = get_canvas(vIndex, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzCanvasPtr(_result, false);
}

inline Ztl_variant_t IWzGr2DLayer::InsertCanvas ( struct IWzCanvas * pCanvas, const Ztl_variant_t & vDelay, const Ztl_variant_t & vAlpha0, const Ztl_variant_t & vAlpha1, const Ztl_variant_t & vZoom0, const Ztl_variant_t & vZoom1 ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_InsertCanvas(pCanvas, vDelay, vAlpha0, vAlpha1, vZoom0, vZoom1, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline IWzCanvasPtr IWzGr2DLayer::RemoveCanvas ( const Ztl_variant_t & vIndex ) {
    struct IWzCanvas * _result = 0;
    HRESULT _hr = raw_RemoveCanvas(vIndex, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzCanvasPtr(_result, false);
}

inline HRESULT IWzGr2DLayer::ShiftCanvas ( const Ztl_variant_t & vValue ) {
    HRESULT _hr = raw_ShiftCanvas(vValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2DLayer::Animate ( enum GR2D_ANITYPE nType, const Ztl_variant_t & vDelayRate, const Ztl_variant_t & vRepeat ) {
    HRESULT _hr = raw_Animate(nType, vDelayRate, vRepeat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzGr2DLayer::GetanimationState ( ) {
    int _result = 0;
    HRESULT _hr = get_animationState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzGr2DLayer::Getvisible ( ) {
    int _result = 0;
    HRESULT _hr = get_visible(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2DLayer::Putvisible ( int pnVisible ) {
    HRESULT _hr = put_visible(pnVisible);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}


#pragma pack(pop)
