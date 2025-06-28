#pragma once
#include "common/dbbasic.h"
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
    void ShowItemToolTip(int32_t x, int32_t y, GW_ItemSlotBase* p, void* pParam, void* pGoods, int32_t bCashShop, int32_t nNpcShopTimeLimitedItemPeriod, int32_t itcToolTip) {
        reinterpret_cast<void(__thiscall*)(CUIToolTip*, int32_t, int32_t, GW_ItemSlotBase*, void*, void*, int32_t, int32_t, int32_t)>(0x008A9300)(this, x, y, p, pParam, pGoods, bCashShop, nNpcShopTimeLimitedItemPeriod, itcToolTip);
    }
};

static_assert(sizeof(CUIToolTip) == 0xA48);