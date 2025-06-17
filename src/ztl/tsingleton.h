#pragma once


template<typename T, uintptr_t ADDRESS>
class TSingleton {
protected:
    inline static T*& ms_pInstance = *reinterpret_cast<T**>(ADDRESS);

public:
    static T* GetInstance() {
        return ms_pInstance;
    }
    static bool IsInstantiated() {
        return ms_pInstance != nullptr;
    }
};