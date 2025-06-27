#pragma once
#include "archive.h"
#include "namespace.h"
#include <comdef.h>
#include <cstdint>


DECLARE_INTERFACE_IID_(IWzPackage, IWzNameSpace, "e610818b-038d-4522-9232-30fcd5f4737c") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzNameSpace methods ***/
    STDMETHOD(get_item)(THIS_ wchar_t*, tagVARIANT*) PURE;
    STDMETHOD(get_property)(THIS_ wchar_t*, tagVARIANT, IWzNameSpaceProperty**) PURE;
    STDMETHOD(get__NewEnum)(THIS_ IUnknown**) PURE;
    STDMETHOD(raw_Mount)(THIS_ wchar_t*, IWzNameSpace*, int32_t) PURE;
    STDMETHOD(raw_Unmount)(THIS_ wchar_t*, tagVARIANT) PURE;
    STDMETHOD(raw__OnMountEvent)(THIS_ IWzNameSpace*, IWzNameSpace*, wchar_t*, int32_t) PURE;
    STDMETHOD(raw__OnGetLocalObject)(THIS_ int32_t, wchar_t*, int32_t*, tagVARIANT*) PURE;

    /*** IWzPackage methods ***/
    STDMETHOD(raw_Init)(THIS_ wchar_t*, wchar_t*, IWzSeekableArchive*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzPackage, __uuidof(IWzPackage));


DECLARE_INTERFACE_IID_(IWzFileSystem, IWzWritableNameSpace, "352d8655-51e4-4668-8ce4-0866e2b6a5b5") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzNameSpace methods ***/
    STDMETHOD(get_item)(THIS_ wchar_t*, tagVARIANT*) PURE;
    STDMETHOD(get_property)(THIS_ wchar_t*, tagVARIANT, IWzNameSpaceProperty**) PURE;
    STDMETHOD(get__NewEnum)(THIS_ IUnknown**) PURE;
    STDMETHOD(raw_Mount)(THIS_ wchar_t*, IWzNameSpace*, int32_t) PURE;
    STDMETHOD(raw_Unmount)(THIS_ wchar_t*, tagVARIANT) PURE;
    STDMETHOD(raw__OnMountEvent)(THIS_ IWzNameSpace*, IWzNameSpace*, wchar_t*, int32_t) PURE;
    STDMETHOD(raw__OnGetLocalObject)(THIS_ int32_t, wchar_t*, int32_t*, tagVARIANT*) PURE;

    /*** IWzWritableNameSpace methods ***/
    STDMETHOD(raw_CreateChildNameSpace)(THIS_ wchar_t*, IWzNameSpace**) PURE;
    STDMETHOD(raw_AddObject)(THIS_ wchar_t*, tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(raw_Unlink)(THIS_ wchar_t*) PURE;

    /*** IWzFileSystem methods ***/
    STDMETHOD(raw_Init)(THIS_ wchar_t*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzFileSystem, __uuidof(IWzFileSystem));