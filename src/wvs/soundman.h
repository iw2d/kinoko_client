#pragma once
#include "ztl/tsingleton.h"
#include "ztl/zcom.h"
#include <cstdint>


class CSoundMan : public TSingleton<CSoundMan, 0x00C687A8> {
public:
    MEMBER_AT(Ztl_bstr_t, 0x10, m_sBGMPath)

    void PlayBGM(const wchar_t* sPath, int32_t bLoop, uint32_t uFadeOutTime, uint32_t uFadeInTime, int32_t bForceRestart) {
        reinterpret_cast<void(__thiscall*)(CSoundMan*, const wchar_t*, int32_t, uint32_t, uint32_t, int32_t)>(0x00446040)(this, sPath, bLoop, uFadeOutTime, uFadeInTime, bForceRestart);
    }
};