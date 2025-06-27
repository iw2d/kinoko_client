#pragma once
#include "hook.h"
#include "ztl/tsingleton.h"
#include "wzlib/property.h"
#include <cstdint>
#include <memory>


struct EQUIPITEM {
    MEMBER_AT(ZtlSecure<uint8_t>, 0xA0, nRUC)
    MEMBER_AT(short, 0x1C0, nirPoison)
    MEMBER_AT(short, 0x1C2, nirIce)
    MEMBER_AT(short, 0x1C4, nirFire)
    MEMBER_AT(short, 0x1C6, nirLight)
    MEMBER_AT(short, 0x1C8, nirHoly)
    MEMBER_AT(int, 0x264, nEnchantCategory)
};


class CItemInfo : public TSingleton<CItemInfo, 0x00C63EA4> {
public:
    IWzPropertyPtr GetItemInfo(int32_t nItemID) {
        IWzPropertyPtr result;
        reinterpret_cast<IWzPropertyPtr*(__thiscall*)(CItemInfo*, IWzPropertyPtr*, int)>(0x005A8F20)(this, std::addressof(result), nItemID);
        return result;
    }
    EQUIPITEM* GetEquipItem(int32_t nItemID) {
        return reinterpret_cast<EQUIPITEM* (__thiscall*)(CItemInfo*, int32_t)>(0x005C0820)(this, nItemID);
    }
};