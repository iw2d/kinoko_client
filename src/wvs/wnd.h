#pragma once
#include "gobj.h"
#include "msghandler.h"
#include "ztl/zalloc.h"
#include <cstdint>


struct DRAGCTX {
    IUIMsgHandler* pContainer;
    ZRef<IDraggable> pObj;
};

static_assert(sizeof(DRAGCTX) == 0xC);


class CCtrlWnd;

class CWnd : public IGObj, public IUIMsgHandler, public ZRefCounted {
public:
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
        ;
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
    virtual void Draw(const tagRECT* pRect) {
        reinterpret_cast<void(__thiscall*)(CWnd*)>(0x009AE5C0)(this);
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
        return reinterpret_cast<void(__thiscall*)(IUIMsgHandler*, int32_t)>(0x009AD370)(this, bEnter);
    }
    virtual int32_t GetAbsLeft() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x009AD480)(this);
    }
    virtual int32_t GetAbsTop() override {
        return reinterpret_cast<int32_t(__thiscall*)(IUIMsgHandler*)>(0x009AD570)(this);
    }
};