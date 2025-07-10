#include "pch.h"
#include "hook.h"
#include "config.h"
#include "ztl/ztl.h"
#include "wzlib/pcom.h"
#include "common/rtti.h"
#include "common/dbbasic.h"
#include "common/iteminfo.h"
#include "common/packet.h"
#include "wvs/util.h"
#include "wvs/dialog.h"
#include "wvs/ctrlwnd.h"
#include "wvs/uitooltip.h"
#include "wvs/layoutman.h"
#include "wvs/wvscontext.h"
#include "wvs/wvsapp.h"
#include "wvs/clientsocket.h"
#include <windows.h>
#include <cstdint>


static auto get_weapon_type = reinterpret_cast<int32_t(__cdecl*)(int32_t nItemID)>(0x0046F660);
static auto get_bodypart_from_item = reinterpret_cast<int32_t(__cdecl*)(int32_t nItemID, int32_t nGender, int32_t* pnBodyPart, int32_t bAll)>(0x0046FBE0);


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

    int32_t m_nItemPOS;
    int32_t m_nItemID;
    ZRef<CCtrlButton> m_pBtOK;
    ZRef<CCtrlButton> m_pBtCancel;
    ZRef<CCtrlButton> m_pBtClose;
    IWzGr2DLayerPtr m_pLayerEffect;
    IWzGr2DLayerPtr m_pLayerItem[2];
    IWzCanvasPtr m_pBackgrndCanvas;
    IWzCanvasPtr m_pSubTitleCanvas;
    GW_ItemSlotBase* m_apChangeItem[2] = {};
    int32_t m_anChangeItemPos[2] = {};
    tagRECT m_arcToolTip[2] = {};
    CUIToolTip m_uiToolTip;
    CLayoutMan m_lm;

    CUIFusionAnvil(int32_t nItemPOS, int32_t nItemID) : m_nItemPOS(nItemPOS),
                                                        m_nItemID(nItemID) {
        CreateDlgOnCenter(L"UI/UIWindow0.img/Synthesizing/backgrnd");
    }
    virtual ~CUIFusionAnvil() override = default;
    virtual void OnCreate(void* pData) override {
        m_lm.Init(this, 0, 0);

        CCtrlButton::CREATEPARAM paramButton;
        paramButton.bAcceptFocus = 1;
        m_pBtOK = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtOk", 1, 0, 0, &paramButton);
        m_pBtOK->SetEnable(0);
        m_pBtCancel = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtCancel", 2, 0, 0, &paramButton);
        m_pBtClose = m_lm.AddButton(L"UI/UIWindow0.img/Synthesizing/BtExit", 8, 0, 0, &paramButton);
        m_pLayerEffect = m_lm.AddLayer(L"UI/UIWindow0.img/Synthesizing/Effect", 1, 1);
        m_pLayerEffect->Animate(GA_REPEAT, vtEmpty, vtEmpty);

        Ztl_variant_t vBackgrnd = get_rm()->GetObjectA(L"UI/UIWindow0.img/Synthesizing/backgrnd1", vtEmpty, vtEmpty);
        m_pBackgrndCanvas = get_unknown(vBackgrnd);
        Ztl_variant_t vSubTitle = get_rm()->GetObjectA(L"UI/UIWindow0.img/Synthesizing/SubTitle", vtEmpty, vtEmpty);
        m_pSubTitleCanvas = get_unknown(vSubTitle);

        for (auto i = 0; i < 2; ++i) {
            Ztl_variant_t vCanvas(0, VT_I4);
            m_pLayerItem[i] = get_gr()->CreateLayer(0, 0, 0, 0, 0, vCanvas, vtEmpty);
            Ztl_variant_t vOrigin(static_cast<IUnknown*>(GetLayer()), true);
            m_pLayerItem[i]->origin = vOrigin;
            m_pLayerItem[i]->color = 0xFFFFFFFF;
            Ztl_variant_t vOverlay(static_cast<IUnknown*>(m_pLayerEffect), true);
            m_pLayerItem[i]->overlay = vOverlay;
            m_pLayerItem[i]->z = 2;
        }
        m_pLayerItem[0]->RelMove(59, 119, vtEmpty, vtEmpty);
        m_pLayerItem[1]->RelMove(128, 119, vtEmpty, vtEmpty);

        SetRect(&m_arcToolTip[0], 43, 103, 75, 135);
        SetRect(&m_arcToolTip[1], 112, 103, 144, 135);
    }
    virtual void Draw(const tagRECT* pRect) {
        CWnd::Draw(pRect);
        IWzCanvasPtr pCanvas = GetCanvas();
        if (m_pBackgrndCanvas) {
            m_lm.CopyToCanvas(pCanvas, m_pBackgrndCanvas, 0, 0);
        }
        if (m_pSubTitleCanvas) {
            m_lm.CopyToCanvas(pCanvas, m_pSubTitleCanvas, 0, 0);
        }
        for (auto i = 0; i < 2; ++i) {
            if (!m_apChangeItem[i]) {
                continue;
            }
            int32_t nItemID = m_apChangeItem[i]->nItemID.GetData();
            Ztl_variant_t vIndex(-2, VT_I4);
            m_pLayerItem[i]->RemoveCanvas(vIndex);

            IWzCanvasPtr pNewCanvas;
            PcCreateObject<IWzCanvasPtr>(L"Canvas", pNewCanvas, nullptr);
            pNewCanvas->Create(32, 32, vtEmpty, vtEmpty);
            CItemInfo::GetInstance()->DrawItemIconForSlot(pNewCanvas, nItemID, 0, 32, 0, 0, 0, 0, 0, 0, 0);

            m_pLayerItem[i]->InsertCanvas(pNewCanvas, vtEmpty, vtEmpty, vtEmpty, vtEmpty, vtEmpty);
        }
    }
    virtual void OnButtonClicked(uint32_t nId) override {
        switch (nId) {
        case 1:
            if (!m_apChangeItem[0] || !m_apChangeItem[1]) {
                return;
            }
            SendPacket();
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
        if (pItem->IsCashItem()) {
            Notice("You cannot use the fusion anvil on cash items.");
            return 0;
        }
        int32_t nItemID = pItem->nItemID.GetData();
        int32_t nBodyPart;
        if (!get_bodypart_from_item(nItemID, 2, &nBodyPart, 0)) {
            Notice("You cannot use the fusion anvil on this item.");
            return 0;
        }
        int32_t nWeaponType = get_weapon_type(nItemID);

        int32_t nIndex = x < GetCanvas()->cx ? 0 : 1;
        int32_t nIndex2 = nIndex ? 0 : 1;
        DEBUG_MESSAGE("CUIFusionAnvil::PutItem [%d] %d %d %d", nIndex, nItemID, nBodyPart, nWeaponType);
        if (m_apChangeItem[nIndex2]) {
            int32_t nItemID2 = m_apChangeItem[nIndex2]->nItemID.GetData(); // TODO check anvil ID
            DEBUG_MESSAGE("CUIFusionAnvil::PutItem [%d] = %d", nIndex2, nItemID2);
            if (nItemID == nItemID2) {
                Notice("You cannot fuse items that have the same appearance.");
                return 0;
            }
            int32_t nBodyPart2;
            if (!get_bodypart_from_item(nItemID2, 2, &nBodyPart2, 0) || nBodyPart != nBodyPart2) {
                Notice("The fusion anvil only works on equipment of the same type.");
                return 0;
            }
            if (nWeaponType && nWeaponType != get_weapon_type(nItemID2)) {
                Notice("The fusion anvil only works on weapons of the same type.");
                return 0;
            }
            m_pBtOK->SetEnable(1);
        }
        m_apChangeItem[nIndex] = pItem;
        m_anChangeItemPos[nIndex] = nSlotPosition;
        InvalidateRect(nullptr);
        return 0;
    }
    void SendPacket() {
        // TODO exclRequest check and set
        COutPacket oPacket(85);                                 // CP_UserConsumeCashItemUseRequest
        oPacket.Encode4(CWvsApp::GetInstance()->m_tUpdateTime); // get_update_time();
        oPacket.Encode2(m_nItemPOS);
        oPacket.Encode4(m_nItemID);
        oPacket.Encode2(m_anChangeItemPos[0]);
        oPacket.Encode2(m_anChangeItemPos[1]);
        CClientSocket::GetInstance()->SendPacket(oPacket);
        SetRet(1);
    }

    static int32_t Notice(const char* sMessage) {
        // CUtilDlg::Notice
        return reinterpret_cast<int32_t(__cdecl*)(ZXString<char>, const wchar_t*, ZRef<CDialog>*, int32_t, int32_t)>(0x00977220)(ZXString<char>(sMessage), nullptr, nullptr, 1, 0);
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
    new CUIFusionAnvil(nPOS, nItemID);
}

static auto CDraggableItem__OnDropped = reinterpret_cast<int32_t(__thiscall*)(CDraggableItem*, IUIMsgHandler*, IUIMsgHandler*, int32_t, int32_t)>(0x00508700);
static auto CRTTI_ms_pRTTI_CUIItem = reinterpret_cast<CRTTI*>(0x00C616D0);

int32_t __fastcall CDraggableItem__OnDropped_hook(CDraggableItem* pThis, void* _EDX, IUIMsgHandler* pFrom, IUIMsgHandler* pTo, int32_t rx, int32_t ry) {
    if (pFrom && pFrom->IsKindOf(CRTTI_ms_pRTTI_CUIItem) && pTo && pTo->IsKindOf(&CUIFusionAnvil::ms_RTTI_CUIFusionAnvil)) {
        ZRef<GW_ItemSlotBase> pItem = CWvsContext::GetInstance()->m_pCharacterData->GetItem(pThis->m_nItemTI, pThis->m_nSlotPosition);
        if (!pItem) {
            return 0;
        }
        return dynamic_cast<CUIFusionAnvil*>(pTo)->PutItem(pItem, pThis->m_nItemTI, pThis->m_nSlotPosition, rx, ry);
    }
    return CDraggableItem__OnDropped(pThis, pFrom, pTo, rx, ry);
}


struct GW_ItemSlotEquip : public GW_ItemSlotBase {
    MEMBER_AT(int32_t, 0x139, nAnvilItemID)
};

static auto GW_ItemSlotEquip__ctor = reinterpret_cast<void(__thiscall*)(GW_ItemSlotEquip*)>(0x004F5FD0);

void __fastcall GW_ItemSlotEquip__ctor_hook(GW_ItemSlotEquip* pThis, void* _EDX) {
    GW_ItemSlotEquip__ctor(pThis);
    pThis->nAnvilItemID = 0;
}

static auto GW_ItemSlotEquip__RawDecode = reinterpret_cast<void(__thiscall*)(GW_ItemSlotEquip*, CInPacket& iPacket)>(0x004F8360);

void __fastcall GW_ItemSlotEquip__RawDecode_hook(GW_ItemSlotEquip* pThis, void* _EDX, CInPacket& iPacket) {
    GW_ItemSlotEquip__RawDecode(pThis, iPacket);
    pThis->nAnvilItemID = reinterpret_cast<uint32_t(__thiscall*)(CInPacket*)>(0x00409870)(&iPacket); // iPacket.Decode4();
}


struct AvatarLook : public ZRefCounted {
    MEMBER_ARRAY_AT(int32_t, 0x19, anHairEquip, 60)
};

static auto AvatarLook__Load = reinterpret_cast<void(__thiscall*)(AvatarLook*, const GW_CharacterStat*, const ZRef<GW_ItemSlotBase>*, const ZRef<GW_ItemSlotBase>*)>(0x004F6E70);

void __fastcall AvatarLook__Load_hook(AvatarLook* pThis, void* _EDX, const GW_CharacterStat* cs, const ZRef<GW_ItemSlotBase>* apEquipped, const ZRef<GW_ItemSlotBase>* apEquipped2) {
    AvatarLook__Load(pThis, cs, apEquipped, apEquipped2);
    for (auto i = 0; i < 60; ++i) {
        auto pItem = static_cast<GW_ItemSlotBase*>(apEquipped[i]);
        if (!pItem) {
            continue;
        }
        int32_t nAnvilItemID = reinterpret_cast<GW_ItemSlotEquip*>(pItem)->nAnvilItemID;
        if (nAnvilItemID) {
            pThis->anHairEquip[i] = nAnvilItemID;
        }
    }
}


void AttachFusionAnvilMod() {
    ATTACH_HOOK(CWvsContext__SendConsumeCashItemUseRequest, CWvsContext__SendConsumeCashItemUseRequest_hook);
    ATTACH_HOOK(CDraggableItem__OnDropped, CDraggableItem__OnDropped_hook);

    Patch4(0x004F76CD + 1, 0x139 + 4); // GW_ItemSlotBase::CreateItem
    Patch4(0x005C3B8A + 1, 0x139 + 4); // CItemInfo::GetItemSlot
    ATTACH_HOOK(GW_ItemSlotEquip__ctor, GW_ItemSlotEquip__ctor_hook);
    ATTACH_HOOK(GW_ItemSlotEquip__RawDecode, GW_ItemSlotEquip__RawDecode_hook);
    ATTACH_HOOK(AvatarLook__Load, AvatarLook__Load_hook);
}