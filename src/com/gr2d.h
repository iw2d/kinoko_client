#pragma once
#include <comdef.h>
#include "shape2d.h"
#include "canvas.h"


enum LAYER_BLENDTYPE {
    LB_NORMAL = 0x0,
    LB_ADD = 0x1,
    LB_INVERSE = 0x2,
    LB_ISOLATED = 0x4,
};

enum GR2D_ANITYPE {
    GA_STOP = 0x0,
    GA_WAIT = 0x100,
    GA_NORMAL = 0x0,
    GA_FIRST = 0x10,
    GA_REPEAT = 0x20,
};


DECLARE_INTERFACE_IID_(IWzGr2DLayer, IWzShape2D, "6dc8c7ce-8e81-4420-b4f6-4b60b7d5fcdf") {
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
    STDMETHOD(get__NewEnum)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_count)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_x)(THIS_ int*) PURE;
    STDMETHOD(put_x)(THIS_ int) PURE;
    STDMETHOD(get_y)(THIS_ int*) PURE;
    STDMETHOD(put_y)(THIS_ int) PURE;
    STDMETHOD(get_x2)(THIS_ int*) PURE;
    STDMETHOD(put_x2)(THIS_ int) PURE;
    STDMETHOD(get_y2)(THIS_ int*) PURE;
    STDMETHOD(put_y2)(THIS_ int) PURE;
    STDMETHOD(raw_Move)(THIS_ int nX, int nY) PURE;
    STDMETHOD(raw_Offset)(THIS_ int nDX, int nDY) PURE;
    STDMETHOD(raw_Scale)(THIS_ int nXMul, int nXDiv, int nYMul, int nYDiv, int nXOrg, int nYOrg) PURE;
    STDMETHOD(raw_Insert)(THIS_ tagVARIANT vIndexOrShape, tagVARIANT vShape) PURE;
    STDMETHOD(raw_Remove)(THIS_ tagVARIANT vIndex, tagVARIANT* pvRemoved) PURE;
    STDMETHOD(raw_Init)(THIS_ int nX, int nY) PURE;

    /*** IWzVector2D methods ***/
    STDMETHOD(get_currentTime)(THIS_ int* pnTime) PURE;
    STDMETHOD(put_currentTime)(THIS_ int nTime) PURE;
    STDMETHOD(get_origin)(THIS_ tagVARIANT* ppOrigin) PURE;
    STDMETHOD(put_origin)(THIS_ tagVARIANT* pOrigin) PURE;
    STDMETHOD(get_rx)(THIS_ int* pnX) PURE;
    STDMETHOD(put_rx)(THIS_ int nX) PURE;
    STDMETHOD(get_ry)(THIS_ int* pnY) PURE;
    STDMETHOD(put_ry)(THIS_ int nY) PURE;
    STDMETHOD(get_a)(THIS_ long double* pa) PURE;
    STDMETHOD(get_ra)(THIS_ long double* pra) PURE;
    STDMETHOD(put_rx)(THIS_ long double ra) PURE;
    STDMETHOD(get_flipX)(THIS_ int* pnFlipX) PURE;
    STDMETHOD(put_flipX)(THIS_ int nFlipX) PURE;
    STDMETHOD(raw__GetSnapshot)(THIS_ int* px, int* py, int* prx, int* pry, int* pxOrg, int* pyOrg, long double* pa, long double *paOrg, tagVARIANT vTime) PURE;
    STDMETHOD(raw_RelMove)(THIS_ int nX, int nY, tagVARIANT nTime, tagVARIANT nType) PURE;
    STDMETHOD(raw_RelOffset)(THIS_ int nDX, int nDY, tagVARIANT nTime, tagVARIANT nType) PURE;
    STDMETHOD(raw_Ratio)(THIS_ IWzVector2D* pOrigin, int nOriginWidth, int nOriginHeight, int nScaleWidth, int nScaleHeight) PURE;
    STDMETHOD(raw_WrapClip)(THIS_ tagVARIANT pOrigin, int nWrapLeft, int nWrapTop, unsigned int uWrapWidth, unsigned int uWrapHeight, tagVARIANT bClip) PURE;
    STDMETHOD(raw_Rotate)(THIS_ long double aAmount, tagVARIANT nTime) PURE;
    STDMETHOD(get_looseLevel)(THIS_ unsigned int* puLevel) PURE;
    STDMETHOD(put_looseLevel)(THIS_ unsigned int uLevel) PURE;
    STDMETHOD(raw_Fly)(THIS_ tagVARIANT* aVar, int nArgs) PURE;

    /*** IWzGr2DLayer methods ***/
    STDMETHOD(get_z)(THIS_ int* pnZ) PURE;
    STDMETHOD(put_z)(THIS_ int nZ) PURE;
    STDMETHOD(get_width)(THIS_ int* pnWidth) PURE;
    STDMETHOD(put_width)(THIS_ int nWidth) PURE;
    STDMETHOD(get_height)(THIS_ int* pnHeight) PURE;
    STDMETHOD(put_height)(THIS_ int nHeight) PURE;
    STDMETHOD(get_lt)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_rb)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(raw_InterLockedOffset)(THIS_ int, int, int, int) PURE;
    STDMETHOD(get_flip)(THIS_ int* pnFlip) PURE;
    STDMETHOD(put_flip)(THIS_ int nFlip) PURE;
    STDMETHOD(get_color)(THIS_ unsigned int* puColor) PURE;
    STDMETHOD(put_color)(THIS_ unsigned int uColor) PURE;
    STDMETHOD(get_alpha)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_redTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_greenBlueTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_blend)(THIS_ LAYER_BLENDTYPE*) PURE;
    STDMETHOD(put_blend)(THIS_ LAYER_BLENDTYPE) PURE;
    STDMETHOD(get_overlay)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(put_overlay)(THIS_ tagVARIANT) PURE;
    STDMETHOD(get_canvas)(THIS_ IWzCanvas**) PURE;
    STDMETHOD(raw_InsertCanvas)(THIS_ IWzCanvas* pCanvas, tagVARIANT vDelay, tagVARIANT vAlpha0, tagVARIANT vAlpha1, tagVARIANT vZoom0, tagVARIANT vZoom1, tagVARIANT*) PURE;
    STDMETHOD(raw_RemoveCanvas)(THIS_ tagVARIANT vIndex, IWzCanvas**) PURE;
    STDMETHOD(raw_ShiftCanvas)(THIS_ tagVARIANT vValue) PURE;
    STDMETHOD(raw_Animate)(THIS_ GR2D_ANITYPE nType, tagVARIANT vDelayRate, tagVARIANT vRepeat) PURE;
    STDMETHOD(get_animationState)(THIS_ int*) PURE;
    STDMETHOD(get_visible)(THIS_ int* pnVisible) PURE;
    STDMETHOD(put_visible)(THIS_ int nVisible) PURE;

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
    STDMETHOD(raw_Initialize)(THIS_ unsigned int uWidth, unsigned int uHeight, tagVARIANT vHwnd, tagVARIANT vBPP, tagVARIANT vRefreshRate) PURE;
    STDMETHOD(raw_Uninitialize)(THIS) PURE;
    STDMETHOD(get_nextRenderTime)(THIS_ int*) PURE;
    STDMETHOD(raw_UpdateCurrentTime)(THIS_ int tTime) PURE;
    STDMETHOD(raw_RenderFrame)(THIS) PURE;
    STDMETHOD(raw_SetFrameSkip)(THIS) PURE;
    STDMETHOD(raw_ToggleFpsPanel)(THIS) PURE;
    STDMETHOD(raw_DisableFpsPanel)(THIS) PURE;
    STDMETHOD(get_width)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_height)(THIS_ unsigned int*) PURE;
    STDMETHOD(put_screenResolution)(THIS_ unsigned int uWidth, unsigned int uHeight) PURE;
    STDMETHOD(get_bpp)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_refreshRate)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_fps100)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_currentTime)(THIS_ int*) PURE;
    STDMETHOD(get_fullScreen)(THIS_ int*) PURE;
    STDMETHOD(put_fullScreen)(THIS_ int) PURE;
    STDMETHOD(get_backColor)(THIS_ unsigned int*) PURE;
    STDMETHOD(put_backColor)(THIS_ unsigned int) PURE;
    STDMETHOD(get_redTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_greenBlueTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_center)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(raw_GetSnapshot)(THIS_ tagVARIANT, unsigned int, int, int) PURE;
    STDMETHOD(raw_CreateLayer)(THIS_ int nLeft, int nTop, unsigned int uWidth, unsigned int uHeight, int nZ, tagVARIANT vCanvas, tagVARIANT dwFilter, IWzGr2DLayer**) PURE;
    STDMETHOD(raw_AdjustCenter)(THIS_ int nAdjustCenterX, int nAdjustCenterY) PURE;
    STDMETHOD(raw_TakeScreenShot)(THIS_ wchar_t*, int) PURE;
    STDMETHOD(raw_SetVideoMode)(THIS_ int bMode) PURE;
    STDMETHOD(raw_SetVideoPath)(THIS_ wchar_t*, int, int) PURE;
    STDMETHOD(raw_PlayVideo)(THIS) PURE;
    STDMETHOD(raw_PauseVideo)(THIS_ int) PURE;
    STDMETHOD(raw_StopVideo)(THIS) PURE;
    STDMETHOD(get_videoStatus)(THIS_ int*) PURE;
    STDMETHOD(put_videoVolume)(THIS_ int) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzGr2D, __uuidof(IWzGr2D));