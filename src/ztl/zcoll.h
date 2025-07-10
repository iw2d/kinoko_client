#pragma once
#include "zalloc.h"
#include <cstdint>
#include <memory>


template <typename T>
T* construct(T* p);

template <typename T>
void destruct(T* p);


template <typename T>
class ZArray {
private:
    T* a;

public:
    explicit ZArray(uint32_t u, const ZAllocHelper& _ALLOC = ZAllocHelper(0)) : a(nullptr) {
        _Alloc(u, _ALLOC);
    }
    ZArray() : a(nullptr) {
    }
    ~ZArray() {
        RemoveAll();
    }
    const T& operator[](size_t i) const {
        return a[i];
    }
    T& operator[](size_t i) {
        return a[i];
    }
    explicit operator const T*() const {
        return a;
    }
    explicit operator T*() {
        return a;
    }

    uint32_t GetCount() const {
        if (!a) {
            return 0;
        }
        return _GetCount();
    }
    int32_t IsEmpty() const {
        return !a || GetCount() == 0;
    }
    uint32_t GetCapacity() const {
        if (!a) {
            return 0;
        }
        uint32_t uCapacity = ZAllocBase::_MemSize(reinterpret_cast<uint32_t*>(a) - 1);
        return (uCapacity - sizeof(uint32_t)) / sizeof(T);
    };

    T* Alloc(uint32_t u, const ZAllocHelper& _ALLOC = ZAllocHelper(0)) {
        return _Alloc(u, _ALLOC);
    }
    T* Realloc(uint32_t u, int32_t nMode, const ZAllocHelper& _ALLOC = ZAllocHelper(0)) {
        return _Realloc(u, nMode, _ALLOC);
    }
    void RemoveAll() {
        if (a) {
            _Destroy(a, std::addressof(a[GetCount()]));
            ZAllocEx<ZAllocAnonSelector>::s_Free(reinterpret_cast<uint32_t*>(a) - 1);
            a = nullptr;
        }
    }

private:
    T* _Alloc(uint32_t u, const ZAllocHelper& _ALLOC) {
        RemoveAll();
        if (u == 0) {
            return nullptr;
        }
        auto p = static_cast<uint32_t*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(sizeof(T) * u + sizeof(uint32_t))) + 1;
        a = reinterpret_cast<T*>(p);
        _GetCount() = u;
        _Construct(a, std::addressof(a[u]));
        return a;
    }
    T* _Realloc(uint32_t u, int32_t nMode, const ZAllocHelper& _ALLOC) {
        uint32_t uCount = GetCount();
        if (u > uCount) {
            if (u > GetCapacity()) {
                auto p = static_cast<uint32_t*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(sizeof(T) * u + sizeof(uint32_t))) + 1;
                if (a) {
                    if ((nMode & 1) == 0) {
                        memcpy(p, a, uCount * sizeof(T));
                    }
                    ZAllocEx<ZAllocAnonSelector>::s_Free(reinterpret_cast<uint32_t*>(a) - 1);
                }
                a = reinterpret_cast<T*>(p);
            } else if ((nMode & 2) == 0) {
                _Construct(std::addressof(a[uCount]), std::addressof(a[u]));
            }
        } else {
            _Destroy(std::addressof(a[u]), std::addressof(a[uCount]));
        }
        if (a) {
            _GetCount() = u;
        }
        return a;
    }
    uint32_t& _GetCount() const {
        return *(reinterpret_cast<uint32_t*>(a) - 1);
    }

    static void _Construct(T* b, T* e) {
        for (T* i = b; i < e; ++i) {
            construct<T>(i);
        }
    }
    static void _Destroy(T* b, T* e) {
        for (T* i = b; i < e; ++i) {
            destruct<T>(i);
        }
    }
};

static_assert(sizeof(ZArray<int32_t>) == 0x4);


template <typename T>
class ZList : private ZRefCountedAccessor<T>, private ZRefCountedAccessor<ZRefCountedDummy<T>> {
private:
    uint32_t _m_uCount;
    T* _m_pHead;
    T* _m_pTail;

public:
    virtual ~ZList() {
        RemoveAll();
    }
    ZList() : _m_uCount(0), _m_pHead(nullptr), _m_pTail(nullptr) {
    }

    uint32_t GetCount() const {
        return _m_uCount;
    }
    int32_t IsEmpty() const {
        return _m_uCount == 0;
    }
    T* GetHeadPosition() const {
        return _m_pHead;
    }
    T* GetTailPosition() const {
        return _m_pTail;
    }
    const T& GetHead() const {
        return *_m_pHead;
    }
    T& GetHead() {
        return *_m_pHead;
    }
    const T& GetTail() const {
        return *_m_pTail;
    }
    T& GetTail() {
        return *_m_pTail;
    }
    void AddTail(const ZList<T>& l) {
        auto p = l._m_pHead;
        while (p) {
            AddTail() = *p;
            p = _GetNext(p);
        }
    }
    T& AddTail() {
        auto pNew = _New(_m_pTail, nullptr);
        if (_m_pTail) {
            _SetNext(_m_pTail, pNew);
        } else {
            _m_pHead = pNew;
        }
        _m_pTail = pNew;
        return *pNew;
    }
    void RemoveAll() {
        auto p = _m_pHead;
        while (p) {
            auto pNext = _GetNext(p);
            _Delete(p);
            p = pNext;
        }
        _m_pTail = nullptr;
        _m_pHead = nullptr;
        _m_uCount = 0;
    }

    static T& GetNext(T*& pos) {
        T& result = *pos;
        pos = pos ? _GetNext(pos) : nullptr;
        return result;
    }
    static T& GetPrev(T*& pos) {
        T& result = *pos;
        pos = pos ? _GetPrev(pos) : nullptr;
        return result;
    }

private:
    T* _New(void* pPrev, void* pNext) {
        auto pDummy = new ZRefCountedDummy<T>();
        pDummy->_m_pPrev = pPrev ? ZRefCountedDummy<T>::From(pPrev) : nullptr;
        pDummy->_m_pNext = pNext ? ZRefCountedDummy<T>::From(pNext) : nullptr;
        ++_m_uCount;
        return std::addressof(pDummy->t);
    }
    T* _New(ZRefCounted* pPrev, ZRefCounted* pNext) {
        auto p = new T();
        p->_m_pPrev = pPrev;
        p->_m_pNext = pNext;
        ++_m_uCount;
        return p;
    }
    void _Delete(void* p) {
        delete ZRefCountedDummy<T>::From(p);
    }
    void _Delete(ZRefCounted* p) {
        delete p;
    }

    static T* _GetNext(void* p) {
        auto pDummy = static_cast<ZRefCountedDummy<T>*>(ZRefCountedDummy<T>::From(p)->_m_pNext);
        return pDummy ? std::addressof(pDummy->t) : nullptr;
    }
    static T* _GetNext(ZRefCounted* p) {
        return p->_m_pNext;
    }
    static void _SetNext(void* p, void* pNext) {
        ZRefCountedDummy<T>::From(p)->_m_pNext = ZRefCountedDummy<T>::From(pNext);
    }
    static void _SetNext(ZRefCounted* p, ZRefCounted* pNext) {
        p->_m_pNext = pNext;
    }
    static T* _GetPrev(void* p) {
        auto pDummy = static_cast<ZRefCountedDummy<T>*>(ZRefCountedDummy<T>::From(p)->_m_pPrev);
        return pDummy ? std::addressof(pDummy->t) : nullptr;
    }
    static T* _GetPrev(ZRefCounted* p) {
        return p->_m_pPrev;
    }
    static void _SetPrev(void* p, void* pPrev) {
        ZRefCountedDummy<T>::From(p)->_m_pPrev = ZRefCountedDummy<T>::From(pPrev);
    }
    static void _SetPrev(ZRefCounted* p, ZRefCounted* pPrev) {
        p->_m_pPrev = pPrev;
    }
};

static_assert(sizeof(ZList<ZRefCounted>) == 0x14);