#pragma once
#include "ztl/tsingleton.h"
#include "ztl/zstr.h"
#include "wvs/util.h"
#include <cstdint>


struct SKILLENTRY {
    MEMBER_AT(int32_t, 0x0, nSkillID)
    MEMBER_AT(ZXString<char>, 0x4, sName)
    MEMBER_AT(ZXString<char>, 0x8, sDescription)
};


class CSkillInfo : public TSingleton<CSkillInfo, 0x00C63EA8> {
};