#pragma once
#include <cstdint>


class CUIToolTip {
public:
    uint8_t padding[0xA48 - sizeof(void**)];

    CUIToolTip() {
        reinterpret_cast<void(__thiscall*)(CUIToolTip*)>(0x008839C0)(this);
    }
    virtual ~CUIToolTip() {
        reinterpret_cast<void(__thiscall*)(CUIToolTip*)>(0x00882F30)(this);
    }
    void ClearToolTip() {
        reinterpret_cast<void(__thiscall*)(CUIToolTip*)>(0x00880F20)(this);
    }
};

static_assert(sizeof(CUIToolTip) == 0xA48);