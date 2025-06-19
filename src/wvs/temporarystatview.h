#pragma once
#include "util.h"
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include "wzlib/gr2d.h"
#include "common/uint128.h"
#include <cstdint>


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

    MEMBER_AT(ZList<ZRef<TEMPORARY_STAT>>, 0x4, m_lTemporaryStat)

    void Show() {
        reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075C6A0)(this);
    }
};