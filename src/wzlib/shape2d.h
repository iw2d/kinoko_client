#pragma once
#include "serialize.h"
#include <comdef.h>
#include <cstdint>


DECLARE_INTERFACE_IID_(IWzShape2D, IWzSerialize, "4cfb57c7-eae3-40b3-ac98-4b2750e3642a") {
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

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzShape2D, __uuidof(IWzShape2D));


DECLARE_INTERFACE_IID_(IWzVector2D, IWzShape2D, "f28bd1ed-3deb-4f92-9eec-10ef5a1c3fb4") {
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

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzVector2D, __uuidof(IWzVector2D));