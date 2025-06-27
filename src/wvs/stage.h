#pragma once
#include "ztl/zalloc.h"
#include "wvs/gobj.h"
#include "wvs/msghandler.h"


class CStage : public IGObj, public IUIMsgHandler, public INetMsgHandler, public ZRefCounted {
public:
    CStage() = default;
    virtual ~CStage() override {
        reinterpret_cast<void(__thiscall*)(CStage*)>(this);
    }
    virtual void Init(void* pParam) {
        ;
    }
    virtual void Close() {
        ;
    }
};

static_assert(sizeof(CStage) == 0x18);


inline ZRef<CStage>& get_stage() {
    return *reinterpret_cast<ZRef<CStage>*>(0x00C6B638);
}


inline void set_stage(CStage* pStage, void* pParam) {
    reinterpret_cast<void(__cdecl*)(CStage*, void*)>(0x00719C30)(pStage, pParam);
}