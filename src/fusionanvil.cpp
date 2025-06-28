#include "pch.h"
#include "hook.h"
#include "config.h"
#include "ztl/zalloc.h"
#include "ztl/zcom.h"
#include "wzlib/pcom.h"
#include "wzlib/gr2d.h"
#include "wzlib/canvas.h"
#include "common/rtti.h"
#include "common/dbbasic.h"
#include "common/iteminfo.h"
#include "wvs/util.h"
#include "wvs/dialog.h"
#include "wvs/ctrlwnd.h"
#include "wvs/uitooltip.h"
#include "wvs/layoutman.h"
#include "wvs/wvscontext.h"
#include <windows.h>
#include <cstdint>


class CDraggableItem : public IDraggable {
public:
    MEMBER_AT(int32_t, 0x18, m_nItemTI)
    MEMBER_AT(int32_t, 0x1C, m_nSlotPosition)
    MEMBER_AT(int32_t, 0x20, m_nIdx)
    MEMBER_AT(IUIMsgHandler*, 0x24, m_pSource)
};


class CUIFusionAnvil : public CUniqueModeless {
public:
    inline static CRTTI ms_RTTI_CUIFusionAnvil = CRTTI(&CUniqueModeless::ms_RTTI_CUniqueModeless);

    ZRef<CCtrlButton> m_pBtOK;
    ZRef<CCtrlButton> m_pBtCancel;
    ZRef<CCtrlButton> m_pBtClose;
    IWzGr2DLayerPtr m_pLayerEffect;
    IWzGr2DLayerPtr m_pLayerItem[2];
    IWzCanvasPtr m_pSubTitleCanvas;
    GW_ItemSlotBase* m_apChangeItem[2] = {};
    int32_t m_anChangeItemPos[2] = {};
    tagRECT m_arcToolTip[2] = {};
    CUIToolTip m_uiToolTip;
    CLayoutMan m_lm;

    CUIFusionAnvil() {
        CreateDlgOnCenter(L"UI/UIWindow0.img/Synthesizing/backgrnd");
    }
    virtual ~CUIFusionAnvil() override = default;
    virtual void OnCreate(void* pData) override {
        m_lm.Init(this, 0, 0);

        CCtrlButton::CREATEPARAM paramButton;
        paramButton.bAcceptFocus = 1;
        m_pBtOK = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtOk", 1, 0, 0, &paramButton);
        m_pBtCancel = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtCancel", 2, 0, 0, &paramButton);
        m_pBtClose = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtExit", 8, 0, 0, &paramButton);
        m_pLayerEffect = m_lm.AddLayer(L"UI/UIWindow0.img/Synthesizing/Effect", 1, 1);
        CHECK_HR(m_pLayerEffect->raw_Animate(GA_REPEAT, vtEmpty, vtEmpty));

        Ztl_variant_t vSubTitle;
        CHECK_HR(get_rm()->raw_GetObject(L"UI/UIWindow0.img/Synthesizing/SubTitle", vtEmpty, vtEmpty, &vSubTitle));
        m_pSubTitleCanvas = get_unknown(vSubTitle);

        for (auto i = 0; i < 2; ++i) {
            Ztl_variant_t vCanvas(0, VT_I4);
            CHECK_HR(get_gr()->raw_CreateLayer(0, 0, 0, 0, 0, vCanvas, vtEmpty, &m_pLayerItem[i]));
            Ztl_variant_t vOrigin(static_cast<IUnknown*>(GetLayer()), true);
            CHECK_HR(m_pLayerItem[i]->put_origin(vOrigin));
            CHECK_HR(m_pLayerItem[i]->put_color(0xFFFFFFFF));
            Ztl_variant_t vOverlay(static_cast<IUnknown*>(m_pLayerEffect), true);
            CHECK_HR(m_pLayerItem[i]->put_overlay(vOverlay));
            CHECK_HR(m_pLayerItem[i]->put_z(2));
        }
        CHECK_HR(m_pLayerItem[0]->raw_RelMove(59, 119, vtEmpty, vtEmpty));
        CHECK_HR(m_pLayerItem[1]->raw_RelMove(128, 119, vtEmpty, vtEmpty));

        SetRect(&m_arcToolTip[0], 43, 103, 75, 135);
        SetRect(&m_arcToolTip[1], 112, 103, 144, 135);
    }
    virtual void Draw(const tagRECT* pRect) {
        CWnd::Draw(pRect);
        if (m_pSubTitleCanvas) {
            IWzCanvasPtr pCanvas = GetCanvas();
            m_lm.CopyToCanvas(pCanvas, m_pSubTitleCanvas, 0, 0);
        }
        for (auto i = 0; i < 2; ++i) {
            if (!m_apChangeItem[i]) {
                continue;
            }
            int32_t nItemID = m_apChangeItem[i]->nItemID.GetData();
            Ztl_variant_t vIndex(-2, VT_I4);
            IWzCanvasPtr pOldCanvas;
            CHECK_HR(m_pLayerItem[i]->raw_RemoveCanvas(vIndex, &pOldCanvas));

            IWzCanvasPtr pNewCanvas;
            PcCreateObject<IWzCanvasPtr>(L"Canvas", pNewCanvas, nullptr);
            CHECK_HR(pNewCanvas->raw_Create(32, 32, vtEmpty, vtEmpty));
            CItemInfo::GetInstance()->DrawItemIconForSlot(pNewCanvas, nItemID, 0, 32, 0, 0, 0, 0, 0, 0, 0);

            Ztl_variant_t vResult;
            CHECK_HR(m_pLayerItem[i]->raw_InsertCanvas(pNewCanvas, vtEmpty, vtEmpty, vtEmpty, vtEmpty, vtEmpty, &vResult));
        }
    }
    virtual void OnButtonClicked(uint32_t nId) override {
        switch (nId) {
        case 1:
            // TODO
            SetRet(1);
            break;
        case 2:
            SetRet(2);
            break;
        case 8:
            SetRet(8);
            break;
        }
    }
    virtual int32_t OnMouseMove(int32_t rx, int32_t ry) {
        POINT pt{rx, ry};
        for (auto i = 0; i < 2; ++i) {
            if (m_apChangeItem[i] && PtInRect(&m_arcToolTip[i], pt)) {
                m_uiToolTip.ShowItemToolTip(rx + GetAbsLeft(), ry + GetAbsTop(), m_apChangeItem[i], nullptr, nullptr, 0, 0, 0);
                return 0;
            }
        }
        m_uiToolTip.ClearToolTip();
        return 0;
    }
    virtual const CRTTI* GetRTTI() const override {
        return &ms_RTTI_CUIFusionAnvil;
    }
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const override {
        return ms_RTTI_CUIFusionAnvil.IsKindOf(pRTTI);
    }

    int32_t PutItem(GW_ItemSlotBase* pItem, int32_t nTI, int32_t nSlotPosition, int32_t x, int32_t y) {
        int32_t cx;
        CHECK_HR(GetCanvas()->get_cx(&cx));
        int32_t nIndex = x < cx ? 0 : 1;
        m_apChangeItem[nIndex] = pItem;
        m_anChangeItemPos[nIndex] = nSlotPosition;
        InvalidateRect(nullptr);
        return 0;
    }
};


static auto CWvsContext__SendConsumeCashItemUseRequest = reinterpret_cast<void(__thiscall*)(CWvsContext*, int32_t, int32_t, int32_t, ZXString<char>)>(0x009EB3E0);

void __fastcall CWvsContext__SendConsumeCashItemUseRequest_hook(CWvsContext* pThis, void* _EDX, int32_t nPOS, int32_t nItemID, int32_t nEPOS, ZXString<char> sDefaultValue) {
    if (nItemID != 5062400) {
        CWvsContext__SendConsumeCashItemUseRequest(pThis, nPOS, nItemID, nEPOS, sDefaultValue);
        return;
    }
    DEBUG_MESSAGE("CWvsContext::SendConsumeCashItemUseRequest %d %d", nPOS, nItemID);
    if (CUniqueModeless::IsInstantiated()) {
        DEBUG_MESSAGE("Not available");
        return;
    }
    new CUIFusionAnvil();
}


static auto CDraggableItem__OnDropped = reinterpret_cast<int32_t(__thiscall*)(CDraggableItem*, IUIMsgHandler*, IUIMsgHandler*, int32_t, int32_t)>(0x00508700);
static auto CRTTI_ms_pRTTI_CUIItem = reinterpret_cast<CRTTI*>(0x00C616D0);

int32_t __fastcall CDraggableItem__OnDropped_hook(CDraggableItem* pThis, void* _EDX, IUIMsgHandler* pFrom, IUIMsgHandler* pTo, int32_t rx, int32_t ry) {
    if (pFrom->IsKindOf(CRTTI_ms_pRTTI_CUIItem) && pTo->IsKindOf(&CUIFusionAnvil::ms_RTTI_CUIFusionAnvil)) {
        ZRef<GW_ItemSlotBase> pItem = CWvsContext::GetInstance()->m_pCharacterData->GetItem(pThis->m_nItemTI, pThis->m_nSlotPosition);
        if (!pItem) {
            return 0;
        }
        return dynamic_cast<CUIFusionAnvil*>(pTo)->PutItem(pItem, pThis->m_nItemTI, pThis->m_nSlotPosition, rx, ry);
    }
    return CDraggableItem__OnDropped(pThis, pFrom, pTo, rx, ry);
}


void AttachFusionAnvilMod() {
    ATTACH_HOOK(CWvsContext__SendConsumeCashItemUseRequest, CWvsContext__SendConsumeCashItemUseRequest_hook);
    ATTACH_HOOK(CDraggableItem__OnDropped, CDraggableItem__OnDropped_hook);
}