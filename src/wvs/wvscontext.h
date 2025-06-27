#pragma once
#include "hook.h"
#include "ztl/tsingleton.h"
#include "ztl/zalloc.h"
#include "common/secure.h"
#include "wvs/temporarystatview.h"
#include <cstdint>


struct GW_ItemSlotBase : public ZRefCounted {
    MEMBER_AT(TSecType<int32_t>, 0xC, nItemID)
};

struct CharacterData {
    MEMBER_ARRAY_AT(ZRef<GW_ItemSlotBase>, 0xF9, aEquipped, 60)
    MEMBER_ARRAY_AT(int32_t, 0x74D, aElemBoost, 8)
};

class CWvsContext : public TSingleton<CWvsContext, 0x00C64068> {
public:
    MEMBER_AT(ZRef<CharacterData>, 0x20C8, m_pCharacterData)
    MEMBER_AT(CTemporaryStatView, 0x35D4, m_temporaryStatView)
    MEMBER_AT(int32_t, 0x40BC, m_bKillMobFromEnterField)
    MEMBER_AT(int32_t, 0x41B8, m_nScreenWidth)
    MEMBER_AT(int32_t, 0x41BC, m_nScreenHeight)
    MEMBER_AT(int32_t, 0x41C0, m_nAdjustCenterY)
    MEMBER_AT(bool, 0x41C4, m_bIsLargeScreen)

    void UI_Close(int32_t nUIType) {
        reinterpret_cast<void(__thiscall*)(CWvsContext*, int32_t)>(0x009D5370)(this, nUIType);
    }
};