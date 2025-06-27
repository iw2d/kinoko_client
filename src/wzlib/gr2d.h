#pragma once
#include "shape2d.h"
#include "canvas.h"
#include <comdef.h>
#include <cstdint>


enum LAYER_BLENDTYPE : int32_t {
    LB_NORMAL = 0x0,
    LB_ADD = 0x1,
    LB_INVERSE = 0x2,
    LB_ISOLATED = 0x4,
};

enum GR2D_ANITYPE : int32_t {
    GA_STOP = 0x0,
    GA_WAIT = 0x100,
    GA_NORMAL = 0x0,
    GA_FIRST = 0x10,
    GA_REPEAT = 0x20,
};


DECLARE_INTERFACE_IID_(IWzGr2DLayer, IWzVector2D, "6dc8c7ce-8e81-4420-b4f6-4b60b7d5fcdf") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzShape2D methods ***/
    STDMETHOD(get_item)(THIS_ tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(get__NewEnum)(THIS_ IUnknown**) PURE;
    STDMETHOD(get_count)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_x)(THIS_ int32_t*) PURE;
    STDMETHOD(put_x)(THIS_ int32_t) PURE;
    STDMETHOD(get_y)(THIS_ int32_t*) PURE;
    STDMETHOD(put_y)(THIS_ int32_t) PURE;
    STDMETHOD(get_x2)(THIS_ int32_t*) PURE;
    STDMETHOD(put_x2)(THIS_ int32_t) PURE;
    STDMETHOD(get_y2)(THIS_ int32_t*) PURE;
    STDMETHOD(put_y2)(THIS_ int32_t) PURE;
    STDMETHOD(raw_Move)(THIS_ int32_t nX, int32_t nY) PURE;
    STDMETHOD(raw_Offset)(THIS_ int32_t nDX, int32_t nDY) PURE;
    STDMETHOD(raw_Scale)(THIS_ int32_t nXMul, int32_t nXDiv, int32_t nYMul, int32_t nYDiv, int32_t nXOrg, int32_t nYOrg) PURE;
    STDMETHOD(raw_Insert)(THIS_ tagVARIANT vIndexOrShape, tagVARIANT vShape) PURE;
    STDMETHOD(raw_Remove)(THIS_ tagVARIANT vIndex, tagVARIANT* pvRemoved) PURE;
    STDMETHOD(raw_Init)(THIS_ int32_t nX, int32_t nY) PURE;

    /*** IWzVector2D methods ***/
    STDMETHOD(get_currentTime)(THIS_ int32_t* pnTime) PURE;
    STDMETHOD(put_currentTime)(THIS_ int32_t nTime) PURE;
    STDMETHOD(get_origin)(THIS_ tagVARIANT* ppOrigin) PURE;
    STDMETHOD(put_origin)(THIS_ tagVARIANT pOrigin) PURE;
    STDMETHOD(get_rx)(THIS_ int32_t* pnX) PURE;
    STDMETHOD(put_rx)(THIS_ int32_t nX) PURE;
    STDMETHOD(get_ry)(THIS_ int32_t* pnY) PURE;
    STDMETHOD(put_ry)(THIS_ int32_t nY) PURE;
    STDMETHOD(get_a)(THIS_ double* pa) PURE;
    STDMETHOD(get_ra)(THIS_ double* pra) PURE;
    STDMETHOD(put_ra)(THIS_ double ra) PURE;
    STDMETHOD(get_flipX)(THIS_ int32_t* pnFlipX) PURE;
    STDMETHOD(put_flipX)(THIS_ int32_t nFlipX) PURE;
    STDMETHOD(raw__GetSnapshot)(THIS_ int32_t* px, int32_t* py, int32_t* prx, int32_t* pry, int32_t* pxOrg, int32_t* pyOrg, double* pa, double* paOrg, tagVARIANT vTime) PURE;
    STDMETHOD(raw_RelMove)(THIS_ int32_t nX, int32_t nY, tagVARIANT nTime, tagVARIANT nType) PURE;
    STDMETHOD(raw_RelOffset)(THIS_ int32_t nDX, int32_t nDY, tagVARIANT nTime, tagVARIANT nType) PURE;
    STDMETHOD(raw_Ratio)(THIS_ IWzVector2D* pOrigin, int32_t nOriginWidth, int32_t nOriginHeight, int32_t nScaleWidth, int32_t nScaleHeight) PURE;
    STDMETHOD(raw_WrapClip)(THIS_ tagVARIANT pOrigin, int32_t nWrapLeft, int32_t nWrapTop, uint32_t uWrapWidth, uint32_t uWrapHeight, tagVARIANT bClip) PURE;
    STDMETHOD(raw_Rotate)(THIS_ double aAmount, tagVARIANT nTime) PURE;
    STDMETHOD(get_looseLevel)(THIS_ uint32_t* puLevel) PURE;
    STDMETHOD(put_looseLevel)(THIS_ uint32_t uLevel) PURE;
    STDMETHOD(raw_Fly)(THIS_ tagVARIANT* aVar, int32_t nArgs) PURE;

    /*** IWzGr2DLayer methods ***/
    STDMETHOD(get_z)(THIS_ int32_t* pnZ) PURE;
    STDMETHOD(put_z)(THIS_ int32_t nZ) PURE;
    STDMETHOD(get_width)(THIS_ int32_t* pnWidth) PURE;
    STDMETHOD(put_width)(THIS_ int32_t nWidth) PURE;
    STDMETHOD(get_height)(THIS_ int32_t* pnHeight) PURE;
    STDMETHOD(put_height)(THIS_ int32_t nHeight) PURE;
    STDMETHOD(get_lt)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_rb)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(raw_InterLockedOffset)(THIS_ int32_t, int32_t, int32_t, int32_t) PURE;
    STDMETHOD(get_flip)(THIS_ int32_t* pnFlip) PURE;
    STDMETHOD(put_flip)(THIS_ int32_t nFlip) PURE;
    STDMETHOD(get_color)(THIS_ uint32_t* puColor) PURE;
    STDMETHOD(put_color)(THIS_ uint32_t uColor) PURE;
    STDMETHOD(get_alpha)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_redTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_greenBlueTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_blend)(THIS_ LAYER_BLENDTYPE*) PURE;
    STDMETHOD(put_blend)(THIS_ LAYER_BLENDTYPE) PURE;
    STDMETHOD(get_overlay)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(put_overlay)(THIS_ tagVARIANT) PURE;
    STDMETHOD(get_canvas)(THIS_ tagVARIANT, IWzCanvas**) PURE;
    STDMETHOD(raw_InsertCanvas)(THIS_ IWzCanvas* pCanvas, tagVARIANT vDelay, tagVARIANT vAlpha0, tagVARIANT vAlpha1, tagVARIANT vZoom0, tagVARIANT vZoom1, tagVARIANT*) PURE;
    STDMETHOD(raw_RemoveCanvas)(THIS_ tagVARIANT vIndex, IWzCanvas**) PURE;
    STDMETHOD(raw_ShiftCanvas)(THIS_ tagVARIANT vValue) PURE;
    STDMETHOD(raw_Animate)(THIS_ GR2D_ANITYPE nType, tagVARIANT vDelayRate, tagVARIANT vRepeat) PURE;
    STDMETHOD(get_animationState)(THIS_ int32_t*) PURE;
    STDMETHOD(get_visible)(THIS_ int32_t* pnVisible) PURE;
    STDMETHOD(put_visible)(THIS_ int32_t nVisible) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzGr2DLayer, __uuidof(IWzGr2DLayer));


DECLARE_INTERFACE_IID_(IWzGr2D, IUnknown, "e576ea33-d465-4f08-aab1-e78df73ee6d9") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzGr2D methods ***/
    STDMETHOD(raw_Initialize)(THIS_ uint32_t uWidth, uint32_t uHeight, tagVARIANT vHwnd, tagVARIANT vBPP, tagVARIANT vRefreshRate) PURE;
    STDMETHOD(raw_Uninitialize)(THIS) PURE;
    STDMETHOD(get_nextRenderTime)(THIS_ int32_t*) PURE;
    STDMETHOD(raw_UpdateCurrentTime)(THIS_ int32_t tTime) PURE;
    STDMETHOD(raw_RenderFrame)(THIS) PURE;
    STDMETHOD(raw_SetFrameSkip)(THIS) PURE;
    STDMETHOD(raw_ToggleFpsPanel)(THIS) PURE;
    STDMETHOD(raw_DisableFpsPanel)(THIS) PURE;
    STDMETHOD(get_width)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_height)(THIS_ uint32_t*) PURE;
    STDMETHOD(put_screenResolution)(THIS_ uint32_t uWidth, uint32_t uHeight) PURE;
    STDMETHOD(get_bpp)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_refreshRate)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_fps100)(THIS_ uint32_t*) PURE;
    STDMETHOD(get_currentTime)(THIS_ int32_t*) PURE;
    STDMETHOD(get_fullScreen)(THIS_ int32_t*) PURE;
    STDMETHOD(put_fullScreen)(THIS_ int32_t) PURE;
    STDMETHOD(get_backColor)(THIS_ uint32_t*) PURE;
    STDMETHOD(put_backColor)(THIS_ uint32_t) PURE;
    STDMETHOD(get_redTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_greenBlueTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_center)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(raw_GetSnapshot)(THIS_ tagVARIANT, uint32_t, int32_t, int32_t) PURE;
    STDMETHOD(raw_CreateLayer)(THIS_ int32_t nLeft, int32_t nTop, uint32_t uWidth, uint32_t uHeight, int32_t nZ, tagVARIANT vCanvas, tagVARIANT dwFilter, IWzGr2DLayer**) PURE;
    STDMETHOD(raw_AdjustCenter)(THIS_ int32_t nAdjustCenterX, int32_t nAdjustCenterY) PURE;
    STDMETHOD(raw_TakeScreenShot)(THIS_ wchar_t*, int32_t) PURE;
    STDMETHOD(raw_SetVideoMode)(THIS_ int32_t bMode) PURE;
    STDMETHOD(raw_SetVideoPath)(THIS_ wchar_t*, int32_t, int32_t) PURE;
    STDMETHOD(raw_PlayVideo)(THIS) PURE;
    STDMETHOD(raw_PauseVideo)(THIS_ int32_t) PURE;
    STDMETHOD(raw_StopVideo)(THIS) PURE;
    STDMETHOD(get_videoStatus)(THIS_ int32_t*) PURE;
    STDMETHOD(put_videoVolume)(THIS_ int32_t) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzGr2D, __uuidof(IWzGr2D));