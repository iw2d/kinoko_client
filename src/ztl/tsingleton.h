#pragma once


template<typename T, uintptr_t pInstance>
class TSingleton {
public:
    static inline T* GetInstance() {
        return *reinterpret_cast<T**>(pInstance);
    }
    static inline bool IsInstantiated() {
        return *reinterpret_cast<T**>(pInstance) != nullptr;
    }
};