#pragma once
#include "ztl/zalloc.h"
#include "common/secure.h"
#include <cstdint>


#pragma pack(push, 1)
struct FUNCKEY_MAPPED {
    uint8_t nType;
    int32_t nID;
};
#pragma pack(pop)

static_assert(sizeof(FUNCKEY_MAPPED) == 0x5);


struct GW_ItemSlotBase : public ZRefCounted {
    uint8_t padding[0x28 - sizeof(ZRefCounted)];
    MEMBER_AT(TSecType<int32_t>, 0xC, nItemID)
};

static_assert(sizeof(GW_ItemSlotBase) == 0x28);