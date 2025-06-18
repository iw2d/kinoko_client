#pragma once
#include "ztl/tsingleton.h"
#include <cstdint>


struct ISMSG {
    uint32_t message;
    uint32_t wParam;
    int32_t lParam;
};
static_assert(sizeof(ISMSG) == 0xC);

class CInputSystem : public TSingleton<CInputSystem, 0x00C68C20> {
};
