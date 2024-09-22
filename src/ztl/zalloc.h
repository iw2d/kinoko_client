#pragma once


class ZAllocAnonSelector;

template<typename T>
class ZAllocEx {
public:
    static void** s_Alloc(size_t uSize);
    static void s_Free(void* p);
};

static ZAllocEx<ZAllocAnonSelector>* s_alloc = reinterpret_cast<ZAllocEx<ZAllocAnonSelector>*>(0x00C6E67C);

template<>
inline void** ZAllocEx<ZAllocAnonSelector>::s_Alloc(size_t uSize) {
    return reinterpret_cast<void** (__thiscall*)(ZAllocEx<ZAllocAnonSelector>*, unsigned int)>(0x00402040)(s_alloc, uSize);
}

template<>
inline void ZAllocEx<ZAllocAnonSelector>::s_Free(void* p) {
    return reinterpret_cast<void (__thiscall*)(ZAllocEx<ZAllocAnonSelector>*, void*)>(0x00402150)(s_alloc, p);
}