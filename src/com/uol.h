#pragma once
#include <comdef.h>
#include "serialize.h"


DECLARE_INTERFACE_IID_(IWzUOL, IWzSerialize, "f945bf59-d1ec-45e8-8bd9-3dd11ac1a48a") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzUOL methods ***/
    STDMETHOD(raw_Init)(THIS_ wchar_t*, tagVARIANT) PURE;
    STDMETHOD(get_UOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(put_UOL)(THIS_ wchar_t*) PURE;
    STDMETHOD(get_filePath)(THIS_ wchar_t**) PURE;
    STDMETHOD(get_className)(THIS_ wchar_t**) PURE;
    STDMETHOD(get_currentDirectory)(THIS_ wchar_t**) PURE;
    STDMETHOD(put_currentDirectory)(THIS_ wchar_t*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzUOL, __uuidof(IWzUOL));