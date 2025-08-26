#include "pch.h"
#include "hook.h"
#include "ztl/ztl.h"
#include "wvs/util.h"
#include <cstdint>


typedef HRESULT(__stdcall* IWzCanvas__raw_Serialize_t)(IWzCanvas*, IWzArchive*);
static IWzCanvas__raw_Serialize_t CWzCanvas__raw_Serialize;

HRESULT __stdcall CWzCanvas__raw_Serialize_hook(IWzCanvas* pThis, IWzArchive* pArchive) {
    HRESULT hr = CWzCanvas__raw_Serialize(pThis, pArchive);
    if (FAILED(hr)) {
        return hr;
    }
    Ztl_variant_t vInlink = pThis->property->item["_inlink"];
    if (V_VT(&vInlink) == VT_BSTR) {
        ZXString<wchar_t> sFilePath(pArchive->absoluteUOL);
        sFilePath.ReleaseBuffer(sFilePath.Find(L".img") + 5);
        sFilePath.Cat(V_BSTR(&vInlink));
        pThis->property->item["_inlink"] = static_cast<const wchar_t*>(sFilePath);
    }
    return hr;
}


void HandleLinkProperty(IWzCanvasPtr pCanvas) {
    // Check for link property
    const wchar_t* asLinkProperty[] = {
            L"_inlink",
            L"_outlink",
            L"source",
    };
    size_t nLinkProperty = sizeof(asLinkProperty) / sizeof(asLinkProperty[0]);
    for (size_t i = 0; i < nLinkProperty; ++i) {
        Ztl_variant_t vLink = pCanvas->property->item[asLinkProperty[i]];
        if (V_VT(&vLink) == VT_BSTR) {
            // Get source canvas
            IWzCanvasPtr pSourceCanvas = get_rm()->GetObjectA(V_BSTR(&vLink)).GetUnknown();
            int32_t nWidth, nHeight, nFormat, nMagLevel;
            pSourceCanvas->GetSnapshot(&nWidth, &nHeight, nullptr, nullptr, (CANVAS_PIXFORMAT*)&nFormat, &nMagLevel);

            // Create target canvas
            pCanvas->Create(nWidth, nHeight, nMagLevel, nFormat);
            pCanvas->AddRawCanvas(0, 0, pSourceCanvas->rawCanvas[0][0]);

            // Set target origin
            IWzVector2DPtr pOrigin = pCanvas->property->item[L"origin"].GetUnknown();
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
    CWzCanvas__raw_Serialize = reinterpret_cast<IWzCanvas__raw_Serialize_t>(GetAddressByPattern("CANVAS.DLL", "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 74"));
    ATTACH_HOOK(CWzCanvas__raw_Serialize, CWzCanvas__raw_Serialize_hook);
    ATTACH_HOOK(get_unknown_orig, get_unknown_hook);
    ATTACH_HOOK(Ztl_variant_t__GetUnknown, Ztl_variant_t__GetUnknown_hook); // for cases where nexon uses this instead of get_unknown
}