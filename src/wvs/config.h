#pragma once
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
    MEMBER_AT(CONFIG_SYSOPT, 0x6C, m_sysOpt)
};