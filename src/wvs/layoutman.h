#pragma once
#include "hook.h"
#include "ztl/ztl.h"
#include "wzlib/pcom.h"
#include "wvs/wnd.h"
#include "wvs/ctrlwnd.h"
#include <cstdint>
#include <memory>


class CLayoutMan {
public:
    CWnd* m_pWnd;
    int32_t m_nOffsetX;
    int32_t m_nOffsetY;
    ZArray<IWzGr2DLayerPtr> m_aLayer;
    ZArray<ZRef<CCtrlOriginButton>> m_aButton;

    CLayoutMan() : m_pWnd(nullptr),
                   m_nOffsetX(0),
                   m_nOffsetY(0),
                   m_aLayer(),
                   m_aButton() {
    }
    ~CLayoutMan() = default;

    void Init(CWnd* pWnd, int32_t nOffsetX, int32_t nOffsetY) {
        m_pWnd = pWnd;
        m_nOffsetX = nOffsetX;
        m_nOffsetY = nOffsetY;
    }
    IWzGr2DLayerPtr AddLayer(const wchar_t* sLayerUOL, int32_t z, int32_t bManaged) {
        IWzGr2DLayerPtr result;
        reinterpret_cast<IWzGr2DLayerPtr*(__thiscall*)(CLayoutMan*, IWzGr2DLayerPtr*, const wchar_t*, int32_t, int32_t)>(0x005CDB70)(this, std::addressof(result), sLayerUOL, z, bManaged);
        return result;
    }
    ZRef<CCtrlOriginButton> AddButton(const wchar_t* sButtonUOL, uint32_t nID, int32_t nOffsetX, int32_t nOffsetY, CCtrlButton::CREATEPARAM* pParam) {
        ZRef<CCtrlOriginButton> result;
        reinterpret_cast<ZRef<CCtrlOriginButton>*(__thiscall*)(CLayoutMan*, ZRef<CCtrlOriginButton>*, const wchar_t*, uint32_t, int32_t, int32_t, CCtrlButton::CREATEPARAM*)>(0x005CF210)(this, std::addressof(result), sButtonUOL, nID, nOffsetX, nOffsetY, pParam);
        return result;
    }
    int32_t CopyToCanvas(IWzCanvasPtr pCanvas, wchar_t* sSrcUOL, int32_t nOffsetX, int32_t nOffsetY) {
        return reinterpret_cast<int32_t(__thiscall*)(CLayoutMan*, IWzCanvasPtr, wchar_t*, int32_t, int32_t)>(0x005CEFC0)(this, pCanvas, sSrcUOL, nOffsetX, nOffsetY);
    }
    int32_t CopyToCanvas(IWzCanvasPtr pCanvas, IWzCanvasPtr pSrcCanvas, int32_t nOffsetX, int32_t nOffsetY) {
        return reinterpret_cast<int32_t(__thiscall*)(CLayoutMan*, IWzCanvasPtr, IWzCanvasPtr, int32_t, int32_t)>(0x005CD350)(this, pCanvas, pSrcCanvas, nOffsetX, nOffsetY);
    }
};

static_assert(sizeof(CLayoutMan) == 0x14);