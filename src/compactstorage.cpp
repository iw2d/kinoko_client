#include "pch.h"
#include "client.h"
#include "hook.h"


class GW_ItemSlotBase;

class CCtrlScrollBar {
public:
    MEMBER_AT(int, 0x38, m_nCurPos);

    inline void SetScrollRange(int nRange) {
        reinterpret_cast<void (__thiscall*)(CCtrlScrollBar*, int)>(0x004E9B70)(this, nRange);
    }
};

class CTrunkDlg {
public:
    struct ITEM {
        int nItemID;
        int nStock;
        int nIdx;
        Ztl_bstr_t sItemName;
        IWzCanvasPtr pIcon;
        ZRef<GW_ItemSlotBase> pItem;
    };
    static_assert(sizeof(ITEM) == 0x1C);

    MEMBER_AT(ZRef<CCtrlScrollBar>, 0x9C, m_pSBGet)
    MEMBER_AT(ZRef<CCtrlScrollBar>, 0xA4, m_pSBPut)
    MEMBER_AT(ZArray<ITEM>, 0xB8, m_aGetItem)
    MEMBER_AT(ZArray<ITEM>, 0xBC, m_aPutItem)
    MEMBER_AT(IWzPropertyPtr, 0xC8, m_pImgFontNumber)
    MEMBER_AT(IWzCanvasPtr, 0xCC, m_pCanvasSelect)
    MEMBER_AT(IWzCanvasPtr, 0xD0, m_pCanvasEnable)
    MEMBER_AT(int, 0xD4, m_nGetSelected)
    MEMBER_AT(int, 0xD8, m_nPutSelected)
    MEMBER_AT(int, 0xDC, m_nSlotCount)
};


static auto CTrunkDlg__DrawGetItem = reinterpret_cast<void (__thiscall*)(CTrunkDlg*, IWzCanvasPtr)>(0x00767710);

void __fastcall CTrunkDlg__DrawGetItem_hook(CTrunkDlg* pThis, void* _EDX, IWzCanvasPtr pCanvas) {
    int nGetCurPos = pThis->m_pSBGet().p->m_nCurPos();
    int nGetItemCount = pThis->m_aGetItem().GetCount();
    for (int i = 0; i < 25; ++i) {
        // resolve item and position
        int nX = 10 + (i % 5) * 42;
        int nY = 125 + (i / 5) * 42;
        int nItemIdx = i + (nGetCurPos * 5);
        if (nItemIdx >= nGetItemCount) {
            // color disabled slot
            if (nItemIdx >= pThis->m_nSlotCount()) {
                CHECK_HRESULT(pCanvas->raw_DrawRectangle(nX, nY - 32, 33, 33, 0XAACCCCCC));
            }
            continue;
        }
        CTrunkDlg::ITEM& item = pThis->m_aGetItem().a[nItemIdx];

        // color selected slot
        if (nItemIdx == pThis->m_nGetSelected()) {
            CHECK_HRESULT(pCanvas->raw_DrawRectangle(nX, nY - 32, 33, 33, 0xAA66CCFF));
        }

        // resolve equip item quality
        ZRef<GW_ItemSlotBase> ref;
        // ZRef<GW_ItemSlotBase>::ZRef<GW_ItemSlotBase>(&ref, &item.pItem);
        reinterpret_cast<void( __thiscall*)(ZRef<GW_ItemSlotBase>*, const ZRef<GW_ItemSlotBase>*)>(0x0042B690)(&ref, &item.pItem);
        // CItemInfo::CalcEquipItemQuality(CItemInfo::GetInstance(), ref);
        int nEquipItemQuality = reinterpret_cast<int (__thiscall*)(CItemInfo*, ZRef<GW_ItemSlotBase>)>(0x005C2A30)(CItemInfo::GetInstance(), ref); // ref released

        // draw grade frame
        void* pSlotBase = item.pItem.p;
        void** vtable = *reinterpret_cast<void***>(pSlotBase);
        int nSlotType = reinterpret_cast<int (__thiscall*)(void*)>(vtable[6])(pSlotBase); // GetType
        if (nSlotType == 1) { // GW_ItemSlotEquip
            // CItemInfo::DrawGradeFrame(CItemInfo::GetInstance(), pCanvas, pSlotBase, nX, nY - 32, nX + 32, nY);
            reinterpret_cast<void (__thiscall*)(CItemInfo*, IWzCanvasPtr, void*, int, int, int, int)>(0x00594D10)(CItemInfo::GetInstance(), pCanvas, pSlotBase, nX, nY - 32, nX + 32, nY);
        }

        // draw icon
        // CItemInfo::DrawItemIconForSlot(CItemInfo::GetInstance(), pCanvas, item.nItemID, nX, nY, 0, 0, nullptr, 0, nEquipItemQuality, 0, 0);
        reinterpret_cast<void (__thiscall*)(CItemInfo*, IWzCanvasPtr, int, int, int, int, int, IWzCanvasPtr, int, int, int, int)>(0x005C0A40)(CItemInfo::GetInstance(), pCanvas, item.nItemID, nX, nY, 0, 0, nullptr, 0, nEquipItemQuality, 0, 0);

        // draw item number
        int nInventoryType = item.nItemID / 1000000;
        if (nInventoryType == 2 || nInventoryType == 3 || nInventoryType == 4) {
            int nItemNumber = reinterpret_cast<int (__thiscall*)(void*)>(vtable[8])(pSlotBase);
            // draw_number_by_image(pCanvas, nX, nY - 12, nItemNumber, pThis->m_pImgFontNumber(), 0);
            reinterpret_cast<int (__cdecl*)(IWzCanvasPtr, int, int, int, IWzPropertyPtr, int)>(0x00965780)(pCanvas, nX, nY - 12, nItemNumber, pThis->m_pImgFontNumber(), 0);
        }
    }
}


static auto CTrunkDlg__GetItemIndexFromPoint = reinterpret_cast<int (__thiscall*)(CTrunkDlg*, unsigned int, unsigned int, int*, int*)>(0x00766540);

int __fastcall CTrunkDlg__GetItemIndexFromPoint_hook(CTrunkDlg* pThis, void* _EDX, unsigned int rx, unsigned int ry, int* pbBuy, int* pnIdx) {
    int nGetCurPos = pThis->m_pSBGet().p->m_nCurPos();
    int nGetItemCount = pThis->m_aGetItem().GetCount();
    for (int i = 0; i < 25; ++i) {
        int nX = 10 + (i % 5) * 42;
        int nY = 91 + (i / 5) * 42;
        int nItemIdx = i + (nGetCurPos * 5);
        if (nItemIdx >= nGetItemCount) {
            break;
        }
        RECT rcSlot;
        SetRect(&rcSlot, nX, nY, nX + 35, nY + 35);
        if (PtInRect(&rcSlot, POINT { (LONG) rx, (LONG) ry })) {
            *pbBuy = 1;
            *pnIdx = nItemIdx;
            return 1;
        }
    }
    int nPutIdx = pThis->m_pSBPut().p->m_nCurPos();
    int nPutItemCount = pThis->m_aPutItem().GetCount();
    for (unsigned int y = 135; y < 303; y += 42) {
        if (nPutIdx >= nPutItemCount) {
            break;
        }
        unsigned int uHeight, uWidth;
        CHECK_HRESULT(pThis->m_pCanvasEnable()->get_height(&uHeight)); // the original implementation uses m_pCanvasSelect here, which is wrong
        CHECK_HRESULT(pThis->m_pCanvasEnable()->get_width(&uWidth));
        RECT rcSlot;
        SetRect(&rcSlot, 241, y, 241 + uWidth, y + uHeight);
        if (PtInRect(&rcSlot, POINT { (LONG) rx, (LONG) ry })) {
            *pbBuy = 0;
            *pnIdx = nPutIdx;
            return 1;
        }
        ++nPutIdx;
    }
    return 0;
}


static auto CTrunkDlg__SetScrollBar = reinterpret_cast<void (__thiscall*)(CTrunkDlg*)>(0x007664D0);

void __fastcall CTrunkDlg__SetScrollBar_hook(CTrunkDlg* pThis, void* _EDX) {
    int nGetItemRange = (pThis->m_nSlotCount() / 5) + 1;
    if (nGetItemRange > 5) {
        nGetItemRange -= 4;
    } else {
        nGetItemRange = 0;
    }
    pThis->m_pSBGet().p->SetScrollRange(nGetItemRange);
    int nPutItemRange = pThis->m_aPutItem().GetCount();
    if (nPutItemRange > 4) {
        nPutItemRange -= 3;
    } else {
        nPutItemRange = 0;
    }
    pThis->m_pSBPut().p->SetScrollRange(nPutItemRange);
}


void AttachCompactStorageMod() {
    ATTACH_HOOK(CTrunkDlg__DrawGetItem, CTrunkDlg__DrawGetItem_hook);
    ATTACH_HOOK(CTrunkDlg__GetItemIndexFromPoint, CTrunkDlg__GetItemIndexFromPoint_hook);
    ATTACH_HOOK(CTrunkDlg__SetScrollBar, CTrunkDlg__SetScrollBar_hook);
}