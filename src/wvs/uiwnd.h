#pragma once
#include "hook.h"
#include "ztl/ztl.h"
#include "wvs/wnd.h"
#include "wvs/ctrlwnd.h"
#include "wvs/uitooltip.h"
#include "wvs/wvscontext.h"
#include <cstdint>


class CUIWnd : public CWnd {
public:
    uint8_t padding[0xB08 - sizeof(CWnd)];
    MEMBER_AT(ZRef<CCtrlButton>, 0x80, m_pBtClose)
    MEMBER_AT(CUIToolTip, 0x88, m_uiToolTip)
    MEMBER_AT(int32_t, 0xAD0, m_nUIType)
    MEMBER_AT(ZArray<uint8_t>, 0xB00, m_abOption)
    MEMBER_AT(ZXString<wchar_t>, 0xB04, m_sBackgrndUOL)

    virtual ~CUIWnd() override {
        // implement CUIWnd destructor so we dont call CWnd destructor twice
        destruct(&m_sBackgrndUOL);
        destruct(&m_abOption);
        destruct(&m_uiToolTip);
        destruct(&m_pBtClose);
    }
    virtual void OnCreate(void* pData) override {
        OnCreate(pData, m_sBackgrndUOL, 0);
    }
    virtual void OnDestroy() override {
        reinterpret_cast<void(__thiscall*)(CUIWnd*)>(0x008DD380)(this);
    }
    virtual void OnButtonClicked(uint32_t nId) override {
        if (nId == 1000) {
            CWvsContext::GetInstance()->UI_Close(m_nUIType);
        }
    }
    virtual int32_t HitTest(int32_t rx, int32_t ry, CCtrlWnd** ppCtrl) override {
        int32_t result = CWnd::HitTest(rx, ry, ppCtrl);
        if (result && (!ppCtrl || !*ppCtrl)) {
            return (ry > 0x18) + 1;
        }
        return result;
    }
    virtual void OnCreate(void* pData, ZXString<wchar_t> sUOL, int32_t bMultiBg) {
        reinterpret_cast<void(__thiscall*)(CUIWnd*, void*, ZXString<wchar_t>, int32_t)>(0x008DDB30)(this, pData, sUOL, bMultiBg);
    }
    virtual void SetScreenMode(bool bIsLarge, bool bNeedInvalidateTabs) {
        reinterpret_cast<void(__thiscall*)(CUIWnd*, bool, bool)>(0x008DD570)(this, bIsLarge, bNeedInvalidateTabs);
    }

    virtual void OnKey(uint32_t wParam, uint32_t lParam) override {
        reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, uint32_t, uint32_t)>(0x008DD550)(this, wParam, lParam);
    }
    virtual int32_t OnSetFocus(int32_t bFocus) override {
        return 0;
    }
    virtual void OnMouseEnter(int32_t bEnter) override {
        CWnd::OnMouseEnter(bEnter);
        ClearToolTip();
    }
    virtual void ClearToolTip() override {
        m_uiToolTip.ClearToolTip();
    }
};

static_assert(sizeof(CUIWnd) == 0xB08);