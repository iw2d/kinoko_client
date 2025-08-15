#pragma once
#pragma pack(push, 8)

#include "IWzSerialize.h"
#include "ztl/zcom.h"

struct __declspec(uuid("1c923939-1338-4f8b-92cf-38935cee1fef"))
/* interface */ IWzSound;
struct __declspec(uuid("d9730ba4-23f5-4c2d-95d4-0e7d3df3765d"))
/* interface */ IWzSoundState;

_COM_SMARTPTR_TYPEDEF(IWzSound, __uuidof(IWzSound));
_COM_SMARTPTR_TYPEDEF(IWzSoundState, __uuidof(IWzSoundState));

struct __declspec(uuid("1c923939-1338-4f8b-92cf-38935cee1fef"))
IWzSound : IWzSerialize
{
    __declspec(property(get=GetglobalVolume,put=PutglobalVolume))
    int globalVolume;
    __declspec(property(get=GetallSoundStates))
    IEnumVARIANTPtr allSoundStates;
    __declspec(property(get=GetglobalFocus,put=PutglobalFocus))
    int globalFocus;
    __declspec(property(get=Getlength))
    int length;
    __declspec(property(get=Gettype))
    Ztl_bstr_t type;

    HRESULT GlobalInit (
        const Ztl_variant_t & vHwnd = vtEmpty,
        const Ztl_variant_t & nChannels = vtEmpty,
        const Ztl_variant_t & nSampleRate = vtEmpty,
        const Ztl_variant_t & nBitsPerSample = vtEmpty );
    HRESULT GlobalClose ( );
    HRESULT GlobalSetBuffer (
        unsigned int uBufferSize,
        unsigned int uMinStream );
    int GetglobalVolume ( );
    void PutglobalVolume (
        int __MIDL__IWzSound0000 );
    IEnumVARIANTPtr GetallSoundStates ( );
    int GetglobalFocus ( );
    void PutglobalFocus (
        int __MIDL__IWzSound0003 );
    IWzSoundStatePtr Play (
        const Ztl_variant_t & nRepeat = vtEmpty,
        const Ztl_variant_t & vRet = vtEmpty );
    int Getlength ( );
    Ztl_bstr_t Gettype ( );
    unsigned int GetSourceBuffer (
        unsigned char * * pSource );

    virtual HRESULT __stdcall raw_GlobalInit (
        /*[in]*/ VARIANT vHwnd = vtEmpty,
        /*[in]*/ VARIANT nChannels = vtEmpty,
        /*[in]*/ VARIANT nSampleRate = vtEmpty,
        /*[in]*/ VARIANT nBitsPerSample = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_GlobalClose ( ) = 0;
    virtual HRESULT __stdcall raw_GlobalSetBuffer (
        /*[in]*/ unsigned int uBufferSize,
        /*[in]*/ unsigned int uMinStream ) = 0;
    virtual HRESULT __stdcall get_globalVolume (
        /*[out,retval]*/ int * __MIDL__IWzSound0000 ) = 0;
    virtual HRESULT __stdcall put_globalVolume (
        /*[in]*/ int __MIDL__IWzSound0000 ) = 0;
    virtual HRESULT __stdcall get_allSoundStates (
        /*[out,retval]*/ struct IEnumVARIANT * * __MIDL__IWzSound0002 ) = 0;
    virtual HRESULT __stdcall get_globalFocus (
        /*[out,retval]*/ int * __MIDL__IWzSound0003 ) = 0;
    virtual HRESULT __stdcall put_globalFocus (
        /*[in]*/ int __MIDL__IWzSound0003 ) = 0;
    virtual HRESULT __stdcall raw_Play (
        /*[in]*/ VARIANT nRepeat,
        /*[in]*/ VARIANT vRet,
        /*[out,retval]*/ struct IWzSoundState * * pNewState ) = 0;
    virtual HRESULT __stdcall get_length (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall get_type (
        /*[out,retval]*/ BSTR * pVal ) = 0;
    virtual HRESULT __stdcall raw_GetSourceBuffer (
        /*[out]*/ unsigned char * * pSource,
        /*[out,retval]*/ unsigned int * pVal ) = 0;
};

struct __declspec(uuid("d9730ba4-23f5-4c2d-95d4-0e7d3df3765d"))
IWzSoundState : IUnknown
{
    __declspec(property(get=GetparentSound))
    IWzSoundPtr parentSound;
    __declspec(property(get=Getplaying,put=Putplaying))
    int playing;
    __declspec(property(get=Getchain,put=Putchain))
    Ztl_variant_t chain;
    __declspec(property(get=GetchainValue,put=PutchainValue))
    int chainValue;
    __declspec(property(get=Getvolume,put=Putvolume))
    int volume;

    IWzSoundPtr GetparentSound ( );
    int Getplaying ( );
    void Putplaying (
        int pnPlaying );
    Ztl_variant_t Getchain ( );
    void Putchain (
        const Ztl_variant_t & pvChain );
    int GetchainValue ( );
    void PutchainValue (
        int pVal );
    int Getvolume ( );
    void Putvolume (
        int pVal );
    HRESULT MoveVolume (
        int nDiff,
        int nDelay );

    virtual HRESULT __stdcall get_parentSound (
        /*[out,retval]*/ struct IWzSound * * ppSound ) = 0;
    virtual HRESULT __stdcall get_playing (
        /*[out,retval]*/ int * pnPlaying ) = 0;
    virtual HRESULT __stdcall put_playing (
        /*[in]*/ int pnPlaying ) = 0;
    virtual HRESULT __stdcall get_chain (
        /*[out,retval]*/ VARIANT * pvChain ) = 0;
    virtual HRESULT __stdcall put_chain (
        /*[in]*/ VARIANT pvChain ) = 0;
    virtual HRESULT __stdcall get_chainValue (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_chainValue (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall get_volume (
        /*[out,retval]*/ int * pVal ) = 0;
    virtual HRESULT __stdcall put_volume (
        /*[in]*/ int pVal ) = 0;
    virtual HRESULT __stdcall raw_MoveVolume (
        /*[in]*/ int nDiff,
        /*[in]*/ int nDelay ) = 0;
};


inline HRESULT IWzSound::GlobalInit ( const Ztl_variant_t & vHwnd, const Ztl_variant_t & nChannels, const Ztl_variant_t & nSampleRate, const Ztl_variant_t & nBitsPerSample ) {
    HRESULT _hr = raw_GlobalInit(vHwnd, nChannels, nSampleRate, nBitsPerSample);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzSound::GlobalClose ( ) {
    HRESULT _hr = raw_GlobalClose();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzSound::GlobalSetBuffer ( unsigned int uBufferSize, unsigned int uMinStream ) {
    HRESULT _hr = raw_GlobalSetBuffer(uBufferSize, uMinStream);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzSound::GetglobalVolume ( ) {
    int _result = 0;
    HRESULT _hr = get_globalVolume(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSound::PutglobalVolume ( int __MIDL__IWzSound0000 ) {
    HRESULT _hr = put_globalVolume(__MIDL__IWzSound0000);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IEnumVARIANTPtr IWzSound::GetallSoundStates ( ) {
    struct IEnumVARIANT * _result = 0;
    HRESULT _hr = get_allSoundStates(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IEnumVARIANTPtr(_result, false);
}

inline int IWzSound::GetglobalFocus ( ) {
    int _result = 0;
    HRESULT _hr = get_globalFocus(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSound::PutglobalFocus ( int __MIDL__IWzSound0003 ) {
    HRESULT _hr = put_globalFocus(__MIDL__IWzSound0003);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzSoundStatePtr IWzSound::Play ( const Ztl_variant_t & nRepeat, const Ztl_variant_t & vRet ) {
    struct IWzSoundState * _result = 0;
    HRESULT _hr = raw_Play(nRepeat, vRet, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzSoundStatePtr(_result, false);
}

inline int IWzSound::Getlength ( ) {
    int _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline Ztl_bstr_t IWzSound::Gettype ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_bstr_t(_result, false);
}

inline unsigned int IWzSound::GetSourceBuffer ( unsigned char * * pSource ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_GetSourceBuffer(pSource, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IWzSoundPtr IWzSoundState::GetparentSound ( ) {
    struct IWzSound * _result = 0;
    HRESULT _hr = get_parentSound(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzSoundPtr(_result, false);
}

inline int IWzSoundState::Getplaying ( ) {
    int _result = 0;
    HRESULT _hr = get_playing(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSoundState::Putplaying ( int pnPlaying ) {
    HRESULT _hr = put_playing(pnPlaying);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline Ztl_variant_t IWzSoundState::Getchain ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_chain(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return Ztl_variant_t(_result, false);
}

inline void IWzSoundState::Putchain ( const Ztl_variant_t & pvChain ) {
    HRESULT _hr = put_chain(pvChain);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzSoundState::GetchainValue ( ) {
    int _result = 0;
    HRESULT _hr = get_chainValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSoundState::PutchainValue ( int pVal ) {
    HRESULT _hr = put_chainValue(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IWzSoundState::Getvolume ( ) {
    int _result = 0;
    HRESULT _hr = get_volume(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzSoundState::Putvolume ( int pVal ) {
    HRESULT _hr = put_volume(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IWzSoundState::MoveVolume ( int nDiff, int nDelay ) {
    HRESULT _hr = raw_MoveVolume(nDiff, nDelay);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}


#pragma pack(pop)
