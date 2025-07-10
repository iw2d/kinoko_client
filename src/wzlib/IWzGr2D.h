#pragma once
#pragma pack(push, 8)

#include "IWzGr2DLayer.h"
#include "ztl/ztl.h"
#include <comdef.h>

struct __declspec(uuid("e576ea33-d465-4f08-aab1-e78df73ee6d9"))
/* interface */ IWzGr2D;

_COM_SMARTPTR_TYPEDEF(IWzGr2D, __uuidof(IWzGr2D));

struct __declspec(uuid("e576ea33-d465-4f08-aab1-e78df73ee6d9"))
IWzGr2D : IUnknown
{
    __declspec(property(put=PutvideoVolume))
    int videoVolume;
    __declspec(property(get=GetnextRenderTime))
    int nextRenderTime;
    __declspec(property(get=Getwidth))
    unsigned int width;
    __declspec(property(get=Getheight))
    unsigned int height;
    __declspec(property(put=PutscreenResolution))
    unsigned int screenResolution[];
    __declspec(property(get=Getbpp))
    unsigned int bpp;
    __declspec(property(get=GetrefreshRate))
    unsigned int refreshRate;
    __declspec(property(get=Getfps100))
    unsigned int fps100;
    __declspec(property(get=GetcurrentTime))
    int currentTime;
    __declspec(property(get=GetfullScreen,put=PutfullScreen))
    int fullScreen;
    __declspec(property(get=GetbackColor,put=PutbackColor))
    unsigned int backColor;
    __declspec(property(get=GetredTone))
    IWzVector2DPtr redTone;
    __declspec(property(get=GetgreenBlueTone))
    IWzVector2DPtr greenBlueTone;
    __declspec(property(get=Getcenter))
    IWzVector2DPtr center;
    __declspec(property(get=GetvideoStatus))
    int videoStatus;

    HRESULT Initialize (
        unsigned int uWidth,
        unsigned int uHeight,
        const Ztl_variant_t & vHwnd = vtEmpty,
        const Ztl_variant_t & vBPP = vtEmpty,
        const Ztl_variant_t & vRefreshRate = vtEmpty );
    HRESULT Uninitialize ( );
    int GetnextRenderTime ( );
    HRESULT UpdateCurrentTime (
        int tTime );
    HRESULT RenderFrame ( );
    HRESULT SetFrameSkip ( );
    HRESULT ToggleFpsPanel ( );
    HRESULT DisableFpsPanel ( );
    unsigned int Getwidth ( );
    unsigned int Getheight ( );
    void PutscreenResolution (
        unsigned int uWidth,
        unsigned int _arg2 );
    unsigned int Getbpp ( );
    unsigned int GetrefreshRate ( );
    unsigned int Getfps100 ( );
    int GetcurrentTime ( );
    int GetfullScreen ( );
    void PutfullScreen (
        int pnFullScreen );
    unsigned int GetbackColor ( );
    void PutbackColor (
        unsigned int puColor );
    IWzVector2DPtr GetredTone ( );
    IWzVector2DPtr GetgreenBlueTone ( );
    IWzVector2DPtr Getcenter ( );
    HRESULT GetSnapshot (
        const Ztl_variant_t & pvBuffer,
        unsigned int uBufferSize,
        int x,
        int y );
    IWzGr2DLayerPtr CreateLayer (
        int nLeft,
        int nTop,
        unsigned int uWidth,
        unsigned int uHeight,
        int nZ,
        const Ztl_variant_t & vCanvas = vtEmpty,
        const Ztl_variant_t & dwFilter = vtEmpty );
    HRESULT AdjustCenter (
        int nAdjustCenterX,
        int nAdjustCenterY );
    HRESULT TakeScreenShot (
        Ztl_bstr_t sFileName,
        int nFileFormat );
    HRESULT SetVideoMode (
        int bMode );
    HRESULT SetVideoPath (
        Ztl_bstr_t sPath,
        int unk1,
        int unk2 );
    HRESULT PlayVideo ( );
    HRESULT PauseVideo (
        int nPause );
    HRESULT StopVideo ( );
    int GetvideoStatus ( );
    void PutvideoVolume (
        int _arg1 );

    virtual HRESULT __stdcall raw_Initialize (
        /*[in]*/ unsigned int uWidth,
        /*[in]*/ unsigned int uHeight,
        /*[in]*/ VARIANT vHwnd = vtEmpty,
        /*[in]*/ VARIANT vBPP = vtEmpty,
        /*[in]*/ VARIANT vRefreshRate = vtEmpty ) = 0;
    virtual HRESULT __stdcall raw_Uninitialize ( ) = 0;
    virtual HRESULT __stdcall get_nextRenderTime (
        /*[out,retval]*/ int * pnTime ) = 0;
    virtual HRESULT __stdcall raw_UpdateCurrentTime (
        /*[in]*/ int tTime ) = 0;
    virtual HRESULT __stdcall raw_RenderFrame ( ) = 0;
    virtual HRESULT __stdcall raw_SetFrameSkip ( ) = 0;
    virtual HRESULT __stdcall raw_ToggleFpsPanel ( ) = 0;
    virtual HRESULT __stdcall raw_DisableFpsPanel ( ) = 0;
    virtual HRESULT __stdcall get_width (
        /*[out,retval]*/ unsigned int * puWidth ) = 0;
    virtual HRESULT __stdcall get_height (
        /*[out,retval]*/ unsigned int * puHeight ) = 0;
    virtual HRESULT __stdcall put_screenResolution (
        /*[in]*/ unsigned int uWidth,
        /*[in]*/ unsigned int _arg2 ) = 0;
    virtual HRESULT __stdcall get_bpp (
        /*[out,retval]*/ unsigned int * puBPP ) = 0;
    virtual HRESULT __stdcall get_refreshRate (
        /*[out,retval]*/ unsigned int * puRefreshRate ) = 0;
    virtual HRESULT __stdcall get_fps100 (
        /*[out,retval]*/ unsigned int * puFps100 ) = 0;
    virtual HRESULT __stdcall get_currentTime (
        /*[out,retval]*/ int * pnCurrentTime ) = 0;
    virtual HRESULT __stdcall get_fullScreen (
        /*[out,retval]*/ int * pnFullScreen ) = 0;
    virtual HRESULT __stdcall put_fullScreen (
        /*[in]*/ int pnFullScreen ) = 0;
    virtual HRESULT __stdcall get_backColor (
        /*[out,retval]*/ unsigned int * puColor ) = 0;
    virtual HRESULT __stdcall put_backColor (
        /*[in]*/ unsigned int puColor ) = 0;
    virtual HRESULT __stdcall get_redTone (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_greenBlueTone (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall get_center (
        /*[out,retval]*/ struct IWzVector2D * * ppVector ) = 0;
    virtual HRESULT __stdcall raw_GetSnapshot (
        /*[in]*/ VARIANT pvBuffer,
        /*[in]*/ unsigned int uBufferSize,
        /*[in]*/ int x,
        /*[in]*/ int y ) = 0;
    virtual HRESULT __stdcall raw_CreateLayer (
        /*[in]*/ int nLeft,
        /*[in]*/ int nTop,
        /*[in]*/ unsigned int uWidth,
        /*[in]*/ unsigned int uHeight,
        /*[in]*/ int nZ,
        /*[in]*/ VARIANT vCanvas,
        /*[in]*/ VARIANT dwFilter,
        /*[out,retval]*/ struct IWzGr2DLayer * * ppLayer ) = 0;
    virtual HRESULT __stdcall raw_AdjustCenter (
        /*[in]*/ int nAdjustCenterX,
        /*[in]*/ int nAdjustCenterY ) = 0;
    virtual HRESULT __stdcall raw_TakeScreenShot (
        /*[in]*/ BSTR sFileName,
        /*[in]*/ int nFileFormat ) = 0;
    virtual HRESULT __stdcall raw_SetVideoMode (
        /*[in]*/ int bMode ) = 0;
    virtual HRESULT __stdcall raw_SetVideoPath (
        /*[in]*/ BSTR sPath,
        /*[in]*/ int unk1,
        /*[in]*/ int unk2 ) = 0;
    virtual HRESULT __stdcall raw_PlayVideo ( ) = 0;
    virtual HRESULT __stdcall raw_PauseVideo (
        /*[in]*/ int nPause ) = 0;
    virtual HRESULT __stdcall raw_StopVideo ( ) = 0;
    virtual HRESULT __stdcall get_videoStatus (
        /*[out,retval]*/ int * pnStatus ) = 0;
    virtual HRESULT __stdcall put_videoVolume (
        /*[in]*/ int _arg1 ) = 0;
};


inline HRESULT IWzGr2D::Initialize ( unsigned int uWidth, unsigned int uHeight, const Ztl_variant_t & vHwnd, const Ztl_variant_t & vBPP, const Ztl_variant_t & vRefreshRate ) {
    HRESULT _hr = raw_Initialize(uWidth, uHeight, vHwnd, vBPP, vRefreshRate);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::Uninitialize ( ) {
    HRESULT _hr = raw_Uninitialize();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzGr2D::GetnextRenderTime ( ) {
    int _result = 0;
    HRESULT _hr = get_nextRenderTime(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IWzGr2D::UpdateCurrentTime ( int tTime ) {
    HRESULT _hr = raw_UpdateCurrentTime(tTime);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::RenderFrame ( ) {
    HRESULT _hr = raw_RenderFrame();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::SetFrameSkip ( ) {
    HRESULT _hr = raw_SetFrameSkip();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::ToggleFpsPanel ( ) {
    HRESULT _hr = raw_ToggleFpsPanel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::DisableFpsPanel ( ) {
    HRESULT _hr = raw_DisableFpsPanel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline unsigned int IWzGr2D::Getwidth ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_width(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzGr2D::Getheight ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_height(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2D::PutscreenResolution ( unsigned int uWidth, unsigned int _arg2 ) {
    HRESULT _hr = put_screenResolution(uWidth, _arg2);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzGr2D::Getbpp ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_bpp(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzGr2D::GetrefreshRate ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_refreshRate(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned int IWzGr2D::Getfps100 ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_fps100(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzGr2D::GetcurrentTime ( ) {
    int _result = 0;
    HRESULT _hr = get_currentTime(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline int IWzGr2D::GetfullScreen ( ) {
    int _result = 0;
    HRESULT _hr = get_fullScreen(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2D::PutfullScreen ( int pnFullScreen ) {
    HRESULT _hr = put_fullScreen(pnFullScreen);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned int IWzGr2D::GetbackColor ( ) {
    unsigned int _result = 0;
    HRESULT _hr = get_backColor(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2D::PutbackColor ( unsigned int puColor ) {
    HRESULT _hr = put_backColor(puColor);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IWzVector2DPtr IWzGr2D::GetredTone ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_redTone(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline IWzVector2DPtr IWzGr2D::GetgreenBlueTone ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_greenBlueTone(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline IWzVector2DPtr IWzGr2D::Getcenter ( ) {
    struct IWzVector2D * _result = 0;
    HRESULT _hr = get_center(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzVector2DPtr(_result, false);
}

inline HRESULT IWzGr2D::GetSnapshot ( const Ztl_variant_t & pvBuffer, unsigned int uBufferSize, int x, int y ) {
    HRESULT _hr = raw_GetSnapshot(pvBuffer, uBufferSize, x, y);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IWzGr2DLayerPtr IWzGr2D::CreateLayer ( int nLeft, int nTop, unsigned int uWidth, unsigned int uHeight, int nZ, const Ztl_variant_t & vCanvas, const Ztl_variant_t & dwFilter ) {
    struct IWzGr2DLayer * _result = 0;
    HRESULT _hr = raw_CreateLayer(nLeft, nTop, uWidth, uHeight, nZ, vCanvas, dwFilter, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IWzGr2DLayerPtr(_result, false);
}

inline HRESULT IWzGr2D::AdjustCenter ( int nAdjustCenterX, int nAdjustCenterY ) {
    HRESULT _hr = raw_AdjustCenter(nAdjustCenterX, nAdjustCenterY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::TakeScreenShot ( Ztl_bstr_t sFileName, int nFileFormat ) {
    HRESULT _hr = raw_TakeScreenShot(sFileName, nFileFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::SetVideoMode ( int bMode ) {
    HRESULT _hr = raw_SetVideoMode(bMode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::SetVideoPath ( Ztl_bstr_t sPath, int unk1, int unk2 ) {
    HRESULT _hr = raw_SetVideoPath(sPath, unk1, unk2);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::PlayVideo ( ) {
    HRESULT _hr = raw_PlayVideo();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::PauseVideo ( int nPause ) {
    HRESULT _hr = raw_PauseVideo(nPause);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IWzGr2D::StopVideo ( ) {
    HRESULT _hr = raw_StopVideo();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline int IWzGr2D::GetvideoStatus ( ) {
    int _result = 0;
    HRESULT _hr = get_videoStatus(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IWzGr2D::PutvideoVolume ( int _arg1 ) {
    HRESULT _hr = put_videoVolume(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}


#pragma pack(pop)
