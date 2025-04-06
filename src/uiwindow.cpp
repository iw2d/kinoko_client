#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


struct CRTTI {
    const CRTTI* m_pPrev;

    int IsKindOf(const CRTTI* pRTTI) const {
        CRTTI result = { this };
        while (result.m_pPrev != pRTTI) {
            result.m_pPrev = result.m_pPrev->m_pPrev;
            if (!result.m_pPrev) {
                return 0;
            }
        }
        return 1;
    }
};

class IGObj {
public:
    virtual void Update() = 0;

    static void* operator new(size_t size) {
        return ZAllocEx<ZAllocAnonSelector>::s_Alloc(size);
    }
    static void operator delete(void* ptr) {
        ZAllocEx<ZAllocAnonSelector>::s_Free(ptr);
    }
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


class CWnd;


class CCtrlWnd : public IGObj, public IUIMsgHandler, public ZRefCounted {
public:
    unsigned char gap0[0x34 - sizeof(IGObj) - sizeof(IUIMsgHandler) - sizeof(ZRefCounted)];

    /*** IGObj methods ***/
    virtual void Update() override {}

    /*** CCtrlWnd methods ***/
    virtual int OnDragDrop(int nState, void* ctx, int rx, int ry) { return 0; }
    virtual void CreateCtrl(CWnd*, unsigned int nId, int l, int t, int w, int h, void* pData) {}
    virtual void Destroy() {}
    virtual void OnCreate(void* pData) {}
    virtual void OnDestroy() {}
    virtual int HitTest(int rx, int ry) { return 0; }
    virtual tagRECT* GetRect(tagRECT* result) { return result; }
    virtual void SetAbove(CCtrlWnd* pCtrl) {}
    virtual void Draw(int rx, int ry, const tagRECT* pRect) {}

    /*** IUIMsgHandler methods ***/
    virtual const CRTTI* GetRTTI() { return 0; }
    virtual int IsKindOf(const CRTTI* pRTTI) { return 0; }
};
static_assert(sizeof(CCtrlWnd) == 0x34);

class CCtrlScrollBar : public CCtrlWnd {
public:
    unsigned char gap0[0x74 - sizeof(CCtrlWnd)];

    virtual void CreateCtrl(CWnd* pParent, unsigned int nId, int hv, int type, int l, int t, int length, void* pData) {}

    CCtrlScrollBar() {
        reinterpret_cast<void (__thiscall*)(CCtrlScrollBar*)>(0x004E9FC0)(this);
    }
};
static_assert(sizeof(CCtrlScrollBar) == 0x74);

class CCtrlTab : public CCtrlWnd {
public:
    struct CREATEPARAM {
        int bDrawBaseImage;
        int nTabSpace;
    };
    static_assert(sizeof(CREATEPARAM) == 0x8);

    unsigned char gap0[0x70 - sizeof(CCtrlWnd)];

    virtual void CreateCtrl(CWnd* pParent, unsigned int nId, int nType, int l, int t, int w, int h, void* pData) {}

    CCtrlTab() {
        reinterpret_cast<void (__thiscall*)(CCtrlTab*)>(0x004EE590)(this);
    }
};
static_assert(sizeof(CCtrlTab) == 0x70);

class CCtrlButton : public CCtrlWnd {
public:
    struct CREATEPARAM {
        int bAcceptFocus;
        int bDrawBack;
        int bAnimateOnce;
        ZXString<wchar_t> sUOL;
    };
    static_assert(sizeof(CREATEPARAM) == 0x10);

    unsigned char gap0[0xADC - sizeof(CCtrlWnd)];

    virtual void CreateCtrl(CWnd* pParent, unsigned int nId, int l, int t, int decClickArea, void* pData) {}

    CCtrlButton() { reinterpret_cast<void (__thiscall*)(CCtrlButton*)>(0x00471740)(this); }
    CCtrlButton(int nDummy) {}
};
static_assert(sizeof(CCtrlButton) == 0xADC);

class CCtrlOriginButton : public CCtrlButton {
public:

};


class CLayoutMan {
public:
    CWnd* m_pWnd;
    int m_nOffsetX;
    int m_nOffsetY;
    ZArray<IWzGr2DLayerPtr> m_aLayer;
    ZArray<ZRef<CCtrlOriginButton>> m_aButton;

    void Init(CWnd* pWnd, int nOffsetX, int nOffsetY) {
        this->m_pWnd = pWnd;
        this->m_nOffsetX = nOffsetX;
        this->m_nOffsetY = nOffsetY;
    }

    ZRef<CCtrlOriginButton>& AddButton(ZRef<CCtrlOriginButton>& result, const wchar_t* sButtonUOL, unsigned int nID, int nOffsetX, int nOffsetY, CCtrlButton::CREATEPARAM* pParam) {
        return reinterpret_cast<ZRef<CCtrlOriginButton>& (__thiscall*)(CLayoutMan*, ZRef<CCtrlOriginButton>&, const wchar_t*, unsigned int, int, int, CCtrlButton::CREATEPARAM*)>(0x005CF210)(this, result, sButtonUOL, nID, nOffsetX, nOffsetY, pParam);
    }
};


class CUIToolTip {
public:
    unsigned char gap0[0xA48];

    virtual ~CUIToolTip() = default;
};


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

    /*** ZRefCounted methods ***/
    virtual ~CWnd() override {
        reinterpret_cast<void (__thiscall*)(CWnd*)>(0x009AEBC0)(this);
    }

    CWnd() { reinterpret_cast<void (__thiscall*)(CWnd*)>(0x009AED30)(this); }
    CWnd(int nDummy) { /* dummy constructor to avoid calling CWnd constructor twice in CUIWnd constructor */ }

    IWzCanvasPtr& GetCanvas(IWzCanvasPtr& result) {
        return reinterpret_cast<IWzCanvasPtr& (__thiscall*)(CWnd*, IWzCanvasPtr&)>(0x0042B170)(this, result);
    }
    void InvalidateRect(const tagRECT* pRect) {
        reinterpret_cast<void (__thiscall*)(CWnd*, const tagRECT*)>(0x009AD3F0)(this, pRect);
    }
};
static_assert(sizeof(CWnd) == 0x80);


class CUIWnd : public CWnd {
public:
    static inline const CRTTI* ms_pRTTI_CUIWnd = reinterpret_cast<const CRTTI*>(0x00C618E8);
    unsigned char gap0[0xB08 - sizeof(CWnd)];

    MEMBER_AT(ZRef<CCtrlButton>, 0x80, m_pBtClose)
    MEMBER_AT(CUIToolTip, 0x88, m_uiToolTip)
    MEMBER_AT(ZArray<unsigned char>, 0xB00, m_abOption)
    MEMBER_AT(ZXString<wchar_t>, 0xB04, m_sBackgrndUOL)
    MEMBER_AT(int, 0xAFC, m_nOption)

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

    /*** ZRefCounted methods ***/
    virtual ~CUIWnd() override {
        // implement CUIWnd destructor so we dont call CWnd destructor twice
        reinterpret_cast<void (__thiscall*)(ZXString<wchar_t>*)>(0x00403920)(&this->m_sBackgrndUOL());  // ZXString<wchar_t>::~ZXString<wchar_t>(&this->m_sBackgrndUOL());
        reinterpret_cast<void (__thiscall*)(ZArray<unsigned char>*)>(0x0042AAE0)(&this->m_abOption());  // ZArray<unsigned char>::RemoveAll(&this->m_abOption());
        this->m_uiToolTip().~CUIToolTip();
        this->m_pBtClose() = nullptr;
    }

    CUIWnd() = delete;
    CUIWnd(int nUIType, int closeType, int closeX, int closeY, int bBackgrnd, int nBackgrndX, int nBackgrndY) : CWnd(0) {
        reinterpret_cast<void (__thiscall*)(CUIWnd*, int, int, int, int, int, int, int)>(0x008DD680)(this, nUIType, closeType, closeX, closeY, bBackgrnd, nBackgrndX, nBackgrndY);
    }
    CUIWnd(int nUIType, int closeType, int closeX, int closeY, const wchar_t* sBackgrndUOL, int nBackgrndX, int nBackgrndY, int bMultiBg) : CWnd(0) {
        reinterpret_cast<void (__thiscall*)(CUIWnd*, int, int, int, int, const wchar_t*, int, int, int)>(0x008DD980)(this, nUIType, closeType, closeX, closeY, sBackgrndUOL, nBackgrndX, nBackgrndY, bMultiBg);
    }
};
static_assert(sizeof(CUIWnd) == 0xB08);


class CUIItemBT : public CUIWnd {
public:
    static inline const CRTTI ms_RTTI_CUIItemBT = { CUIWnd::ms_pRTTI_CUIWnd };
    ZRef<CCtrlTab> m_pTab;
    ZRef<CCtrlScrollBar> m_pSBItem;
    ZRef<CCtrlButton> m_pBtCoin;
    int m_nFirstPositon;
    int m_nItemTI;
    CLayoutMan m_lm;

    virtual void OnCreate(void* pData) override {
        // CUIWnd::OnCreate(this, pData, sBackgroundUOL, 1);
        reinterpret_cast<void (__thiscall*)(CUIWnd*, void*, ZXString<wchar_t>, int)>(0x008DDB30)(this, pData, ZXString<wchar_t>(), 1);

        this->m_lm.Init(this, 0, 0);

        CCtrlTab::CREATEPARAM paramTab;
        paramTab.bDrawBaseImage = 0;
        paramTab.nTabSpace = 1;
        this->m_pTab = new CCtrlTab();
        this->m_pTab->CreateCtrl(this, 2000, 8, 13, 28, 214, 22, &paramTab);
        Ztl_variant_t vEmpty;
        Ztl_variant_t vTabEnabled;
        Ztl_variant_t vTabDisabled;
        CHECK_HRESULT(get_rm()->raw_GetObject(L"UI/UIWindowBT.img/CharacterUI/Item/Tab/enabled", vEmpty, vEmpty, &vTabEnabled));
        CHECK_HRESULT(get_rm()->raw_GetObject(L"UI/UIWindowBT.img/CharacterUI/Item/Tab/disabled", vEmpty, vEmpty, &vTabDisabled));
        IWzPropertyPtr pTabEnabled = IWzPropertyPtr(vTabEnabled.GetUnknown(false, false));
        IWzPropertyPtr pTabDisabled = IWzPropertyPtr(vTabDisabled.GetUnknown(false, false));
        for (int i = 0; i < 5; i++) {
            wchar_t sIndex[10];
            swprintf_s(sIndex, 10, L"%d", i);
            Ztl_variant_t vCanvasEnabled;
            Ztl_variant_t vCanvasDisabled;
            CHECK_HRESULT(pTabEnabled->get_item(sIndex, &vCanvasEnabled));
            CHECK_HRESULT(pTabDisabled->get_item(sIndex, &vCanvasDisabled));
            IUnknownPtr pCanvasEnabledUnknown;
            IUnknownPtr pCanvasDisabledUnknown;
            // get_unknown(std::addressof(pCanvasEnabledUnknown), &vCanvasEnabled);
            reinterpret_cast<IUnknownPtr* (__cdecl*)(IUnknownPtr*, Ztl_variant_t*)>(0x004176E0)(std::addressof(pCanvasEnabledUnknown), &vCanvasEnabled);
            reinterpret_cast<IUnknownPtr* (__cdecl*)(IUnknownPtr*, Ztl_variant_t*)>(0x004176E0)(std::addressof(pCanvasDisabledUnknown), &vCanvasDisabled);
            IWzCanvasPtr pCanvasEnabled(pCanvasEnabledUnknown);
            IWzCanvasPtr pCanvasDisabled(pCanvasDisabledUnknown);
            // CCtrlTab::AddItem_Canvas(this->m_pTab, pCanvasDisabled, pCanvasEnabled, 1);
            reinterpret_cast<void (__thiscall*)(CCtrlTab*, IWzCanvasPtr, IWzCanvasPtr, int)>(0x004EFC40)(this->m_pTab, pCanvasDisabled, pCanvasEnabled, 1);
        }

        this->m_pSBItem = new CCtrlScrollBar();
        this->m_pSBItem->CreateCtrl(this, 2001, 1, 8, 301, 51, 138, nullptr);

        this->m_lm.AddButton(ZRef<CCtrlOriginButton>(), L"UI/UIWindowBT.img/CharacterUI/Item/BtCoin", 2002, 0, 0, nullptr);
        this->m_lm.AddButton(ZRef<CCtrlOriginButton>(), L"UI/UIWindowBT.img/CharacterUI/Item/BtPoint0", 2003, 0, 0, nullptr);


        // CCtrlTab::SetTab(this->m_pTab, this->m_nOption());
        reinterpret_cast<void (__thiscall*)(CCtrlTab*, int)>(0x004EDE60)(this->m_pTab, this->m_nOption());
    }
    virtual void OnChildNotify(unsigned int nId, unsigned int param1, unsigned int param2) override {
        DEBUG_MESSAGE("OnChildNotify %d %d %d", nId, param1, param2);
        if (nId == 2000) {
            if (param1 == 500) {
                this->m_nOption() = param2;
                if (param2 == 3) {
                    this->m_nItemTI = 4;
                } else if (param2 == 4) {
                    this->m_nItemTI = 3;
                } else {
                    this->m_nItemTI = param2;
                }
                this->m_nFirstPositon = 1; // TODO: static array ms-anItemScrollPos to save scroll position
                this->InvalidateRect(nullptr);
            }
        }
        if (param1 == 100) {
            this->OnButtonClicked(nId);
        }
    }
    virtual void Draw(const tagRECT* pRect) override {
        CWnd::Draw(pRect);
        IWzCanvasPtr pCanvas;
        this->GetCanvas(pCanvas);

        IWzFontPtr pMoneyFont;
        // get_basic_font(pMoneyFont, FONT_NO_BLACK_SMALL);
        reinterpret_cast<IWzFontPtr& (__cdecl*)(IWzFontPtr&, int)>(0x0095F9D0)(pMoneyFont, 0x39);
        int nMoney = CWvsContext::GetInstance()->m_pCharacterData()->characterStat().nMoney().Fuse();
        ZXString<char> sMoney;
        // format_integer(&sMoney, nMoney, 1);
        reinterpret_cast<void (__cdecl*)(ZXString<char>*, int, int)>(0x009658D0)(&sMoney, nMoney, 1);
        Ztl_bstr_t lsMoney(sMoney._m_pStr);
        Ztl_variant_t vEmpty;
        unsigned int uTextWidth;
        CHECK_HRESULT(pMoneyFont->raw_CalcTextWidth(lsMoney.m_Data->m_wstr, vEmpty, &uTextWidth));
        unsigned int result[4];
        CHECK_HRESULT(pCanvas->raw_DrawText(103 - uTextWidth, 199, lsMoney.m_Data->m_wstr, pMoneyFont, vEmpty, vEmpty, result));

        ZArray<ZRef<GW_ItemSlotBase>>& aItemSlot = (&CWvsContext::GetInstance()->m_pCharacterData()->aaItemSlot())[this->m_nItemTI];
        int nItemSlot = reinterpret_cast<int*>(aItemSlot.a)[-1];
        for (int i = 0; i < 32; ++i) {
            int nPos = this->m_nFirstPositon + i;
            if (nPos >= nItemSlot) {
                break;
            }
            ZRef<GW_ItemSlotBase> pItem = aItemSlot.a[nPos];
            if (!pItem) {
                continue;
            }

            int x = 11 + (i % 8) * 36;
            int y = 85 + (i / 8) * 35;

            // TSecType<long>::GetData(&pItem->nItemID());
            int nItemID = reinterpret_cast<int (__thiscall*)(TSecType<long>*)>(0x0042B750)(&pItem->nItemID());
            CItemInfo::GetInstance()->DrawItemIconForSlot(pCanvas, nItemID, x, y, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    virtual const CRTTI* GetRTTI() override {
        return &ms_RTTI_CUIItemBT;
    }
    virtual int IsKindOf(const CRTTI* pRTTI) override {
        return ms_RTTI_CUIItemBT.IsKindOf(pRTTI);
    }
    virtual ~CUIItemBT() override = default;

    CUIItemBT() : CUIWnd(0, 5, 299, 6, L"UI/UIWindowBT.img/CharacterUI/Item/FullBackgrnd", 0, 0, 1) {
        // CUIWnd::CreateUIWndPosSaved(this, 321, 249, 10);
        reinterpret_cast<void (__thiscall*)(CUIWnd*, int, int, int)>(0x008DD300)(this, 321, 249, 10);
    }
};


static auto CWvsContext__UI_Open = reinterpret_cast<void (__thiscall*)(void*, unsigned int, int)>(0x009D83F0);
void __fastcall CWvsContext__UI_Open_hook(void* _ECX, void* _EDX, unsigned int nUIType, int nDefaultTab) {
    if (nUIType == 0) {
        ZRef<CUIItemBT>* pUIItem = reinterpret_cast<ZRef<CUIItemBT>*>(reinterpret_cast<uintptr_t>(_ECX) + 0x3E30);
        if (!pUIItem->p) {
            *pUIItem = new CUIItemBT();
        }
        return;
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
            *pUIItem = nullptr;
        }
        return;
    }
    CWvsContext__UI_Close(_ECX, nUIType);
}


void AttachCustomUIWindowMod() {
    ATTACH_HOOK(CWvsContext__UI_Open, CWvsContext__UI_Open_hook);
    ATTACH_HOOK(CWvsContext__UI_Close, CWvsContext__UI_Close_hook);
}