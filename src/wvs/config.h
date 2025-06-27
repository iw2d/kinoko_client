#pragma once
#include "hook.h"
#include "ztl/tsingleton.h"
#include <cstdint>


struct CONFIG_SYSOPT {
    int32_t nSysOpt_Video;
    int32_t nSysOpt_BGMVol;
    int32_t bSysOpt_BGMMute;
    int32_t nSysOpt_SEVol;
    int32_t bSysOpt_SEMute;
    int32_t nSysOpt_ScreenShot;
    int32_t nSysOpt_MouseSpeed;
    int32_t nSysOpt_HPFlash;
    int32_t nSysOpt_MPFlash;
    int32_t bSysOpt_Tremble;
    int32_t nSysOpt_MobInfo;
    int32_t bSysOpt_LargeScreen;
    int32_t bSysOpt_WindowedMode;
    int32_t bSysOpt_Minimap_Normal;
};

static_assert(sizeof(CONFIG_SYSOPT) == 0x38);


class CConfig : public TSingleton<CConfig, 0x00C687AC> {
public:
    enum : int32_t {
        GLOBAL_OPT = 0x0,
        LAST_CONNECT_INFO = 0x1,
        CHARACTER_OPT = 0x2,
    };

    MEMBER_AT(CONFIG_SYSOPT, 0x6C, m_sysOpt)

    int32_t GetOpt_Int(int32_t nType, const char* sKey, int32_t nDefaultValue, int32_t nLowBound, int32_t nHighBound) {
        return reinterpret_cast<int32_t(__thiscall*)(CConfig*, int32_t, const char*, int32_t, int32_t, int32_t)>(0x004B25F0)(this, nType, sKey, nDefaultValue, nLowBound, nHighBound);
    }
    void SetOpt_Int(int32_t nType, const char* sKey, int32_t nValue) {
        reinterpret_cast<void(__thiscall*)(CConfig*, int32_t, const char*, int32_t)>(0x004B2650)(this, nType, sKey, nValue);
    }
};