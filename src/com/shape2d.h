#pragma once
#include <comdef.h>
#include "serialize.h"


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
    STDMETHOD(put_ra)(THIS_ long double ra) PURE;
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

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzVector2D, __uuidof(IWzVector2D));