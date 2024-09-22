#pragma once
#include <comdef.h>
#include "archive.h"


DECLARE_INTERFACE_IID_(IWzSerialize, IUnknown, "01110bc6-d843-4ff1-918c-9cf64632fb15") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzSerialize, __uuidof(IWzSerialize));