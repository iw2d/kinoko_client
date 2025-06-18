#pragma once
#include "serialize.h"
#include "property.h"
#include <comdef.h>
#include <cstdint>


enum CANVAS_ALPHATYPE : int32_t {
    CA_OVERWRITE = 0xFF,
    CA_REMOVEALPHA = 0xFE,
};

enum CANVAS_LEVELMAP : int32_t {
    CL_ALL16 = 0x1,
    CL_ALL32 = 0x2,
    CL_ALL56 = 0x201,
    CL_USE32OVER1 = 0x10002,
    CL_USE32OVER2 = 0x20002,
    CL_USE56OVER1 = 0x10201,
    CL_USE56OVER2 = 0x20201,
};

enum CANVAS_PIXFORMAT : int32_t {
    CP_UNKNOWN = 0x0,
    CP_A4R4G4B4 = 0x1,
    CP_A8R8G8B8 = 0x2,
    CP_R5G6B5 = 0x201,
    CP_DXT3 = 0x402,
};

enum CANVAS_DITHERTYPE : int32_t {
    CD_NONE = 0x0,
    CD_FS0 = 0x1,
    CD_FS1 = 0x2,
    CD_FS = 0x3,
    CD_FD0 = 0x11,
    CD_FD1 = 0x12,
    CD_FD = 0x13,
};


struct IWzCanvas;

DECLARE_INTERFACE_IID_(IWzRawCanvasAllocator, IUnknown, "58003396-b7c8-4ef4-a863-7622b90b956c") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzRawCanvasAllocator methods ***/
    STDMETHOD(raw_AllocCanvas)(THIS_ IWzCanvas*, CANVAS_PIXFORMAT, int32_t) PURE;
    STDMETHOD(raw_ConvertIfNotAvailable)(THIS_ IWzCanvas*, int32_t*) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzRawCanvasAllocator, __uuidof(IWzRawCanvasAllocator));


DECLARE_INTERFACE_IID_(IWzRawCanvas, IUnknown, "312126f0-c399-4111-8eab-0f96a30b6b7c") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzRawCanvas methods ***/
    STDMETHOD(get_pixelFormat)(THIS_ CANVAS_PIXFORMAT*) PURE;
    STDMETHOD(get_magLevel)(THIS_ int32_t*) PURE;
    STDMETHOD(get_width)(THIS_ uint32_t* puWidth) PURE;
    STDMETHOD(get_height)(THIS_ uint32_t* puHeight) PURE;
    STDMETHOD(raw__LockAddress)(THIS_ int32_t* pnPitch, tagVARIANT*) PURE;
    STDMETHOD(raw__UnlockAddress)(THIS_ tagRECT*) PURE;
    STDMETHOD(raw_SetTexture)(THIS_ uint32_t*) PURE;
    STDMETHOD(raw_GetTextureSize)(THIS_ tagRECT*) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzRawCanvas, __uuidof(IWzRawCanvas));


DECLARE_INTERFACE_IID_(IWzFont, IUnknown, "2bef046d-ccd6-445a-88c4-929fc35d30ac") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzFont methods ***/
    STDMETHOD(raw_Create)(THIS_ wchar_t* sName, uint32_t uHeight, uint32_t uColor, tagVARIANT sStyle) PURE;
    STDMETHOD(get_color)(THIS_ int32_t*) PURE;
    STDMETHOD(get_height)(THIS_ int32_t*) PURE;
    STDMETHOD(get_fullHeight)(THIS_ int32_t*) PURE;
    STDMETHOD(raw_CalcTextWidth)(THIS_ wchar_t* sText, tagVARIANT vTabOrg, uint32_t*) PURE;
    STDMETHOD(raw_CalcLongestText)(THIS_ wchar_t* sText, int32_t nWidth, tagVARIANT vTabOrg, int32_t*) PURE;
    STDMETHOD(raw_CalcLongestTextForGlobal)(THIS_ wchar_t* sText, int32_t nWidth, tagVARIANT vTabOrg, int32_t*) PURE;
    STDMETHOD(raw_CalcLongestTextForGlobalEx)(THIS_ wchar_t* sText, int32_t nWidth, int32_t* pnPosAdj, tagVARIANT vTabOrg, int32_t*) PURE;
    STDMETHOD(raw_CalcLineCountForGlobal)(THIS_ wchar_t* sText, int32_t nWidth, tagVARIANT vTabOrg, int32_t*) PURE;
    STDMETHOD(raw_DrawText)(THIS_ int32_t nLeft, int32_t nTop, wchar_t* sText, IWzCanvas* pCanvas, tagVARIANT vAlpha, tagVARIANT vTabOrg, uint32_t* puHeight) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzFont, __uuidof(IWzFont));


DECLARE_INTERFACE_IID_(IWzCanvas, IWzSerialize, "7600dc6c-9328-4bff-9624-5b0f5c01179e") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzCanvas methods ***/
    STDMETHOD(get_defaultDither)(THIS_ CANVAS_DITHERTYPE*) PURE;
    STDMETHOD(put_defaultDither)(THIS_ CANVAS_DITHERTYPE) PURE;
    STDMETHOD(get_defaultLevelMap)(THIS_ CANVAS_LEVELMAP*) PURE;
    STDMETHOD(put_defaultLevelMap)(THIS_ CANVAS_LEVELMAP) PURE;
    STDMETHOD(get_defaultAllocator)(THIS_ IWzRawCanvasAllocator**) PURE;
    STDMETHOD(put_defaultAllocator)(THIS_ IWzRawCanvasAllocator*) PURE;
    STDMETHOD(raw_Create)(THIS_ uint32_t uWidth, uint32_t uHeight, tagVARIANT nMagLevel, tagVARIANT nPixFormat) PURE;
    STDMETHOD(raw_AddRawCanvas)(THIS_ int32_t, int32_t, IWzRawCanvas*) PURE;
    STDMETHOD(get_rawCanvas)(THIS_ int32_t, int32_t, IWzRawCanvas**) PURE;
    STDMETHOD(get_tileWidth)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_tileHeight)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_width)(THIS_ uint32_t*) PURE;
    STDMETHOD(put_width)(THIS_ uint32_t) PURE;
    STDMETHOD(get_height)(THIS_ uint32_t*) PURE;
    STDMETHOD(put_height)(THIS_ uint32_t) PURE;
    STDMETHOD(get_pixelFormat)(THIS_ CANVAS_PIXFORMAT*) PURE;
    STDMETHOD(put_pixelFormat)(THIS_ CANVAS_PIXFORMAT) PURE;
    STDMETHOD(get_magLevel)(THIS_ int32_t*) PURE;
    STDMETHOD(put_magLevel)(THIS_ int32_t) PURE;
    STDMETHOD(raw_GetSnapshotU)(THIS_ uint32_t*, uint32_t*, uint32_t*, uint32_t*, CANVAS_PIXFORMAT*, int32_t*) PURE;
    STDMETHOD(raw_GetSnapshot)(THIS_ int32_t*, int32_t*, int32_t*, int32_t*, CANVAS_PIXFORMAT*, int32_t*) PURE;
    STDMETHOD(get_property)(THIS_ IWzProperty**) PURE;
    STDMETHOD(get_cx)(THIS_ int32_t*) PURE;
    STDMETHOD(put_cx)(THIS_ int32_t) PURE;
    STDMETHOD(get_cy)(THIS_ int32_t*) PURE;
    STDMETHOD(put_cy)(THIS_ int32_t) PURE;
    STDMETHOD(raw_SetClipRect)(THIS_ int32_t, int32_t, int32_t, int32_t, tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(raw_Copy)(THIS_ int32_t, int32_t, IWzCanvas*, tagVARIANT) PURE;
    STDMETHOD(raw_CopyEx)(THIS_ int32_t, int32_t, IWzCanvas*, CANVAS_ALPHATYPE, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, tagVARIANT) PURE;
    STDMETHOD(get_pixel)(THIS_ int32_t, int32_t, uint32_t*) PURE;
    STDMETHOD(raw_DrawRectangle)(THIS_ int32_t nLeft, int32_t nTop, uint32_t uWidth, uint32_t uHeight, uint32_t uColor) PURE;
    STDMETHOD(raw_DrawLine)(THIS_ int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t uColor, tagVARIANT nWidth) PURE;
    STDMETHOD(raw_DrawPolygon)(THIS) PURE;
    STDMETHOD(raw_DrawText)(THIS_ int32_t nLeft, int32_t nTop, wchar_t* sText, IWzFont* pFont, tagVARIANT vAlpha, tagVARIANT vTabOrg, uint32_t*) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzCanvas, __uuidof(IWzCanvas));