#pragma once
#include <comdef.h>
#include "archive.h"


enum RESMAN_PARAM {
    RC_AUTO_SERIALIZE = 0x1,
    RC_AUTO_SERIALIZE_NO_CACHE = 0x2,
    RC_NO_AUTO_SERIALIZE = 0x4,
    RC_DEFAULT_AUTO_SERIALIZE = 0x0,
    RC_AUTO_SERIALIZE_MASK = 0x7,
    RC_AUTO_REPARSE = 0x10,
    RC_NO_AUTO_REPARSE = 0x20,
    RC_DEFAULT_AUTO_REPARSE = 0x0,
    RC_AUTO_REPARSE_MASK = 0x30,
};


DECLARE_INTERFACE_IID_(IWzResMan, IUnknown, "57dfe40b-3e20-4dbc-97e8-805a50f381bf") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzResMan methods ***/
    STDMETHOD(get_rootNameSpace)(THIS_ IUnknown**) PURE;
    STDMETHOD(put_rootNameSpace)(THIS_ IUnknown*) PURE;
    STDMETHOD(raw_SetResManParam)(THIS_ int, int, int) PURE;
    STDMETHOD(raw_CreateObject)(THIS_ wchar_t*, IUnknown**) PURE;
    STDMETHOD(raw_GetObject)(THIS_ wchar_t*, tagVARIANT, tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(raw_SerializeObject)(THIS_ IWzArchive*, tagVARIANT, IUnknown**) PURE;
    STDMETHOD(raw_FlushCachedObjects)(THIS_ int) PURE;
    STDMETHOD(raw_OverrideObject)(THIS_ wchar_t*, wchar_t*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzResMan, __uuidof(IWzResMan));