#pragma once
#include "ztl/tsingleton.h"
#include <cstdint>


class CWvsContext : public TSingleton<CWvsContext, 0x00C64068> {
public:
    MEMBER_AT(int32_t, 0x41B8, m_nScreenWidth)
    MEMBER_AT(int32_t, 0x41BC, m_nScreenHeight)
    MEMBER_AT(int32_t, 0x41C0, m_nAdjustCenterY)
    MEMBER_AT(bool, 0x41C4, m_bIsLargeScreen)
};