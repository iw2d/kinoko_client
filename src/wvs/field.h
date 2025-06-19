#pragma once
#include "maploadable.h"


class CField : public CMapLoadable {
};


inline CField* __cdecl get_field() {
    return reinterpret_cast<CField*(__cdecl*)()>(0x00439320)();
}