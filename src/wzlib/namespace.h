#pragma once
#include <comdef.h>
#include <cstdint>


DECLARE_INTERFACE_IID_(IWzNameSpaceProperty, IUnknown, "a0b5164f-f89b-41b8-8264-15371fd02b4c") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzNameSpaceProperty methods ***/
    STDMETHOD(get_item)(THIS_ tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(get_name)(THIS_ wchar_t**) PURE;
    STDMETHOD(get_path)(THIS_ wchar_t**) PURE;
    STDMETHOD(get_size)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(get_checksum)(THIS_ int32_t*) PURE;
    STDMETHOD(get_property)(THIS_ wchar_t*, tagVARIANT*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzNameSpaceProperty, __uuidof(IWzNameSpaceProperty));


DECLARE_INTERFACE_IID_(IWzNameSpace, IUnknown, "2aeeeb36-a4e1-4e2b-8f6f-2e7bdec5c53d") {
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

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzNameSpace, __uuidof(IWzNameSpace));


DECLARE_INTERFACE_(IWzWritableNameSpace, IWzNameSpace) {
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

    END_INTERFACE;
};