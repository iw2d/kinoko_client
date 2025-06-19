#pragma once
#include "rtti.h"
#include "ztl/zalloc.h"
#include "wzlib/gr2d.h"
#include "common/dbbasic.h"
#include "common/packet.h"
#include <cstdint>


class IUIMsgHandler;

class IDraggable : public ZRefCounted {
public:
    IWzGr2DLayerPtr m_pLayer;
    FUNCKEY_MAPPED m_OldIcon;

    virtual ~IDraggable() override = default;
    virtual int32_t OnDropped(IUIMsgHandler* pFrom, IUIMsgHandler* pTo, int32_t rx, int32_t ry) {
        return 0;
    }
    virtual int32_t OnDoubleClicked() {
        return 0;
    }
};

static_assert(sizeof(IDraggable) == 0x18);


class IUIMsgHandler {
public:
    virtual void OnKey(uint32_t wParam, uint32_t lParam) {
        ;
    }
    virtual int32_t OnSetFocus(int32_t bFocus) {
        return 0;
    }
    virtual void OnMouseButton(uint32_t msg, uint32_t wParam, int32_t rx, int32_t ry) {
        ;
    }
    virtual int32_t OnMouseMove(int32_t rx, int32_t ry) {
        return 0;
    }
    virtual int32_t OnMouseWheel(int32_t rx, int32_t ry, int32_t nWheel) {
        return 0;
    }
    virtual void OnMouseEnter(int32_t bEnter) {
        ;
    }
    virtual void OnDraggableMove(int32_t nState, IDraggable* pObj, int32_t rx, int32_t ry) {
        ;
    }
    virtual void SetEnable(int32_t bEnable) {
        ;
    }
    virtual int32_t IsEnabled() const {
        return 1;
    }
    virtual void SetShow(int32_t bShow) {
        ;
    }
    virtual int32_t IsShown() const {
        return 1;
    }
    virtual int32_t GetAbsLeft() {
        return 0;
    }
    virtual int32_t GetAbsTop() {
        return 0;
    }
    virtual void ClearToolTip() {
        ;
    }
    virtual void OnIMEModeChange(char cMode) {
        ;
    }
    virtual void OnIMEResult(const char* sComp) {
        ;
    }
    virtual void OnIMEComp(const char* sComp, void* adwCls, uint32_t nClsIdx, int32_t nCursor, void* lCand, int32_t nBegin, int32_t nPage, int32_t nCur) {
        ;
    }
    virtual const CRTTI* GetRTTI() const {
        return nullptr;
    }
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const {
        return 0;
    }
};

static_assert(sizeof(IUIMsgHandler) == 0x4);


class INetMsgHandler {
public:
    virtual void OnPacket(int32_t nType, CInPacket& iPacket) {
        ;
    }
};

static_assert(sizeof(INetMsgHandler) == 0x4);