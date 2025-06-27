#pragma once
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include "wzlib/gr2d.h"
#include "common/uint128.h"
#include <cstdint>


enum : int32_t {
    TSV_NONE = 0x0,
    TSV_ITEM = 0x1,
    TSV_SKILL = 0x2,
    TSV_ETC = 0x3,
    TSV_PRIVILEGE = 0x4,
};


class CTemporaryStatView {
public:
    class TEMPORARY_STAT : public ZRefCounted {
    public:
        UINT128 uFlagTemp;
        int32_t nType;
        int32_t nID;
        int32_t nSubID;
        ZXString<char> sToolTip;
        IWzGr2DLayerPtr pLayer;
        IWzGr2DLayerPtr pLayerShadow;
        int32_t nIndexShadow;
        int32_t tHideTime;
        int32_t bNoShadow;
        int32_t tLeft;
        int32_t tLeftUnit;

        virtual ~TEMPORARY_STAT() override = default;
    };
    static_assert(sizeof(TEMPORARY_STAT) == 0x48);

public:
    ZList<ZRef<TEMPORARY_STAT>> m_lTemporaryStat;

    CTemporaryStatView() = default;
    virtual ~CTemporaryStatView() = default;

    void SetTemporary(int32_t nType, int32_t nID, int32_t tDuration, UINT128 uFlagTemp, ZXString<char> sToolTip, int32_t nSubID, int32_t nHideTime) {
        reinterpret_cast<void (__thiscall*)(CTemporaryStatView*, int, int, int, UINT128, ZXString<char>, int, int)>(0x0075FA50)(this, nType, nID, tDuration, uFlagTemp, sToolTip, nSubID, nHideTime);
    }
    void ResetTemporary(int32_t nType, int32_t nID) {
        reinterpret_cast<void (__thiscall*)(CTemporaryStatView*, int32_t, int32_t)>(0x0075D030)(this, nType, nID);
    }
};

static_assert(sizeof(CTemporaryStatView) == 0x18);