#pragma once
#include "ztl/tsingleton.h"
#include "wzlib/property.h"
#include <cstdint>
#include <memory>


class CItemInfo : public TSingleton<CItemInfo, 0x00C63EA4> {
public:
    IWzPropertyPtr GetItemInfo(int32_t nItemID) {
        IWzPropertyPtr result;
        reinterpret_cast<IWzPropertyPtr*(__thiscall*)(CItemInfo*, IWzPropertyPtr*, int)>(0x005A8F20)(this, std::addressof(result), nItemID);
        return result;
    }
};