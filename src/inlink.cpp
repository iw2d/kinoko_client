#include "pch.h"
#include "client.h"
#include "hook.h"


typedef HRESULT (__stdcall* CWzCanvas__raw_Serialize_t)(IWzCanvas*, IWzArchive*);
static auto CWzCanvas__raw_Serialize = reinterpret_cast<CWzCanvas__raw_Serialize_t>(0x500216D0);

HRESULT __stdcall CWzCanvas__raw_Serialize_hook(IWzCanvas* pThis, IWzArchive* pArchive) {
    HRESULT hr = CWzCanvas__raw_Serialize(pThis, pArchive);
    if (FAILED(hr)) {
        return hr;
    }
    IWzPropertyPtr pProperty;
    hr = pThis->get_property(&pProperty);
    if (FAILED(hr)) {
        return hr;
    }
    Ztl_variant_t inlink;
    hr = pProperty->get_item(L"_inlink", &inlink);
    if (FAILED(hr)) {
        return hr;
    }
    if (inlink.vt == VT_BSTR) {
        wchar_t* sUOL;
        hr = pArchive->get_absoluteUOL(&sUOL);
        if (FAILED(hr)) {
            return hr;
        }
        Ztl_variant_t filepath;
        filepath.vt = VT_BSTR;
        // filepath.bstrVal = ZComAPI::ZComSysAllocStringLen(sUOL, wcslen(sUOL) + wcslen(inlink.bstrVal) + 1);
        filepath.bstrVal = reinterpret_cast<wchar_t* (__cdecl*)(wchar_t*, unsigned int)>(0x004012D0)(sUOL, wcslen(sUOL) + wcslen(inlink.bstrVal) + 1);
        wchar_t* p = wcsstr(filepath.bstrVal, L".img");
        if (p) {
            *(p + 4) = '/';
            memcpy(p + 5, inlink.bstrVal, sizeof(wchar_t) * (wcslen(inlink.bstrVal) + 1));
        }
        hr = pProperty->put_item(L"_inlink", filepath);
    }
    return hr;
}


void HandleLinkProperty(IWzCanvasPtr pCanvas) {
    // Check for link property
    IWzPropertyPtr pProperty;
    CHECK_HRESULT(pCanvas->get_property(&pProperty));
    const wchar_t* asLinkProperty[] = {
        L"_inlink",
        L"_outlink",
        L"source",
    };
    size_t nLinkProperty = sizeof(asLinkProperty) / sizeof(asLinkProperty[0]);
    for (size_t i = 0; i < nLinkProperty; ++i) {
        Ztl_variant_t link;
        CHECK_HRESULT(pProperty->get_item(const_cast<wchar_t*>(asLinkProperty[i]), &link));
        if (link.vt == VT_BSTR) {
            Ztl_variant_t vEmpty;
            Ztl_variant_t vSource;
            CHECK_HRESULT(get_rm()->raw_GetObject(link.bstrVal, vEmpty, vEmpty, &vSource));
            // Get source canvas
            IWzCanvasPtr pSourceCanvas = IWzCanvasPtr(vSource.GetUnknown(false, false));
            int nWidth, nHeight, nFormat, nMagLevel;
            CHECK_HRESULT(pSourceCanvas->raw_GetSnapshot(&nWidth, &nHeight, nullptr, nullptr, &nFormat, &nMagLevel));
            IWzRawCanvasPtr pRawCanvas;
            CHECK_HRESULT(pSourceCanvas->get_rawCanvas(0, 0, &pRawCanvas));
            // Get origin
            Ztl_variant_t vOrigin;
            CHECK_HRESULT(pProperty->get_item(L"origin", &vOrigin));
            IWzVector2DPtr pOrigin = IWzVector2DPtr(vOrigin.GetUnknown(false, false));
            int nOriginX, nOriginY;
            CHECK_HRESULT(pOrigin->get_x(&nOriginX));
            CHECK_HRESULT(pOrigin->get_y(&nOriginY));
            // Create target canvas
            Ztl_variant_t vMagLevel(nMagLevel, VT_I4);
            Ztl_variant_t vFormat(nFormat, VT_I4);
            CHECK_HRESULT(pCanvas->raw_Create(nWidth, nHeight, vMagLevel, vFormat));
            CHECK_HRESULT(pCanvas->raw_AddRawCanvas(0, 0, pRawCanvas));
            // Set target origin
            CHECK_HRESULT(pCanvas->put_cx(nOriginX));
            CHECK_HRESULT(pCanvas->put_cy(nOriginY));
            break;
        }
    }
}


typedef IUnknownPtr* (__cdecl* get_unknown_t)(IUnknownPtr*, Ztl_variant_t*);
static auto get_unknown = reinterpret_cast<get_unknown_t>(0x004176E0);

IUnknownPtr* __cdecl get_unknown_hook(IUnknownPtr* result, Ztl_variant_t* v) {
    get_unknown(result, v);
    IWzCanvasPtr pCanvas;
    IUnknownPtr pUnknown(*result);
    if (SUCCEEDED(pUnknown.QueryInterface(__uuidof(IWzCanvas), &pCanvas))) {
        HandleLinkProperty(pCanvas);
    }
    return result;
}


typedef IUnknown* (__thiscall* Ztl_variant_t__GetUnknown_t)(Ztl_variant_t*, bool, bool);
static auto Ztl_variant_t__GetUnknown = reinterpret_cast<Ztl_variant_t__GetUnknown_t>(0x00401A60);

IUnknown* __fastcall Ztl_variant_t__GetUnknown_hook(Ztl_variant_t* pThis, void* _EDX, bool fAddRef, bool fTryChangeType) {
    IUnknownPtr pUnknown = Ztl_variant_t__GetUnknown(pThis, fAddRef, fTryChangeType);
    IWzCanvasPtr pCanvas;
    if (SUCCEEDED(pUnknown.QueryInterface(__uuidof(IWzCanvas), &pCanvas))) {
        HandleLinkProperty(pCanvas);
    }
    return pUnknown;
}


void AttachClientInlink() {
    LoadLibrary("CANVAS.DLL");
    ATTACH_HOOK(CWzCanvas__raw_Serialize, CWzCanvas__raw_Serialize_hook);
    ATTACH_HOOK(get_unknown, get_unknown_hook);
    ATTACH_HOOK(Ztl_variant_t__GetUnknown, Ztl_variant_t__GetUnknown_hook); // for cases where nexon uses this instead of get_unknown
}