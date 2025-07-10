#include "pch.h"
#include "hook.h"
#include "ztl/ztl.h"
#include "wzlib/pcom.h"
#include "wvs/util.h"
#include <cstdint>


static auto CWzCanvas__raw_Serialize = reinterpret_cast<HRESULT(__stdcall*)(IWzCanvas*, IWzArchive*)>(0x500216D0);

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
    Ztl_variant_t vInlink;
    hr = pProperty->get_item(L"_inlink", &vInlink);
    if (FAILED(hr)) {
        return hr;
    }
    if (V_VT(&vInlink) == VT_BSTR) {
        wchar_t* sUOL;
        hr = pArchive->get_absoluteUOL(&sUOL);
        if (FAILED(hr)) {
            return hr;
        }
        Ztl_variant_t vFilePath;
        V_VT(&vFilePath) = VT_BSTR;
        V_BSTR(&vFilePath) = ZComAPI::ZComSysAllocStringLen(sUOL, wcslen(sUOL) + wcslen(V_BSTR(&vInlink)) + 1);
        wchar_t* p = wcsstr(V_BSTR(&vFilePath), L".img");
        if (p) {
            *(p + 4) = '/';
            memcpy(p + 5, V_BSTR(&vInlink), sizeof(wchar_t) * (wcslen(V_BSTR(&vInlink)) + 1));
        }
        hr = pProperty->put_item(L"_inlink", vFilePath);
    }
    return hr;
}


void HandleLinkProperty(IWzCanvasPtr pCanvas) {
    // Check for link property
    IWzPropertyPtr pProperty = pCanvas->property;
    const wchar_t* asLinkProperty[] = {
            L"_inlink",
            L"_outlink",
            L"source",
    };
    size_t nLinkProperty = sizeof(asLinkProperty) / sizeof(asLinkProperty[0]);
    for (size_t i = 0; i < nLinkProperty; ++i) {
        Ztl_variant_t vLink = pProperty->item[asLinkProperty[i]];
        if (V_VT(&vLink) == VT_BSTR) {
            // Get source canvas
            Ztl_variant_t vSourceCanvas = get_rm()->GetObjectA(V_BSTR(&vLink), vtEmpty, vtEmpty);
            IWzCanvasPtr pSourceCanvas = vSourceCanvas.GetUnknown(false, false);
            int32_t nWidth, nHeight, nFormat, nMagLevel;
            pSourceCanvas->raw_GetSnapshot(&nWidth, &nHeight, nullptr, nullptr, (CANVAS_PIXFORMAT*)&nFormat, &nMagLevel);

            // Create target canvas
            Ztl_variant_t vMagLevel(nMagLevel, VT_I4);
            Ztl_variant_t vFormat(nFormat, VT_I4);
            pCanvas->Create(nWidth, nHeight, vMagLevel, vFormat);
            pCanvas->raw_AddRawCanvas(0, 0, pSourceCanvas->rawCanvas[0][0]);

            // Set target origin
            IWzVector2DPtr pOrigin = pProperty->item[L"origin"];
            pCanvas->cx = pOrigin->x;
            pCanvas->cy = pOrigin->y;
            break;
        }
    }
}

static auto get_unknown_orig = reinterpret_cast<IUnknownPtr*(__cdecl*)(IUnknownPtr*, Ztl_variant_t*)>(0x004176E0);

IUnknownPtr* __cdecl get_unknown_hook(IUnknownPtr* result, Ztl_variant_t* v) {
    get_unknown_orig(result, v);
    IWzCanvasPtr pCanvas;
    if (SUCCEEDED(result->QueryInterface(__uuidof(IWzCanvas), &pCanvas))) {
        HandleLinkProperty(pCanvas);
    }
    return result;
}


static auto Ztl_variant_t__GetUnknown = reinterpret_cast<IUnknown*(__thiscall*)(Ztl_variant_t*, bool, bool)>(0x00401A60);

IUnknown* __fastcall Ztl_variant_t__GetUnknown_hook(Ztl_variant_t* pThis, void* _EDX, bool fAddRef, bool fTryChangeType) {
    IUnknownPtr result = pThis->GetUnknown(fAddRef, fTryChangeType);
    IWzCanvasPtr pCanvas;
    if (SUCCEEDED(result.QueryInterface(__uuidof(IWzCanvas), &pCanvas))) {
        HandleLinkProperty(pCanvas);
    }
    return result;
}


void AttachClientInlink() {
    LoadLibraryA("CANVAS.DLL");
    ATTACH_HOOK(CWzCanvas__raw_Serialize, CWzCanvas__raw_Serialize_hook);
    ATTACH_HOOK(get_unknown_orig, get_unknown_hook);
    ATTACH_HOOK(Ztl_variant_t__GetUnknown, Ztl_variant_t__GetUnknown_hook); // for cases where nexon uses this instead of get_unknown
}