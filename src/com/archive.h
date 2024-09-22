#pragma once
#include <comdef.h>


DECLARE_INTERFACE_IID_(IWzArchive, IUnknown, "3c39b9aa-18cc-408c-8716-c4fb2117266e") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzArchive methods ***/
    STDMETHOD(get_loading)(THIS_ int*) PURE;
    STDMETHOD(raw_Read)(THIS_ unsigned char*, unsigned int, unsigned int*) PURE;
    STDMETHOD(raw_Write)(THIS_ unsigned char*, unsigned int, unsigned int*) PURE;
    STDMETHOD(get_absoluteUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(put_absoluteUOL)(THIS_ wchar_t*) PURE;
    STDMETHOD(get_position)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_context)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(put_context)(THIS_ tagVARIANT*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzArchive, __uuidof(IWzArchive));


DECLARE_INTERFACE_IID_(IWzSeekableArchive, IWzArchive, "35c1f133-7f61-496e-878f-9a1758afa9ea") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzArchive methods ***/
    STDMETHOD(get_loading)(THIS_ int*) PURE;
    STDMETHOD(raw_Read)(THIS_ unsigned char*, unsigned int, unsigned int*) PURE;
    STDMETHOD(raw_Write)(THIS_ unsigned char*, unsigned int, unsigned int*) PURE;
    STDMETHOD(get_absoluteUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(put_absoluteUOL)(THIS_ wchar_t*) PURE;
    STDMETHOD(get_position)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_context)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(put_context)(THIS_ tagVARIANT*) PURE;

    /*** IWzSeekableArchive methods ***/
    STDMETHOD(put_position)(THIS_ unsigned int) PURE;
    STDMETHOD(get_length)(THIS_ unsigned int*) PURE;
    STDMETHOD(put_length)(THIS_ unsigned int) PURE;
    STDMETHOD(get_address)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(raw_Rewind)(THIS) PURE;
    STDMETHOD(raw_ReadLock)(THIS_ wchar_t*, unsigned int, IWzSeekableArchive**) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzSeekableArchive, __uuidof(IWzSeekableArchive));