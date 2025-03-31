#include <new>

#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#define SAVE_VTABLES(this) \
    void** vtable1 = *reinterpret_cast<void***>(this); \
    void** vtable2 = *reinterpret_cast<void***>(static_cast<IUIMsgHandler*>(this)); \
    void** vtable3 = *reinterpret_cast<void***>(static_cast<ZRefCounted*>(this));

#define RESTORE_VTABLES(this) \
    *reinterpret_cast<void***>(this) = vtable1; \
    *reinterpret_cast<void***>(static_cast<IUIMsgHandler*>(this)) = vtable2; \
    *reinterpret_cast<void***>(static_cast<ZRefCounted*>(this)) = vtable3;


struct CRTTI {
    const CRTTI* m_pPrev;
};

class IGObj {
public:
    virtual void Update() = 0;
};
static_assert(sizeof(IGObj) == 0x4);

class IUIMsgHandler {
public:
    virtual void OnKey(unsigned int wParam, unsigned int lParam) {}
    virtual int OnSetFocus(int bFocus) { return 0; }
    virtual void OnMouseButton(unsigned int msg, unsigned int wParam, int rx, int ry) {}
    virtual int OnMouseMove(int rx, int ry) { return 0; }
    virtual int OnMouseWheel(int rx, int ry, int nWheel) { return 0; }
    virtual void OnMouseEnter(int bEnter) {}
    virtual void OnDraggableMove(int nState, void* pObj, int rx, int ry) {}
    virtual void SetEnable(int bEnable) {}
    virtual int IsEnabled() { return 1; }
    virtual void SetShow(int bShow) {}
    virtual int IsShown() { return 1; }
    virtual int GetAbsLeft() { return 0; }
    virtual int GetAbsTop() { return 0; }
    virtual void ClearToolTip() {}
    virtual void OnIMEModeChange(char cMode) {}
    virtual void OnIMEResult(const char* sComp) {}
    virtual void OnIMEComp(const char* sComp, void* adwCls, unsigned int nClsIdx, int nCursor, void* lCand, int nBegin, int nPage, int nCur) {}
    virtual const CRTTI* GetRTTI() = 0;
    virtual int IsKindOf(const CRTTI* pRTTI) = 0;
};
static_assert(sizeof(IUIMsgHandler) == 0x4);

class ZRefCounted {
public:
    union {
        volatile long m_nRef;
        ZRefCounted* m_pNext;
    };
    ZRefCounted* m_pPrev;

    virtual ~ZRefCounted() = default;
};
static_assert(sizeof(ZRefCounted) == 0xC);


class CWnd : public IGObj, public IUIMsgHandler, public ZRefCounted {
public:
    static inline const CRTTI* ms_pRTTI_CWnd = reinterpret_cast<const CRTTI*>(0x00C6194C);
    unsigned char gap0[0x80 - sizeof(IGObj) - sizeof(IUIMsgHandler) - sizeof(ZRefCounted)];

    /*** IGObj methods ***/
    virtual void Update() override {}

    /*** CWnd methods ***/
    virtual int OnDragDrop(int nState, void* ctx, int rx, int ry) { return 0; }
    virtual void PreCreateWnd(int l, int t, unsigned int w, unsigned int h, int z, int bScreenCoord, void* pData) {
        reinterpret_cast<void (__thiscall*)(CWnd*, int, int, unsigned int, unsigned int, int, int, void*)>(0x009ADDC0)(this, l, t, w, h, z, bScreenCoord, pData);
    }
    virtual void OnCreate(void* pData) {}
    virtual void OnDestroy() {}
    virtual void OnMoveWnd(int l, int t) { reinterpret_cast<void (__thiscall*)(CWnd*, int, int)>(0x009AEE10)(this, l, t); }
    virtual void OnEndMoveWnd() { reinterpret_cast<void (__thiscall*)(CWnd*)>(0x009AECA0)(this); }
    virtual void OnChildNotify(unsigned int nId, unsigned int param1, unsigned int param2) { reinterpret_cast<void (__thiscall*)(CWnd*, unsigned int, unsigned int, unsigned int)>(0x00429260)(this, nId, param1, param2); }
    virtual void OnButtonClicked(unsigned int nId) {}
    virtual int HitTest(int rx, int ry, void** ppCtrl) { return reinterpret_cast<int (__thiscall*)(CWnd*, int, int, void**)>(0x009AE3B0)(this, rx, ry, ppCtrl); }
    virtual int OnActivate(int bActive) { return reinterpret_cast<int (__thiscall*)(CWnd*, int)>(0x009AD350)(this, bActive); }
    virtual void Draw(const tagRECT* pRect) { reinterpret_cast<void (__thiscall*)(CWnd*, const tagRECT* pRect)>(0x009AE5C0)(this, pRect); }
    virtual int IsMyAddon(CWnd* pWnd) { return 0; }

    /*** IUIMsgHandler methods ***/
    virtual void OnKey(unsigned int wParam, unsigned int lParam) override {}
    virtual int OnSetFocus(int bFocus) override { return reinterpret_cast<int (__thiscall*)(IUIMsgHandler*, int)>(0x009AD3A0)(this, bFocus); }
    virtual void OnMouseButton(unsigned int msg, unsigned int wParam, int rx, int ry) override {}
    virtual int OnMouseMove(int rx, int ry) override { return 0; }
    virtual int OnMouseWheel(int rx, int ry, int nWheel) override { return reinterpret_cast<int (__thiscall*)(IUIMsgHandler*, int, int, int)>(0x009AE4D0)(this, rx, ry, nWheel); }
    virtual void OnMouseEnter(int bEnter) override { reinterpret_cast<void (__thiscall*)(IUIMsgHandler*, int)>(0x009AD370)(this, bEnter); }
    virtual int GetAbsLeft() override { return reinterpret_cast<int (__thiscall*)(IUIMsgHandler*)>(0x009AD480)(this); }
    virtual int GetAbsTop() override { return reinterpret_cast<int (__thiscall*)(IUIMsgHandler*)>(0x009AD570)(this); }
};
static_assert(sizeof(CWnd) == 0x80);


class CUIWnd : public CWnd {
public:
    static inline const CRTTI* ms_pRTTI_CUIWnd = reinterpret_cast<const CRTTI*>(0x00C618E8);
    unsigned char padding[0xB08 - sizeof(CWnd)];

    /*** CWnd methods ***/
    virtual void OnCreate(void* pData) override { reinterpret_cast<void (__thiscall*)(CUIWnd*, void*)>(0x008DDA90)(this, pData); }
    virtual void OnDestroy() override { reinterpret_cast<void (__thiscall*)(CUIWnd*)>(0x008DD380)(this); }
    virtual void OnButtonClicked(unsigned int nId) override { reinterpret_cast<void (__thiscall*)(CUIWnd*, unsigned int)>(0x008DD520)(this, nId); }
    virtual int HitTest(int rx, int ry, void** ppCtrl) override { return reinterpret_cast<int (__thiscall*)(CUIWnd*, int, int, void**)>(0x008DD2C0)(this, rx, ry, ppCtrl); }

    /*** CUIWnd methods ***/
    virtual void OnCreate(void* pData, void* sUOL, int bMultiBg) { return reinterpret_cast<void (__thiscall*)(CUIWnd*, void*, void*, int)>(0x008DDB30)(this, pData, sUOL, bMultiBg); }
    virtual void SetScreenMode(bool bIsLarge, bool bNeedInvalidateTabs) { return reinterpret_cast<void (__thiscall*)(CUIWnd*, bool, bool)>(0x008DD570)(this, bIsLarge, bNeedInvalidateTabs); }

    /*** IUIMsgHandler methods ***/
    virtual void OnKey(unsigned int wParam, unsigned int lParam) override { reinterpret_cast<void (__thiscall*)(IUIMsgHandler*, unsigned int, unsigned int)>(0x008DD550)(this, wParam, lParam); }
    virtual int OnSetFocus(int bFocus) override { return 0; }
    virtual void OnMouseEnter(int bEnter) override { reinterpret_cast<void (__thiscall*)(IUIMsgHandler*, int)>(0x008DD2A0)(this, bEnter); }
    virtual void ClearToolTip() override { reinterpret_cast<void (__thiscall*)(IUIMsgHandler*)>(0x004ABE20)(this); }
};
static_assert(sizeof(CUIWnd) == 0xB08);


class CUIItemBT : public CUIWnd {
public:
    static inline const CRTTI ms_RTTI_CUIItemBT = { CUIWnd::ms_pRTTI_CUIWnd };

    virtual void OnCreate(void* pData) override {
        ZXString<wchar_t> sBackgroundUOL;
        // ZXString<wchar_t>::ZXString<wchar_t>(&sBackgroundUOL, L"", -1);
        reinterpret_cast<void (__thiscall*)(ZXString<wchar_t>*, const wchar_t*, int)>(0x00416D00)(&sBackgroundUOL, L"UI/UIWindowBT.img/CharacterUI/Item/FullBackgrnd", -1);
        // CUIWnd::OnCreate(this, pData, sBackgroundUOL, 1);
        reinterpret_cast<void (__thiscall*)(CUIWnd*, void*, ZXString<wchar_t>, int)>(0x008DDB30)(this, pData, sBackgroundUOL, 1);
    }
    virtual const CRTTI* GetRTTI() override {
        return &ms_RTTI_CUIItemBT;
    }
    virtual int IsKindOf(const CRTTI* pRTTI) override {
        CRTTI result = { &ms_RTTI_CUIItemBT };
        while (result.m_pPrev != pRTTI) {
            result.m_pPrev = result.m_pPrev->m_pPrev;
            if (!result.m_pPrev) {
                return 0;
            }
        }
        return 1;
    }
    virtual ~CUIItemBT() override {
        // CUIWnd::~CUIWnd(this);
        reinterpret_cast<void (__thiscall*)(CUIWnd*)>(0x008DD780)(this);
    }

    CUIItemBT() {
        SAVE_VTABLES(this);
        // CUIWnd::CUIWnd(this, 0, 5, 299, 6, 1, 0, 0);
        reinterpret_cast<void (__thiscall*)(CUIWnd*, int, int, int, int, int, int, int)>(0x008DD680)(this, 0, 5, 299, 6, 1, 0, 0);
        RESTORE_VTABLES(this);

        // CUIWnd::CreateUIWndPosSaved(this, 321, 249, 10);
        reinterpret_cast<void (__thiscall*)(CUIWnd*, int, int, int)>(0x008DD300)(this, 321, 249, 10);
    }
};


static auto CWvsContext__UI_Open = reinterpret_cast<void (__thiscall*)(void*, unsigned int, int)>(0x009D83F0);
void __fastcall CWvsContext__UI_Open_hook(void* _ECX, void* _EDX, unsigned int nUIType, int nDefaultTab) {
    if (nUIType == 0) {
        ZRef<CUIItemBT>* pUIItem = reinterpret_cast<ZRef<CUIItemBT>*>(reinterpret_cast<uintptr_t>(_ECX) + 0x3E30);
        if (!pUIItem->p) {
            CUIItemBT* p = reinterpret_cast<CUIItemBT*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(sizeof(CUIItemBT)));
            if (p) {
                new (p) CUIItemBT();
            }
            pUIItem->p = p;
            InterlockedIncrement(&pUIItem->p->m_nRef);
        }
    }
    CWvsContext__UI_Open(_ECX, nUIType, nDefaultTab);
}


static auto CWvsContext__UI_Close = reinterpret_cast<void (__thiscall*)(void*, unsigned int)>(0x009D5370);
void __fastcall CWvsContext__UI_Close_hook(void* _ECX, void* _EDX, unsigned int nUIType) {
    if (nUIType == 0) {
        ZRef<CUIItemBT>* pUIItem = reinterpret_cast<ZRef<CUIItemBT>*>(reinterpret_cast<uintptr_t>(_ECX) + 0x3E30);
        if (pUIItem->p) {
            // CWnd::Destroy(pUIItem->p);
            reinterpret_cast<void (__thiscall*)(void*)>(0x009B0E50)(pUIItem->p);
            // ZRef<>::operator=(&pUIItem, nullptr);
            if (!InterlockedDecrement(&pUIItem->p->m_nRef)) {
                InterlockedIncrement(&pUIItem->p->m_nRef);
                pUIItem->p->~CUIItemBT();
                ZAllocEx<ZAllocAnonSelector>::s_Free(pUIItem->p);
            }
            pUIItem->p = nullptr;
        }
    }
    CWvsContext__UI_Close(_ECX, nUIType);
}


void AttachCustomUIWindowMod() {
    ATTACH_HOOK(CWvsContext__UI_Open, CWvsContext__UI_Open_hook);
    ATTACH_HOOK(CWvsContext__UI_Close, CWvsContext__UI_Close_hook);
}