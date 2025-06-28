#pragma once
#include "hook.h"
#include "ztl/tsingleton.h"
#include "ztl/zalloc.h"
#include "ztl/ztl.h"
#include "wvs/wnd.h"
#include "wvs/ctrlwnd.h"
#include <cstdint>


class CDialog : public CWnd {
public:
    inline static CRTTI& ms_RTTI_CDialog = *reinterpret_cast<CRTTI*>(0x00C5689C);

    uint8_t padding[0x90 - sizeof(CWnd)];
    MEMBER_AT(int32_t, 0x80, m_nRet)
    MEMBER_AT(int32_t, 0x84, m_bTerminate)
    MEMBER_AT(ZRef<CDialog>, 0x88, m_pChildModal)

    CDialog() : CWnd() {
        construct(&m_pChildModal);
    }
    explicit CDialog(int32_t nDummy) : CWnd(0) {
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
    virtual const CRTTI* GetRTTI() const override {
        return &ms_RTTI_CDialog;
    }
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const override {
        return ms_RTTI_CDialog.IsKindOf(pRTTI);
    }

    void CreateDlg(const wchar_t* sUOL, int32_t bScreenCoord, void* pData, int32_t bMulti) {
        reinterpret_cast<void(__thiscall*)(CDialog*, const wchar_t*, int32_t, void*, int32_t)>(0x004FEC40)(this, sUOL, bScreenCoord, pData, bMulti);
    }
    void CreateDlgOnCenter(const wchar_t* sUOL) {
        reinterpret_cast<void(__thiscall*)(CDialog*, const wchar_t*)>(0x004FEDD0)(this, sUOL);
    }
    int32_t DoModal() {
        return reinterpret_cast<int32_t(__thiscall*)(CDialog*)>(0x004FE7A0)(this);
    }
};

static_assert(sizeof(CDialog) == 0x90);


class CUniqueModeless : public CDialog, public TSingleton<CUniqueModeless, 0x00C64060> {
public:
    inline static CRTTI& ms_RTTI_CUniqueModeless = *reinterpret_cast<CRTTI*>(0x00C568A0);

    uint8_t padding[0x94 - sizeof(CDialog)];

    CUniqueModeless() : CDialog(0) {
        reinterpret_cast<void(__thiscall*)(CUniqueModeless*)>(0x004FEB70)(this);
    }
    ~CUniqueModeless() {
        ms_pInstance = nullptr;
    }

    virtual void SetRet(int32_t nRet) override {
        reinterpret_cast<void(__thiscall*)(CUniqueModeless*, int32_t)>(0x00429310)(this, nRet);
    }
    virtual void ResetInfo() {
        ;
    }

    virtual const CRTTI* GetRTTI() const override {
        return &ms_RTTI_CUniqueModeless;
    }
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const override {
        return ms_RTTI_CUniqueModeless.IsKindOf(pRTTI);
    }
};

static_assert(sizeof(CUniqueModeless) == 0x94);