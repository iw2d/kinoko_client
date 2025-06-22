#pragma once
#include "util.h"
#include "gobj.h"
#include "ztl/zalloc.h"
#include "ztl/tsingleton.h"
#include "wzlib/shape2d.h"
#include "common/vecctrl.h"
#include <cstdint>


class CLife : public IGObj, public IVecCtrlOwner, public ZRefCounted {
};


class CAvatar {
};


class CUser : public CLife, public CAvatar {
public:
    MEMBER_AT(IWzVector2DPtr, 0x19E4, m_pvc)
    MEMBER_AT(uint32_t, 0x19E8, m_dwCharacterId)
};


class CUserLocal : public CUser, public TSingleton<CUserLocal, 0x00C68754> {
public:
    MEMBER_AT(int32_t, 0x4808, m_bJumpKeyUp)

    int32_t GetJobCode() {
        return reinterpret_cast<int32_t(__thiscall*)(CUserLocal*)>(0x00908EB0)(this);
    }
    int32_t DoActiveSkill(int32_t nSkillID, uint32_t nScanCode, int32_t* pnConsumeCheck) {
        return reinterpret_cast<int32_t(__thiscall*)(CUserLocal*, int32_t, uint32_t, int32_t*)>(0x009445B0)(this, nSkillID, nScanCode, pnConsumeCheck);
    }
};