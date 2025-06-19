#pragma once
#include "util.h"
#include "gobj.h"
#include "msghandler.h"
#include "wnd.h"
#include "ztl/zalloc.h"
#include "ztl/zcom.h"
#include "ztl/ztl.h"
#include "wzlib/shape2d.h"

#include <windows.h>
#include <cstdint>


typedef int32_t FONT_TYPE;

struct DRAGCTX {
    IUIMsgHandler* pContainer;
    ZRef<IDraggable> pObj;
};

static_assert(sizeof(DRAGCTX) == 0xC);


class CCtrlWnd : public IGObj, public IUIMsgHandler, public ZRefCounted {
protected:
    MEMBER_AT(uint32_t, 0x14, m_nCtrlId)
    MEMBER_AT(IWzVector2DPtr, 0x18, m_pLTCtrl)
    MEMBER_AT(CWnd*, 0x24, m_pParent)
    MEMBER_AT(int32_t, 0x28, m_bAcceptFocus)
    MEMBER_AT(int32_t, 0x2C, m_bEnabled)
    MEMBER_AT(int32_t, 0x30, m_bShown)

public:
    CCtrlWnd() {
        m_nCtrlId = -1;
        construct(&m_pLTCtrl);
        construct(&m_pParent); // m_pParent = nullptr;
        m_bAcceptFocus = 1;
        m_bEnabled = 1;
        m_bShown = 1;
    }
    explicit CCtrlWnd(int32_t nDummy) {
        ;
    }

    virtual ~CCtrlWnd() override {
        destruct(&m_pLTCtrl);
    }
    virtual void Update() override {
        ;
    }
    virtual int32_t OnDragDrop(int32_t nState, DRAGCTX& ctx, int32_t rx, int32_t ry) {
        return 0;
    }
    virtual void CreateCtrl(CWnd* pParent, uint32_t nId, int32_t l, int32_t t, int32_t w, int32_t h, void* pData) {
        reinterpret_cast<void(__thiscall*)(CCtrlWnd*, CWnd*, uint32_t, int32_t, int32_t, int32_t, int32_t, void*)>(0x004F0900)(this, pParent, nId, l, t, w, h, pData);
    }
    virtual void Destroy() {
        reinterpret_cast<void(__thiscall*)(CCtrlWnd*)>(0x004F0220)(this);
    }
    virtual void OnCreate(void* pData) {
        ;
    }
    virtual void OnDestroy(void* pData) {
        ;
    }
    virtual int32_t HitTest(uint32_t rx, uint32_t ry) {
        return reinterpret_cast<int32_t(__thiscall*)(CCtrlWnd*, uint32_t, uint32_t)>(0x004EFD30)(this, rx, ry);
    }
    virtual void GetRect(tagRECT* pRect) {
        reinterpret_cast<void(__thiscall*)(CCtrlWnd*, tagRECT*)>(0x004EFE10)(this, pRect);
    }
    virtual void SetAbove(CCtrlWnd* pCtrl) {
        reinterpret_cast<void(__thiscall*)(CCtrlWnd*, CCtrlWnd*)>(0x004F0B50)(this, pCtrl);
    }
    virtual void Draw(int32_t rx, int32_t ry, const tagRECT* pRect) {
        ;
    }

    virtual void OnKey(uint32_t wParam, uint32_t lParam) override {
        m_pParent->OnKey(wParam, lParam);
    }
    virtual int32_t OnSetFocus(int32_t bFocus) override {
        if (bFocus) {
            return !!m_pParent;
        } else {
            return 1;
        }
    }
    virtual void OnMouseButton(uint32_t msg, uint32_t wParam, int32_t rx, int32_t ry) override {
        ;
    }
    virtual int32_t OnMouseMove(int32_t rx, int32_t ry) override {
        return 0;
    }
    virtual void OnMouseEnter(int32_t bEnter) override {
        return reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, int32_t)>(0x004EFDE0)(this, bEnter);
    }
    virtual void SetEnable(int32_t bEnable) override {
        reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, int32_t)>(0x004EFF80)(this, bEnable);
    }
    virtual int32_t IsEnabled() const override {
        return m_bAcceptFocus;
    }
    virtual void SetShow(int32_t bShow) override {
        reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, int32_t)>(0x004EFDA0)(this, bShow);
    }
    virtual int32_t IsShown() const override {
        return m_bEnabled;
    }
    virtual int32_t GetAbsLeft() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x00471890)(this);
    }
    virtual int32_t GetAbsTop() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x004718F0)(this);
    }
};


class CCtrlComboBox : public CCtrlWnd {
public:
    struct CREATEPARAM {
        uint8_t padding[0x54];
        MEMBER_AT(int32_t, 0x0, nBackColor)
        MEMBER_AT(int32_t, 0x4, nBackFocusedColor)
        MEMBER_AT(int32_t, 0x8, nBorderColor)
        MEMBER_AT(Ztl_bstr_t, 0x4C, sButtonUOL)

        CREATEPARAM() {
            reinterpret_cast<void(__thiscall*)(CREATEPARAM*)>(0x004894F0)(this);
        }
        ~CREATEPARAM() {
            destruct(&sButtonUOL);
        }
    };
    static_assert(sizeof(CREATEPARAM) == 0x54);

    uint8_t padding[0x110 - sizeof(CCtrlWnd)];
    MEMBER_AT(int, 0x68, m_nSelect)

    CCtrlComboBox() : CCtrlWnd(0) {
        reinterpret_cast<void(__thiscall*)(CCtrlComboBox*)>(0x004DA090)(this);
    }
    virtual void CreateCtrl(CWnd* pParent, uint32_t nId, int32_t nType, int32_t l, int32_t t, int32_t w, int32_t h, void* pData) {
        reinterpret_cast<void(__thiscall*)(CCtrlComboBox*, CWnd*, uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void*)>(0x004DA330)(this, pParent, nId, nType, l, t, w, h, pData);
    }
    void AddItem(const char* sItemName, uint32_t dwParam) {
        reinterpret_cast<void(__thiscall*)(CCtrlComboBox*, const char*, uint32_t)>(0x004DE640)(this, sItemName, dwParam);
    }
    void SetSelect(int32_t nSelect) {
        reinterpret_cast<void(__thiscall*)(CCtrlComboBox*, int32_t)>(0x004D90E0)(this, nSelect);
    }
};

static_assert(sizeof(CCtrlComboBox) == 0x110);