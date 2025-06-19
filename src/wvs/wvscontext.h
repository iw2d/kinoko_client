#pragma once
#include "util.h"
#include "temporarystatview.h"
#include "ztl/tsingleton.h"
#include <cstdint>


class CWvsContext : public TSingleton<CWvsContext, 0x00C64068> {
public:
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