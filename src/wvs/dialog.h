#pragma once
#include "hook.h"
#include "ztl/zalloc.h"
#include "ztl/ztl.h"
#include "wvs/wnd.h"
#include "wvs/ctrlwnd.h"
#include <cstdint>


class CDialog : public CWnd {
protected:
    MEMBER_AT(int32_t, 0x80, m_nRet)
    MEMBER_AT(int32_t, 0x84, m_bTerminate)
    MEMBER_AT(ZRef<CDialog>, 0x88, m_pChildModal)

public:
    CDialog() : CWnd() {
        construct(&m_pChildModal);
    }
    explicit CDialog(int32_t nDummy) {
        ;
    }
    virtual ~CDialog() override {
        destruct(&m_pChildModal);
    }
    virtual void OnDestroy() override {
        if (m_pChildModal) {
            m_pChildModal->SetRet(3);
            m_pChildModal = nullptr;
        }
    }
    virtual void OnButtonClicked(uint32_t nId) override {
        switch (nId) {
        case 1:
            SetRet(1);
            break;
        case 2:
            SetRet(2);
            break;
        case 8:
            SetRet(8);
            break;
        }
    }
    virtual int32_t HitTest(int32_t rx, int32_t ry, CCtrlWnd** ppCtrl) override {
        int32_t result = CWnd::HitTest(rx, ry, ppCtrl);
        if (result && (!ppCtrl || !*ppCtrl)) {
            if (m_bScreenCoord) {
                return 1;
            }
        }
        return result;
    }
    virtual void SetRet(int32_t nRet) {
        if (!m_bTerminate) {
            m_nRet = nRet;
            m_bTerminate = 1;
            if (!m_dwWndKey) {
                Destroy();
            }
        }
    }

    virtual void OnKey(uint32_t wParam, uint32_t lParam) override {
        reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, uint32_t, uint32_t)>(0x004FEAD0)(this, wParam, lParam);
    }
};