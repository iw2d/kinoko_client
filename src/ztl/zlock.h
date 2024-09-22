#pragma once


template<typename T>
class ZSynchronizedHelper;


class ZFatalSectionData {
private:
    void* _m_pTIB = nullptr;
    volatile long _m_nRef = 0;

    template<typename T>
    friend class ZSynchronizedHelper;
};
static_assert(sizeof(ZFatalSectionData) == 0x8);


class ZFatalSection : public ZFatalSectionData {
private:
    static struct _TEB* __fastcall _TryM(volatile long* __formal) {
        _TEB* pTeb = NtCurrentTeb();
        long result = InterlockedCompareExchange(__formal, reinterpret_cast<long>(pTeb), 0);
        if (result) {
            if (result == reinterpret_cast<long>(pTeb)) {
                InterlockedIncrement(__formal + 1);
                return nullptr;
            }
        } else {
            InterlockedExchange(__formal + 1, 1);
        }
        return reinterpret_cast<_TEB*>(result);
    }

    static struct _TEB* __fastcall _TryS(volatile long* __formal) {
        _TEB* pTeb = NtCurrentTeb();
        long result = InterlockedCompareExchange(__formal, reinterpret_cast<long>(pTeb), 0);
        InterlockedExchange(__formal + 1, 1);
        return reinterpret_cast<_TEB*>(result);
    }

    static struct _TEB* __fastcall _TryI(volatile long *p) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        struct _TEB* (__fastcall* pfnTry)(volatile long*) = ZFatalSection::_TryM;
        if (si.dwNumberOfProcessors <= 1) {
            pfnTry = ZFatalSection::_TryS;
        }
        _s_pfnTry = pfnTry;
        return pfnTry(p);
    }

    inline static struct _TEB* (__fastcall* _s_pfnTry)(volatile long*) = ZFatalSection::_TryI;

public:
    void Lock() {
        volatile long* p = reinterpret_cast<volatile long*>(this);
        if (ZFatalSection::_s_pfnTry(p) && ZFatalSection::_s_pfnTry(p)) {
            do {
                Sleep(0);
            } while (ZFatalSection::_s_pfnTry(p));
        }
    }
};
static_assert(sizeof(ZFatalSection) == 0x8);


template<typename T>
class ZSynchronizedHelper {
public:
    T* m_pLock = nullptr;

    ZSynchronizedHelper(ZFatalSection* lock);
    ~ZSynchronizedHelper();
};
static_assert(sizeof(ZSynchronizedHelper<ZFatalSection>) == 0x4);


template <>
inline ZSynchronizedHelper<ZFatalSection>::ZSynchronizedHelper(ZFatalSection* lock) {
    this->m_pLock = lock;
    lock->Lock();
}

template <>
inline ZSynchronizedHelper<ZFatalSection>::~ZSynchronizedHelper() {
    if (this->m_pLock->_m_nRef-- == 1) {
        this->m_pLock->_m_pTIB = nullptr;
    }
}