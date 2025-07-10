#pragma once
#pragma pack(push, 8)

#include "IWzSerialize.h"
#include "IWzProperty.h"
#include "ztl/ztl.h"
#include <comdef.h>

enum CANVAS_ALPHATYPE : int;
enum CANVAS_LEVELMAP : int;
enum CANVAS_PIXFORMAT : int;
enum CANVAS_DITHERTYPE : int;
struct __declspec(uuid("7600dc6c-9328-4bff-9624-5b0f5c01179e"))
/* interface */ IWzCanvas;
struct __declspec(uuid("58003396-b7c8-4ef4-a863-7622b90b956c"))
/* interface */ IWzRawCanvasAllocator;
struct __declspec(uuid("312126f0-c399-4111-8eab-0f96a30b6b7c"))
/* interface */ IWzRawCanvas;
struct __declspec(uuid("2bef046d-ccd6-445a-88c4-929fc35d30ac"))
/* interface */ IWzFont;

_COM_SMARTPTR_TYPEDEF(IWzRawCanvas, __uuidof(IWzRawCanvas));
_COM_SMARTPTR_TYPEDEF(IWzCanvas, __uuidof(IWzCanvas));
_COM_SMARTPTR_TYPEDEF(IWzRawCanvasAllocator, __uuidof(IWzRawCanvasAllocator));
_COM_SMARTPTR_TYPEDEF(IWzFont, __uuidof(IWzFont));

enum CANVAS_ALPHATYPE : int
{
    CA_OVERWRITE = 255,
    CA_REMOVEALPHA = 254
};

enum CANVAS_LEVELMAP : int
{
    CL_ALL16 = 1,
    CL_ALL32 = 2,
    CL_ALL56 = 513,
    CL_USE32OVER1 = 65538,
    CL_USE32OVER2 = 131074,
    CL_USE56OVER1 = 66049,
    CL_USE56OVER2 = 131585
};

enum CANVAS_PIXFORMAT : int
{
    CP_UNKNOWN = 0,
    CP_A4R4G4B4 = 1,
    CP_A8R8G8B8 = 2,
    CP_R5G6B5 = 513,
    CP_DXT3 = 1026
};

enum CANVAS_DITHERTYPE : int
{
    CD_NONE = 0,
    CD_FS0 = 1,
    CD_FS1 = 2,
    CD_FS = 3,
    CD_FD0 = 17,
    CD_FD1 = 18,
    CD_FD = 19
};

struct __declspec(uuid("312126f0-c399-4111-8eab-0f96a30b6b7c"))
IWzRawCanvas : IUnknown
{
    __declspec(property(get=GetpixelFormat))
    enum CANVAS_PIXFORMAT pixelFormat;
    __declspec(property(get=GetmagLevel))
    int magLevel;
    __declspec(property(get=Getwidth))
    unsigned int width;
    __declspec(property(get=Getheight))
    unsigned int height;

    enum CANVAS_PIXFORMAT GetpixelFormat ( );
    int GetmagLevel ( );
    unsigned int Getwidth ( );
    unsigned int Getheight ( );
    Ztl_variant_t _LockAddress (
        int * pnPitch );
    HRESULT _UnlockAddress (
        struct tagRECT * prc );
    HRESULT SetTexture (
        unsigned int * pTexture );
    struct tagRECT GetTextureSize ( );

    virtual HRESULT __stdcall get_pixelFormat (
        /*[out,retval]*/ enum CANVAS_PIXFORMAT * pnFormat ) = 0;
    virtual HRESULT __stdcall get_magLevel (
        /*[out,retval]*/ int * pnMagLevel ) = 0;
    virtual HRESULT __stdcall get_width (
        /*[out,retval]*/ unsigned int * puWidth ) = 0;
    virtual HRESULT __stdcall get_height (
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
    virtual HRESULT __stdcall raw__LockAddress (
        /*[out]*/ int * pnPitch,
        /*[out,retval]*/ VARIANT * pvAddress ) = 0;
    virtual HRESULT __stdcall raw__UnlockAddress (
        /*[in]*/ struct tagRECT * prc ) = 0;
    virtual HRESULT __stdcall raw_SetTexture (
        /*[in]*/ unsigned int * pTexture ) = 0;
    virtual HRESULT __stdcall raw_GetTextureSize (
        /*[out,retval]*/ struct tagRECT * prc ) = 0;
};

struct __declspec(uuid("7600dc6c-9328-4bff-9624-5b0f5c01179e"))
IWzCanvas : IWzSerialize
{
    __declspec(property(get=GetdefaultDither,put=PutdefaultDither))
    enum CANVAS_DITHERTYPE defaultDither;
    __declspec(property(get=GetdefaultLevelMap,put=PutdefaultLevelMap))
    enum CANVAS_LEVELMAP defaultLevelMap;
    __declspec(property(get=GetdefaultAllocator,put=PutdefaultAllocator))
    IWzRawCanvasAllocatorPtr defaultAllocator;
    __declspec(property(get=GetrawCanvas))
    IWzRawCanvasPtr rawCanvas[][];
    __declspec(property(get=GettileWidth))
    unsigned int tileWidth;
    __declspec(property(get=GettileHeight))
    unsigned int tileHeight;
    __declspec(property(get=Getwidth,put=Putwidth))
    unsigned int width;
    __declspec(property(get=Getheight,put=Putheight))
    unsigned int height;
    __declspec(property(get=GetpixelFormat,put=PutpixelFormat))
    enum CANVAS_PIXFORMAT pixelFormat;
    __declspec(property(get=GetmagLevel,put=PutmagLevel))
    int magLevel;
    __declspec(property(get=Getproperty))
    IWzPropertyPtr property;
    __declspec(property(get=Getcx,put=Putcx))
    int cx;
    __declspec(property(get=Getcy,put=Putcy))
    int cy;
    __declspec(property(get=Getpixel))
    unsigned int pixel[][];

    enum CANVAS_DITHERTYPE GetdefaultDither ( );
    void PutdefaultDither (
        enum CANVAS_DITHERTYPE pnDefaultDither );
    enum CANVAS_LEVELMAP GetdefaultLevelMap ( );
    void PutdefaultLevelMap (
        enum CANVAS_LEVELMAP pnDefaultLevelMap );
    IWzRawCanvasAllocatorPtr GetdefaultAllocator ( );
    void PutdefaultAllocator (
        struct IWzRawCanvasAllocator * ppAllocator );
    HRESULT Create (
        unsigned int uWidth,
        unsigned int uHeight,
        const Ztl_variant_t & vMagLevel = vtEmpty,
        const Ztl_variant_t & vPixFormat = vtEmpty );
    HRESULT AddRawCanvas (
        int x,
        int y,
        struct IWzRawCanvas * pRawCanvas );
    IWzRawCanvasPtr GetrawCanvas (
        int x,
        int y );
    unsigned int GettileWidth ( );
    unsigned int GettileHeight ( );
    unsigned int Getwidth ( );
    void Putwidth (
        unsigned int puWidth );
    unsigned int Getheight ( );
    void Putheight (
        unsigned int puHeight );
    enum CANVAS_PIXFORMAT GetpixelFormat ( );
    void PutpixelFormat (
        enum CANVAS_PIXFORMAT pnFormat );
    int GetmagLevel ( );
    void PutmagLevel (
        int pnMagLevel );
    HRESULT GetSnapshotU (
        unsigned int * puWidth,
        unsigned int * puHeight,
        unsigned int * puTileWidth,
        unsigned int * puTileHeight,
        enum CANVAS_PIXFORMAT * pFormat,
        int * pnMagLevel );
    HRESULT GetSnapshot (
        int * pnWidth,
        int * pnHeight,
        int * pnTileWidth,
        int * pnTileHeight,
        enum CANVAS_PIXFORMAT * pFormat,
        int * pnMagLevel );
    IWzPropertyPtr Getproperty ( );
    int Getcx ( );
    void Putcx (
        int pnX );
    int Getcy ( );
    void Putcy (
        int pnY );
    Ztl_variant_t SetClipRect (
        int nLeft,
        int nTop,
        int nWidth,
        int nHeight,
        const Ztl_variant_t & bTemp = vtEmpty );
    HRESULT Copy (
        int nDstLeft,
        int nDstTop,
        struct IWzCanvas * pSource,
        const Ztl_variant_t & nAlpha = vtEmpty );
    HRESULT CopyEx (
        int nDstLeft,
        int nDstTop,
        struct IWzCanvas * pSource,
        enum CANVAS_ALPHATYPE nAlpha,
        int nWidth,
        int nHeight,
        int nSrcLeft,
        int nSrcTop,
        int nSrcWidth,
        int nSrcHeight,
        const Ztl_variant_t & pAdjust = vtEmpty );
    unsigned int Getpixel (
        int nX,
        int nY );
    HRESULT DrawRectangle (
        int nLeft,
        int nTop,
        unsigned int uWidth,
        unsigned int uHeight,
        unsigned int uColor );
    HRESULT DrawLine (
        int x1,
        int y1,
        int x2,
        int y2,
        unsigned int uColor,
        const Ztl_variant_t & nWidth = vtEmpty );
    HRESULT DrawPolygon ( );
    unsigned int DrawTextA (
        int nLeft,
        int nTop,
        Ztl_bstr_t sText,
        struct IWzFont * pFont,
        const Ztl_variant_t & vAlpha = vtEmpty,
        const Ztl_variant_t & vTabOrg = vtEmpty );

    virtual HRESULT __stdcall get_defaultDither (
        /*[out,retval]*/ enum CANVAS_DITHERTYPE * pnDefaultDither ) = 0;
    virtual HRESULT __stdcall put_defaultDither (
        /*[in]*/ enum CANVAS_DITHERTYPE pnDefaultDither ) = 0;
    virtual HRESULT __stdcall get_defaultLevelMap (
        /*[out,retval]*/ enum CANVAS_LEVELMAP * pnDefaultLevelMap ) = 0;
    virtual HRESULT __stdcall put_defaultLevelMap (
        /*[in]*/ enum CANVAS_LEVELMAP pnDefaultLevelMap ) = 0;
    virtual HRESULT __stdcall get_defaultAllocator (
        /*[out,retval]*/ struct IWzRawCanvasAllocator * * ppAllocator ) = 0;
    virtual HRESULT __stdcall put_defaultAllocator (
        /*[in]*/ struct IWzRawCanvasAllocator * ppAllocator ) = 0;
    virtual HRESULT __stdcall raw_Create (
        /*[in]*/ unsigned int uWidth,
        /*[in]*/ unsigned int uHeight,
        /*[in]*/ VARIANT vMagLevel = vtEmpty,
        /*[in]*/ VARIANT vPixFormat = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_AddRawCanvas (
        /*[in]*/ int x,
        /*[in]*/ int y,
        /*[in]*/ struct IWzRawCanvas * pRawCanvas ) = 0;
    virtual HRESULT __stdcall get_rawCanvas (
        /*[in]*/ int x,
        /*[in]*/ int y,
        /*[out,retval]*/ struct IWzRawCanvas * * ppRawCanvas ) = 0;
    virtual HRESULT __stdcall get_tileWidth (
        /*[out,retval]*/ unsigned int * puWidth ) = 0;
    virtual HRESULT __stdcall get_tileHeight (
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
    virtual HRESULT __stdcall get_width (
        /*[out,retval]*/ unsigned int * puWidth ) = 0;
    virtual HRESULT __stdcall put_width (
        /*[in]*/ unsigned int puWidth ) = 0;
    virtual HRESULT __stdcall get_height (
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
    virtual HRESULT __stdcall put_height (
        /*[in]*/ unsigned int puHeight ) = 0;
    virtual HRESULT __stdcall get_pixelFormat (
        /*[out,retval]*/ enum CANVAS_PIXFORMAT * pnFormat ) = 0;
    virtual HRESULT __stdcall put_pixelFormat (
        /*[in]*/ enum CANVAS_PIXFORMAT pnFormat ) = 0;
    virtual HRESULT __stdcall get_magLevel (
        /*[out,retval]*/ int * pnMagLevel ) = 0;
    virtual HRESULT __stdcall put_magLevel (
        /*[in]*/ int pnMagLevel ) = 0;
    virtual HRESULT __stdcall raw_GetSnapshotU (
        /*[out]*/ unsigned int * puWidth,
        /*[out]*/ unsigned int * puHeight,
        /*[out]*/ unsigned int * puTileWidth,
        /*[out]*/ unsigned int * puTileHeight,
        /*[out]*/ enum CANVAS_PIXFORMAT * pFormat,
        /*[out]*/ int * pnMagLevel ) = 0;
    virtual HRESULT __stdcall raw_GetSnapshot (
        /*[out]*/ int * pnWidth,
        /*[out]*/ int * pnHeight,
        /*[out]*/ int * pnTileWidth,
        /*[out]*/ int * pnTileHeight,
        /*[out]*/ enum CANVAS_PIXFORMAT * pFormat,
        /*[out]*/ int * pnMagLevel ) = 0;
    virtual HRESULT __stdcall get_property (
        /*[out,retval]*/ struct IWzProperty * * ppProperty ) = 0;
    virtual HRESULT __stdcall get_cx (
        /*[out,retval]*/ int * pnX ) = 0;
    virtual HRESULT __stdcall put_cx (
        /*[in]*/ int pnX ) = 0;
    virtual HRESULT __stdcall get_cy (
        /*[out,retval]*/ int * pnY ) = 0;
    virtual HRESULT __stdcall put_cy (
        /*[in]*/ int pnY ) = 0;
    virtual HRESULT __stdcall raw_SetClipRect (
        /*[in]*/ int nLeft,
        /*[in]*/ int nTop,
        /*[in]*/ int nWidth,
        /*[in]*/ int nHeight,
        /*[in]*/ VARIANT bTemp,
        /*[out,retval]*/ VARIANT * pvToken ) = 0;
    virtual HRESULT __stdcall raw_Copy (
        /*[in]*/ int nDstLeft,
        /*[in]*/ int nDstTop,
        /*[in]*/ struct IWzCanvas * pSource,
        /*[in]*/ VARIANT nAlpha = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_CopyEx (
        /*[in]*/ int nDstLeft,
        /*[in]*/ int nDstTop,
        /*[in]*/ struct IWzCanvas * pSource,
        /*[in]*/ enum CANVAS_ALPHATYPE nAlpha,
        /*[in]*/ int nWidth,
        /*[in]*/ int nHeight,
        /*[in]*/ int nSrcLeft,
        /*[in]*/ int nSrcTop,
        /*[in]*/ int nSrcWidth,
        /*[in]*/ int nSrcHeight,
        /*[in]*/ VARIANT pAdjust = vtEmpty ) = 0;
    virtual HRESULT __stdcall get_pixel (
        /*[in]*/ int nX,
        /*[in]*/ int nY,
        /*[out,retval]*/ unsigned int * puColor ) = 0;
    virtual HRESULT __stdcall raw_DrawRectangle (
        /*[in]*/ int nLeft,
        /*[in]*/ int nTop,
        /*[in]*/ unsigned int uWidth,
        /*[in]*/ unsigned int uHeight,
        /*[in]*/ unsigned int uColor ) = 0;
    virtual HRESULT __stdcall raw_DrawLine (
        /*[in]*/ int x1,
        /*[in]*/ int y1,
        /*[in]*/ int x2,
        /*[in]*/ int y2,
        /*[in]*/ unsigned int uColor,
        /*[in]*/ VARIANT nWidth = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_DrawPolygon ( ) = 0;
    virtual HRESULT __stdcall raw_DrawTextA (
        /*[in]*/ int nLeft,
        /*[in]*/ int nTop,
        /*[in]*/ BSTR sText,
        /*[in]*/ struct IWzFont * pFont,
        /*[in]*/ VARIANT vAlpha,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
};

struct __declspec(uuid("58003396-b7c8-4ef4-a863-7622b90b956c"))
IWzRawCanvasAllocator : IUnknown
{
    HRESULT AllocCanvas (
        struct IWzCanvas * pCanvas,
        enum CANVAS_PIXFORMAT nPixFormat,
        int nMagLevel );
    HRESULT ConvertIfNotAvailable (
        struct IWzCanvas * pCanvas,
        int * pnNewPixFormat );

    virtual HRESULT __stdcall raw_AllocCanvas (
        /*[in]*/ struct IWzCanvas * pCanvas,
        /*[in]*/ enum CANVAS_PIXFORMAT nPixFormat,
        /*[in]*/ int nMagLevel ) = 0;
    virtual HRESULT __stdcall raw_ConvertIfNotAvailable (
        /*[in]*/ struct IWzCanvas * pCanvas,
        /*[out]*/ int * pnNewPixFormat ) = 0;
};

struct __declspec(uuid("2bef046d-ccd6-445a-88c4-929fc35d30ac"))
IWzFont : IUnknown
{
    __declspec(property(get=Getcolor))
    int color;
    __declspec(property(get=Getheight))
    int height;
    __declspec(property(get=GetfullHeight))
    int fullHeight;

    HRESULT Create (
        Ztl_bstr_t sName,
        unsigned int uHeight,
        unsigned int uColor,
        const Ztl_variant_t & sStyle = vtEmpty );
    int Getcolor ( );
    int Getheight ( );
    int GetfullHeight ( );
    unsigned int CalcTextWidth (
        Ztl_bstr_t sText,
        const Ztl_variant_t & vTabOrg = vtEmpty );
    int CalcLongestText (
        Ztl_bstr_t sText,
        int nWidth,
        const Ztl_variant_t & vTabOrg = vtEmpty );
    int CalcLongestTextForGlobal (
        Ztl_bstr_t sText,
        int nWidth,
        const Ztl_variant_t & vTabOrg = vtEmpty );
    int CalcLongestTextForGlobalEx (
        Ztl_bstr_t sText,
        int nWidth,
        int * pnPosAdj,
        const Ztl_variant_t & vTabOrg = vtEmpty );
    int CalcLineCountForGlobal (
        Ztl_bstr_t sText,
        int nWidth,
        const Ztl_variant_t & vTabOrg = vtEmpty );
    unsigned int DrawTextA (
        int nLeft,
        int nTop,
        Ztl_bstr_t sText,
        struct IWzCanvas * pCanvas,
        const Ztl_variant_t & vAlpha = vtEmpty,
        const Ztl_variant_t & vTabOrg = vtEmpty );

    virtual HRESULT __stdcall raw_Create (
        /*[in]*/ BSTR sName,
        /*[in]*/ unsigned int uHeight,
        /*[in]*/ unsigned int uColor,
        /*[in]*/ VARIANT sStyle = vtEmpty ) = 0;
    virtual HRESULT __stdcall get_color (
        /*[out,retval]*/ int * pnColor ) = 0;
    virtual HRESULT __stdcall get_height (
        /*[out,retval]*/ int * pnHeight ) = 0;
    virtual HRESULT __stdcall get_fullHeight (
        /*[out,retval]*/ int * pnFullHeight ) = 0;
    virtual HRESULT __stdcall raw_CalcTextWidth (
        /*[in]*/ BSTR sText,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ unsigned int * puWidth ) = 0;
    virtual HRESULT __stdcall raw_CalcLongestText (
        /*[in]*/ BSTR sText,
        /*[in]*/ int nWidth,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ int * pnIndex ) = 0;
    virtual HRESULT __stdcall raw_CalcLongestTextForGlobal (
        /*[in]*/ BSTR sText,
        /*[in]*/ int nWidth,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ int * pnIndex ) = 0;
    virtual HRESULT __stdcall raw_CalcLongestTextForGlobalEx (
        /*[in]*/ BSTR sText,
        /*[in]*/ int nWidth,
        /*[out]*/ int * pnPosAdj,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ int * pnIndex ) = 0;
    virtual HRESULT __stdcall raw_CalcLineCountForGlobal (
        /*[in]*/ BSTR sText,
        /*[in]*/ int nWidth,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ int * pnLineCount ) = 0;
    virtual HRESULT __stdcall raw_DrawTextA (
        /*[in]*/ int nLeft,
        /*[in]*/ int nTop,
        /*[in]*/ BSTR sText,
        /*[in]*/ struct IWzCanvas * pCanvas,
        /*[in]*/ VARIANT vAlpha,
        /*[in]*/ VARIANT vTabOrg,
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
};


inline enum CANVAS_PIXFORMAT IWzRawCanvas::GetpixelFormat ( ) {
    enum CANVAS_PIXFORMAT _result;
    HRESULT _hr = get_pixelFormat(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzRawCanvas::GetmagLevel ( ) {
    int _result = 0;
    HRESULT _hr = get_magLevel(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzRawCanvas::Getwidth ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_width(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzRawCanvas::Getheight ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_height(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline Ztl_variant_t IWzRawCanvas::_LockAddress ( int * pnPitch ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw__LockAddress(pnPitch, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline HRESULT IWzRawCanvas::_UnlockAddress ( struct tagRECT * prc ) {
    HRESULT _hr = raw__UnlockAddress(prc);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzRawCanvas::SetTexture ( unsigned int * pTexture ) {
    HRESULT _hr = raw_SetTexture(pTexture);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline struct tagRECT IWzRawCanvas::GetTextureSize ( ) {
    struct tagRECT _result;
    HRESULT _hr = raw_GetTextureSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline enum CANVAS_DITHERTYPE IWzCanvas::GetdefaultDither ( ) {
    enum CANVAS_DITHERTYPE _result;
    HRESULT _hr = get_defaultDither(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::PutdefaultDither ( enum CANVAS_DITHERTYPE pnDefaultDither ) {
    HRESULT _hr = put_defaultDither(pnDefaultDither);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CANVAS_LEVELMAP IWzCanvas::GetdefaultLevelMap ( ) {
    enum CANVAS_LEVELMAP _result;
    HRESULT _hr = get_defaultLevelMap(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::PutdefaultLevelMap ( enum CANVAS_LEVELMAP pnDefaultLevelMap ) {
    HRESULT _hr = put_defaultLevelMap(pnDefaultLevelMap);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzRawCanvasAllocatorPtr IWzCanvas::GetdefaultAllocator ( ) {
    struct IWzRawCanvasAllocator * _result = 0;
    HRESULT _hr = get_defaultAllocator(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzRawCanvasAllocatorPtr(_result, false);
}

inline void IWzCanvas::PutdefaultAllocator ( struct IWzRawCanvasAllocator * ppAllocator ) {
    HRESULT _hr = put_defaultAllocator(ppAllocator);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzCanvas::Create ( unsigned int uWidth, unsigned int uHeight, const Ztl_variant_t & vMagLevel, const Ztl_variant_t & vPixFormat ) {
    HRESULT _hr = raw_Create(uWidth, uHeight, vMagLevel, vPixFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzCanvas::AddRawCanvas ( int x, int y, struct IWzRawCanvas * pRawCanvas ) {
    HRESULT _hr = raw_AddRawCanvas(x, y, pRawCanvas);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IWzRawCanvasPtr IWzCanvas::GetrawCanvas ( int x, int y ) {
    struct IWzRawCanvas * _result = 0;
    HRESULT _hr = get_rawCanvas(x, y, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzRawCanvasPtr(_result, false);
}

inline unsigned int IWzCanvas::GettileWidth ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_tileWidth(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzCanvas::GettileHeight ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_tileHeight(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzCanvas::Getwidth ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_width(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::Putwidth ( unsigned int puWidth ) {
    HRESULT _hr = put_width(puWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzCanvas::Getheight ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_height(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::Putheight ( unsigned int puHeight ) {
    HRESULT _hr = put_height(puHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CANVAS_PIXFORMAT IWzCanvas::GetpixelFormat ( ) {
    enum CANVAS_PIXFORMAT _result;
    HRESULT _hr = get_pixelFormat(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::PutpixelFormat ( enum CANVAS_PIXFORMAT pnFormat ) {
    HRESULT _hr = put_pixelFormat(pnFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzCanvas::GetmagLevel ( ) {
    int _result = 0;
    HRESULT _hr = get_magLevel(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::PutmagLevel ( int pnMagLevel ) {
    HRESULT _hr = put_magLevel(pnMagLevel);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzCanvas::GetSnapshotU ( unsigned int * puWidth, unsigned int * puHeight, unsigned int * puTileWidth, unsigned int * puTileHeight, enum CANVAS_PIXFORMAT * pFormat, int * pnMagLevel ) {
    HRESULT _hr = raw_GetSnapshotU(puWidth, puHeight, puTileWidth, puTileHeight, pFormat, pnMagLevel);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzCanvas::GetSnapshot ( int * pnWidth, int * pnHeight, int * pnTileWidth, int * pnTileHeight, enum CANVAS_PIXFORMAT * pFormat, int * pnMagLevel ) {
    HRESULT _hr = raw_GetSnapshot(pnWidth, pnHeight, pnTileWidth, pnTileHeight, pFormat, pnMagLevel);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IWzPropertyPtr IWzCanvas::Getproperty ( ) {
    struct IWzProperty * _result = 0;
    HRESULT _hr = get_property(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzPropertyPtr(_result, false);
}

inline int IWzCanvas::Getcx ( ) {
    int _result = 0;
    HRESULT _hr = get_cx(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::Putcx ( int pnX ) {
    HRESULT _hr = put_cx(pnX);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzCanvas::Getcy ( ) {
    int _result = 0;
    HRESULT _hr = get_cy(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzCanvas::Putcy ( int pnY ) {
    HRESULT _hr = put_cy(pnY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_variant_t IWzCanvas::SetClipRect ( int nLeft, int nTop, int nWidth, int nHeight, const Ztl_variant_t & bTemp ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_SetClipRect(nLeft, nTop, nWidth, nHeight, bTemp, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline HRESULT IWzCanvas::Copy ( int nDstLeft, int nDstTop, struct IWzCanvas * pSource, const Ztl_variant_t & nAlpha ) {
    HRESULT _hr = raw_Copy(nDstLeft, nDstTop, pSource, nAlpha);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzCanvas::CopyEx ( int nDstLeft, int nDstTop, struct IWzCanvas * pSource, enum CANVAS_ALPHATYPE nAlpha, int nWidth, int nHeight, int nSrcLeft, int nSrcTop, int nSrcWidth, int nSrcHeight, const Ztl_variant_t & pAdjust ) {
    HRESULT _hr = raw_CopyEx(nDstLeft, nDstTop, pSource, nAlpha, nWidth, nHeight, nSrcLeft, nSrcTop, nSrcWidth, nSrcHeight, pAdjust);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline unsigned int IWzCanvas::Getpixel ( int nX, int nY ) {
    unsigned int _result = 0;
    HRESULT _hr = get_pixel(nX, nY, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IWzCanvas::DrawRectangle ( int nLeft, int nTop, unsigned int uWidth, unsigned int uHeight, unsigned int uColor ) {
    HRESULT _hr = raw_DrawRectangle(nLeft, nTop, uWidth, uHeight, uColor);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzCanvas::DrawLine ( int x1, int y1, int x2, int y2, unsigned int uColor, const Ztl_variant_t & nWidth ) {
    HRESULT _hr = raw_DrawLine(x1, y1, x2, y2, uColor, nWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzCanvas::DrawPolygon ( ) {
    HRESULT _hr = raw_DrawPolygon();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline unsigned int IWzCanvas::DrawTextA ( int nLeft, int nTop, Ztl_bstr_t sText, struct IWzFont * pFont, const Ztl_variant_t & vAlpha, const Ztl_variant_t & vTabOrg ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_DrawTextA(nLeft, nTop, sText, pFont, vAlpha, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IWzRawCanvasAllocator::AllocCanvas ( struct IWzCanvas * pCanvas, enum CANVAS_PIXFORMAT nPixFormat, int nMagLevel ) {
    HRESULT _hr = raw_AllocCanvas(pCanvas, nPixFormat, nMagLevel);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzRawCanvasAllocator::ConvertIfNotAvailable ( struct IWzCanvas * pCanvas, int * pnNewPixFormat ) {
    HRESULT _hr = raw_ConvertIfNotAvailable(pCanvas, pnNewPixFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzFont::Create ( Ztl_bstr_t sName, unsigned int uHeight, unsigned int uColor, const Ztl_variant_t & sStyle ) {
    HRESULT _hr = raw_Create(sName, uHeight, uColor, sStyle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzFont::Getcolor ( ) {
    int _result = 0;
    HRESULT _hr = get_color(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::Getheight ( ) {
    int _result = 0;
    HRESULT _hr = get_height(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::GetfullHeight ( ) {
    int _result = 0;
    HRESULT _hr = get_fullHeight(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzFont::CalcTextWidth ( Ztl_bstr_t sText, const Ztl_variant_t & vTabOrg ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_CalcTextWidth(sText, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::CalcLongestText ( Ztl_bstr_t sText, int nWidth, const Ztl_variant_t & vTabOrg ) {
    int _result = 0;
    HRESULT _hr = raw_CalcLongestText(sText, nWidth, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::CalcLongestTextForGlobal ( Ztl_bstr_t sText, int nWidth, const Ztl_variant_t & vTabOrg ) {
    int _result = 0;
    HRESULT _hr = raw_CalcLongestTextForGlobal(sText, nWidth, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::CalcLongestTextForGlobalEx ( Ztl_bstr_t sText, int nWidth, int * pnPosAdj, const Ztl_variant_t & vTabOrg ) {
    int _result = 0;
    HRESULT _hr = raw_CalcLongestTextForGlobalEx(sText, nWidth, pnPosAdj, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzFont::CalcLineCountForGlobal ( Ztl_bstr_t sText, int nWidth, const Ztl_variant_t & vTabOrg ) {
    int _result = 0;
    HRESULT _hr = raw_CalcLineCountForGlobal(sText, nWidth, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzFont::DrawTextA ( int nLeft, int nTop, Ztl_bstr_t sText, struct IWzCanvas * pCanvas, const Ztl_variant_t & vAlpha, const Ztl_variant_t & vTabOrg ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_DrawTextA(nLeft, nTop, sText, pCanvas, vAlpha, vTabOrg, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}


#pragma pack(pop)
