#pragma once
#include "serialize.h"
#include <comdef.h>
#include <cstdint>


struct IWzSound;

DECLARE_INTERFACE_IID_(IWzSoundState, IUnknown, "d9730ba4-23f5-4c2d-95d4-0e7d3df3765d") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSoundState-specific methods ***/
    STDMETHOD(get_parentSound)(THIS_ IWzSound**) PURE;
    STDMETHOD(get_playing)(THIS_ int32_t*) PURE;
    STDMETHOD(put_playing)(THIS_ int32_t) PURE;
    STDMETHOD(get_chain)(THIS_ tagVARIANT*) PURE;
    STDMETHOD(put_chain)(THIS_ tagVARIANT) PURE;
    STDMETHOD(get_chainValue)(THIS_ int32_t*) PURE;
    STDMETHOD(put_chainValue)(THIS_ int32_t) PURE;
    STDMETHOD(get_volume)(THIS_ int32_t*) PURE;
    STDMETHOD(put_volume)(THIS_ int32_t) PURE;
    STDMETHOD(raw_MoveVolume)(THIS_ int32_t, int32_t) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzSoundState, __uuidof(IWzSoundState));


DECLARE_INTERFACE_IID_(IWzSound, IWzSerialize, "1c923939-1338-4f8b-92cf-38935cee1fef") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzSound methods ***/
    STDMETHOD(raw_GlobalInit)(THIS_ tagVARIANT vHwnd, tagVARIANT vChannels, tagVARIANT vSampleRate, tagVARIANT vBitsPerSample) PURE;
    STDMETHOD(raw_GlobalClose)(THIS) PURE;
    STDMETHOD(raw_GlobalSetBuffer)(THIS_ uint32_t, uint32_t) PURE;
    STDMETHOD(get_globalVolume)(THIS_ int32_t*) PURE;
    STDMETHOD(put_globalVolume)(THIS_ int32_t) PURE;
    STDMETHOD(get_allSoundStates)(THIS_ IEnumVARIANT**) PURE;
    STDMETHOD(get_globalFocus)(THIS_ int32_t *) PURE;
    STDMETHOD(put_globalFocus)(THIS_ int32_t) PURE;
    STDMETHOD(raw_Play)(THIS_ tagVARIANT, tagVARIANT, IWzSoundState**) PURE;
    STDMETHOD(get_length)(THIS_ int32_t *) PURE;
    STDMETHOD(get_type)(THIS_ wchar_t **) PURE;
    STDMETHOD(raw_GetSourceBuffer)(THIS_ uint8_t**, uint32_t*) PURE;

    END_INTERFACE;
};

_COM_SMARTPTR_TYPEDEF(IWzSound, __uuidof(IWzSound));