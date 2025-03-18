#include <map>

#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#define AL_ITEMEFF 2 // hijack AL_ADMINEFFECT


struct USERLAYER {
    int bFixed;
    int nPos; // POSTYPE
    IWzGr2DLayerPtr pLayer;
};
static_assert(sizeof(USERLAYER) == 0xC);

class CAvatar {
public:
    inline int GetCurrentAction(int* pnDir) {
        return reinterpret_cast<int (__thiscall*)(CAvatar*, int*)>(0x00460800)(this, pnDir);
    }
};

class CUser {
public:
    struct ADDITIONALLAYER {
        int nDataForRepeat;
        int nType;
        int nData;
        USERLAYER l;
        USERLAYER l2;
    };
    static_assert(sizeof(ADDITIONALLAYER) == 0x24);

    MEMBER_AT(IWzGr2DLayerPtr, 0x1960, m_pLayerUnderFace);

    inline ADDITIONALLAYER* GetAdditionalLayer(int nType) {
        return reinterpret_cast<ADDITIONALLAYER* (__thiscall*)(CUser*, int)>(0x008DE110)(this, nType);
    }
    inline ADDITIONALLAYER* RemoveAdditionalLayer(int nType) {
        return reinterpret_cast<ADDITIONALLAYER* (__thiscall*)(CUser*, int)>(0x008E06F0)(this, nType);
    }
    inline int LoadLayer(Ztl_bstr_t bsUOL, int bLeft, USERLAYER* l, int* pnRepeatStartIndex) {
        return reinterpret_cast<int (__thiscall*)(CUser*, Ztl_bstr_t, int, USERLAYER*, int*)>(0x008E96D0)(this, bsUOL, bLeft, l, pnRepeatStartIndex);
    }
};


static std::map<int, IWzPropertyPtr> g_mItemEffectProp;

static auto CItemInfo__IterateItemInfo = reinterpret_cast<int (__thiscall*)(CItemInfo*)>(0x005C4C00);

int __fastcall CItemInfo__IterateItemInfo_hook(CItemInfo* pThis, void* _EDX) {
    // load item eff
    Ztl_variant_t vEmpty;
    Ztl_variant_t vItemEff;
    CHECK_HRESULT(get_rm()->raw_GetObject(L"Effect/ItemEff.img", vEmpty, vEmpty, &vItemEff));
    IWzPropertyPtr pItemEff = IWzPropertyPtr(vItemEff.GetUnknown(false, false));

    // iterate item eff
    IUnknownPtr pEnumUnknown;
    CHECK_HRESULT(pItemEff->get__NewEnum(&pEnumUnknown));
    IEnumVARIANTPtr pEnum = IEnumVARIANTPtr(pEnumUnknown);
    while (1) {
        VARIANT rgVar[1];
        ULONG uCeltFetched;
        if (FAILED(pEnum->Next(1, rgVar, &uCeltFetched)) || uCeltFetched == 0) {
            break;
        }

        // check for effect node
        int nItemID = wcstol(rgVar[0].bstrVal, nullptr, 10);
        Ztl_variant_t vProp;
        CHECK_HRESULT(pItemEff->get_item(rgVar[0].bstrVal, &vProp));
        IWzPropertyPtr pProp = IWzPropertyPtr(vProp.GetUnknown(false, false));
        Ztl_variant_t vEffect;
        CHECK_HRESULT(pProp->get_item(L"effect", &vEffect));
        if (vEffect.vt != VT_EMPTY) {
            DEBUG_MESSAGE("Loaded Effect/ItemEff.img/%d", nItemID);
            g_mItemEffectProp[nItemID] = IWzPropertyPtr(vEffect.GetUnknown(false, false));
        }
    }
    return CItemInfo__IterateItemInfo(pThis);
}


void UpdateItemEff(CUser* pUser) {
    CAvatar* pAvatar = reinterpret_cast<CAvatar*>(reinterpret_cast<uintptr_t>(pUser) + 0x88);
    bool bItemEff = false;
    auto pAdditionalLayer = pUser->GetAdditionalLayer(AL_ITEMEFF);
    int* anHairEquip = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(pAvatar) + 0x1D);
    for (size_t i = 0; i < 60; ++i) {
        int nItemID = anHairEquip[i];
        if (auto search = g_mItemEffectProp.find(nItemID); search != g_mItemEffectProp.end()) {
            bItemEff = true;

            // get current action
            int nAction = pAvatar->GetCurrentAction(nullptr);
            Ztl_bstr_t sActionName;
            // get_action_name_from_code(&sActionName, nAction);
            reinterpret_cast<void (__cdecl*)(Ztl_bstr_t*, int)>(0x004BA910)(&sActionName, nAction);

            // action and item id are packed into pAdditionalLayer->nData
            if ((pAdditionalLayer->nData & 0xFFFFFF) == nItemID && ((pAdditionalLayer->nData >> 24) & 0xFF) == nAction) {
                continue;
            }
            pAdditionalLayer->nData = (nAction << 24) | (nItemID & 0xFFFFFF);

            // resolve UOL
            IWzPropertyPtr pEffect = search->second;
            Ztl_variant_t vAction;
            CHECK_HRESULT(pEffect->get_item(sActionName.m_Data->m_wstr, &vAction));
            wchar_t sUOL[1024];
            swprintf(sUOL, 1024, L"Effect/ItemEff.img/%d/effect/%ls", nItemID, vAction.vt == VT_EMPTY ? L"default" : sActionName.m_Data->m_wstr);

            // load layer and animate
            int bFlip;
            CHECK_HRESULT(pUser->m_pLayerUnderFace()->get_flip(&bFlip));
            if (pUser->LoadLayer(Ztl_bstr_t(sUOL), bFlip, &pAdditionalLayer->l, 0)) {
                Ztl_variant_t vEmpty;
                CHECK_HRESULT(pAdditionalLayer->l.pLayer->raw_Animate(GA_REPEAT, vEmpty, vEmpty));
            } else {
                pUser->RemoveAdditionalLayer(AL_ITEMEFF);
            }
        }
    }

    // set layer visibility
    if (pAdditionalLayer->l.pLayer) {
        CHECK_HRESULT(pAdditionalLayer->l.pLayer->put_visible(bItemEff));
    }
}


static auto CUser__OnAvatarModified = reinterpret_cast<void (__thiscall*)(void*)>(0x008F6F00);

void __fastcall CUser__OnAvatarModified_hook(void* _ECX, void* _EDX) {
    CUser__OnAvatarModified(_ECX);
    UpdateItemEff(reinterpret_cast<CUser*>(reinterpret_cast<uintptr_t>(_ECX) - 0x88)); // ecx offset
}


static auto CUser__SetMoveAction = reinterpret_cast<void (__thiscall*)(void*, int, int)>(0x008DF540);

void __fastcall CUser__SetMoveAction_hook(void* _ECX, void* _EDX, int nMA, int bReload) {
    CUser__SetMoveAction(_ECX, nMA, bReload);
    UpdateItemEff(reinterpret_cast<CUser*>(reinterpret_cast<uintptr_t>(_ECX) - 0x88)); // ecx offset
}


void AttachItemEffectMod() {
    ATTACH_HOOK(CItemInfo__IterateItemInfo, CItemInfo__IterateItemInfo_hook);
    ATTACH_HOOK(CUser__OnAvatarModified, CUser__OnAvatarModified_hook);
    ATTACH_HOOK(CUser__SetMoveAction, CUser__SetMoveAction_hook);
}