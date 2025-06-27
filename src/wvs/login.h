#pragma once
#include "hook.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include "wvs/maploadable.h"
#include <cstdint>


class CSystemInfo {
public:
    uint8_t SupportId[16];
    uint8_t MachineId[16];

    virtual ~CSystemInfo() = default;
};
static_assert(sizeof(CSystemInfo) == 0x24);


class CLogin : public CMapLoadable {
public:
    struct CHANNELITEM {
        ZXString<char> sName;
        int32_t nUserNo;
        int32_t nWorldID;
        int32_t nChanenlID;
        int32_t bAdultChannel;
    };
    static_assert(sizeof(CHANNELITEM) == 0x14);

    struct WORLDITEM {
        int32_t nWorldID;
        ZXString<char> sName;
        int32_t nWorldState;
        ZXString<char> sWorldEventDesc;
        int32_t nWorldEventEXP_WSE;
        int32_t nWorldEventDrop_WSE;
        int32_t nBlockCharCreation;
        ZArray<CHANNELITEM> ci;
    };
    static_assert(sizeof(WORLDITEM) == 0x20);

    struct BALLOON {
        int32_t nX;
        int32_t nY;
        ZXString<char> sMessage;
    };
    static_assert(sizeof(BALLOON) == 0xC);

    MEMBER_AT(int, 0x1AC, m_bRequestSent)
    MEMBER_AT(ZArray<WORLDITEM>, 0x1CC, m_WorldItem)
    MEMBER_AT(ZArray<BALLOON>, 0x230, m_aBalloon)
};