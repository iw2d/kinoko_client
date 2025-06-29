#pragma once
#include "hook.h"
#include "ztl/zalloc.h"
#include "wzlib/gr2d.h"
#include "wzlib/canvas.h"
#include "wvs/gobj.h"
#include "wvs/msghandler.h"
#include <cstdint>
#include <memory>


struct DRAGCTX;
class CCtrlWnd;

class CWnd : public IGObj, public IUIMsgHandler, public ZRefCounted {
protected:
    inline static CRTTI& ms_RTTI_CWnd = *reinterpret_cast<CRTTI*>(0x00C6194C);

    uint8_t padding[0x80 - sizeof(IGObj) - sizeof(IUIMsgHandler) - sizeof(ZRefCounted)];
    MEMBER_AT(uint32_t, 0x14, m_dwWndKey)
    MEMBER_AT(uint32_t, 0x3C, m_bScreenCoord)

public:
    enum UIOrigin : int32_t {
        Origin_LT = 0x0,
        Origin_CT = 0x1,
        Origin_RT = 0x2,
        Origin_LC = 0x3,
        Origin_CC = 0x4,
        Origin_RC = 0x5,
        Origin_LB = 0x6,
        Origin_CB = 0x7,
        Origin_RB = 0x8,
        Origin_NUM = 0x9,
    };

    CWnd() {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009AED30)(this);
    }
    explicit CWnd(int32_t nDummy) {
        ;
    }

    virtual ~CWnd() override {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009AEBC0)(this);
    }
    virtual void Update() override {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009AE7C0)(this);
    }
    virtual int32_t OnDragDrop(int32_t nState, DRAGCTX& ctx, int32_t rx, int32_t ry) {
        return 0;
    }
    virtual void PreCreateWnd(int32_t l, int32_t t, int32_t w, int32_t h, int32_t z, int32_t bScreenCoord, void* pData) {
        reinterpret_cast<void(__thiscall*)(CWnd*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void*)>(0x009ADDC0)(this, l, t, w, h, z, bScreenCoord, pData);
    }
    virtual void OnCreate(void* pData) {
        ;
    }
    virtual void OnDestroy() {
        ;
    }
    virtual void OnMoveWnd(int32_t l, int32_t t) {
        reinterpret_cast<void(__thiscall*)(CWnd*, int32_t, int32_t)>(0x009AEE10)(this, l, t);
    }
    virtual void OnEndMoveWnd() {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009AECA0)(this);
    }
    virtual void OnChildNotify(uint32_t nId, uint32_t param1, uint32_t param2) {
        if (param1 == 100) {
            OnButtonClicked(nId);
        }
    }
    virtual void OnButtonClicked(uint32_t nId) {
        ;
    }
    virtual int32_t HitTest(int32_t rx, int32_t ry, CCtrlWnd** ppCtrl) {
        return reinterpret_cast<int32_t(__thiscall*)(CWnd*, int32_t, int32_t, CCtrlWnd**)>(0x009AE3B0)(this, rx, ry, ppCtrl);
    }
    virtual int32_t OnActivate(int32_t bActive) {
        return reinterpret_cast<int32_t(__thiscall*)(CWnd*, int32_t)>(0x009AD350)(this, bActive);
    }
    virtual void Draw(const RECT* pRect) {
        reinterpret_cast<void(__thiscall*)(CWnd*, const RECT*)>(0x009AE5C0)(this, pRect);
    }
    virtual int32_t IsAMyAddon(CWnd* pWnd) {
        return 0;
    }

    virtual void OnKey(uint32_t wParam, uint32_t lParam) override {
        ;
    }
    virtual int32_t OnSetFocus(int32_t bFocus) override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*, int32_t)>(0x009AD3A0)(this, bFocus);
    }
    virtual void OnMouseButton(uint32_t msg, uint32_t wParam, int32_t rx, int32_t ry) override {
        ;
    }
    virtual int32_t OnMouseMove(int32_t rx, int32_t ry) override {
        return 0;
    }
    virtual int32_t OnMouseWheel(int32_t rx, int32_t ry, int32_t nWheel) override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*, int32_t, int32_t, int32_t)>(0x009AE4D0)(this, rx, ry, nWheel);
    }
    virtual void OnMouseEnter(int32_t bEnter) override {
        reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, int32_t)>(0x009AD370)(this, bEnter);
    }
    virtual int32_t GetAbsLeft() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x009AD480)(this);
    }
    virtual int32_t GetAbsTop() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x009AD570)(this);
    }
    virtual const CRTTI* GetRTTI() const override {
        return &ms_RTTI_CWnd;
    }
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const override {
        return ms_RTTI_CWnd.IsKindOf(pRTTI);
    }

    void CreateWnd() {
        // TODO
    }
    void Destroy() {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009B0E50)(this);
    }
    IWzGr2DLayerPtr GetLayer() {
                IWzGr2DLayerPtr result;
        reinterpret_cast<IWzGr2DLayerPtr*(__thiscall*)(CWnd*, IWzGr2DLayerPtr*)>(0x0042A270)(this, std::addressof(result));
        return result;
    }
    IWzCanvasPtr GetCanvas() {
        IWzCanvasPtr result;
        reinterpret_cast<IWzCanvasPtr*(__thiscall*)(CWnd*, IWzCanvasPtr*)>(0x0042B170)(this, std::addressof(result));
        return result;
    }
    void InvalidateRect(const RECT* pRect) {
        reinterpret_cast<void(__thiscall*)(CWnd*, const RECT*)>(0x009AD3F0)(this, pRect);
    }
};

static_assert(sizeof(CWnd) == 0x80);