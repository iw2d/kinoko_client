#include "pch.h"
#include "client.h"
#include "hook.h"


class CAvatar {
public:
    MEMBER_AT(int, 0x18AC, m_nChairHeight)
    MEMBER_AT(IWzVector2DPtr, 0x18BC, m_pBodyOrigin)
};


void SetBodyOrigin(CAvatar* pAvatar, int x, int y) {
    int packed = 0xF0000000;
    packed |= (x & 0x0FFF) << 16;
    packed |= (y & 0xFFFF);
    pAvatar->m_nChairHeight() = packed;
}

void FixBodyOrigin(CAvatar* pAvatar) {
    int packed = pAvatar->m_nChairHeight();
    if (packed & 0xF0000000) {
        int x = (packed >> 16) & 0x0FFF;
        if (x & 0x0800) {
            x |= 0xFFFFF000;
        }
        int y = (packed & 0xFFFF);
        if (y & 0x8000) {
            y |= 0xFFFF0000;
        }
        Ztl_variant_t vEmpty;
        CHECK_HRESULT(pAvatar->m_pBodyOrigin()->raw_RelMove(x, y, vEmpty, vEmpty));
    }
}


typedef void (__thiscall* CAvatar__SetChairHeight_t)(CAvatar*, int);
static auto CAvatar__SetChairHeight = reinterpret_cast<CAvatar__SetChairHeight_t>(0x00903A50);

void __fastcall CAvatar__SetChairHeight_hook(CAvatar* pThis, void* _EDX, int nItemID) {
    CAvatar__SetChairHeight(pThis, nItemID);
    if (pThis->m_nChairHeight() != 0 || nItemID == 0) {
        return;
    }
    const IWzPropertyPtr pProperty;
    CItemInfo::GetInstance()->GetItemInfo(const_cast<IWzPropertyPtr*>(&pProperty), nItemID);
    if (!pProperty) {
        return;
    }
    Ztl_variant_t bodyRelMove;
    CHECK_HRESULT(pProperty->get_item(L"bodyRelMove", &bodyRelMove));
    IWzVector2DPtr pVector;
    IUnknownPtr pUnknown(bodyRelMove.GetUnknown(false, false));
    if (FAILED(pUnknown.QueryInterface(__uuidof(IWzVector2D), &pVector))) {
        return;
    }
    int x, y;
    CHECK_HRESULT(pVector->get_x(&x));
    CHECK_HRESULT(pVector->get_y(&y));
    SetBodyOrigin(pThis, x, y);
}


typedef void (__thiscall* CAvatar__Update_t)(CAvatar*);
static auto CAvatar__Update = reinterpret_cast<CAvatar__Update_t>(0x0046C050);

void __fastcall CAvatar__Update_hook(CAvatar* pThis, void* _EDX) {
    CAvatar__Update(pThis);
    FixBodyOrigin(pThis);
}


void AttachPortableChairMod() {
    ATTACH_HOOK(CAvatar__SetChairHeight, CAvatar__SetChairHeight_hook);
    ATTACH_HOOK(CAvatar__Update, CAvatar__Update_hook);
}