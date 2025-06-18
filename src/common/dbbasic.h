#pragma once
#include <cstdint>


#pragma pack(push, 1)
struct FUNCKEY_MAPPED {
    uint8_t nType;
    int32_t nID;
};
#pragma pack(pop)

static_assert(sizeof(FUNCKEY_MAPPED) == 0x5);