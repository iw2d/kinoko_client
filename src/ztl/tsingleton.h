#pragma once


template<typename T, uintptr_t pInstance>
class TSingleton {
public:
    static T* GetInstance() {
        return *reinterpret_cast<T**>(pInstance);
    }
    static bool IsInstantiated() {
        return *reinterpret_cast<T**>(pInstance) == nullptr;
    }
};