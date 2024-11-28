#pragma once


class ZAllocAnonSelector;

template<typename T>
class ZAllocEx {
public:
    static void** s_Alloc(size_t uSize);
    static void s_Free(void* p);
};

static ZAllocEx<ZAllocAnonSelector>* s_alloc = reinterpret_cast<ZAllocEx<ZAllocAnonSelector>*>(0x00E1EB64);

template<>
inline void** ZAllocEx<ZAllocAnonSelector>::s_Alloc(size_t uSize) {
    return reinterpret_cast<void** (__thiscall*)(ZAllocEx<ZAllocAnonSelector>*, unsigned int)>(0x00401FF0)(s_alloc, uSize);
}

template<>
inline void ZAllocEx<ZAllocAnonSelector>::s_Free(void* p) {
    return reinterpret_cast<void (__thiscall*)(ZAllocEx<ZAllocAnonSelector>*, void*)>(0x00402100)(s_alloc, p);
}