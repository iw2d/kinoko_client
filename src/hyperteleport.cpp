#include "pch.h"
#include "client.h"
#include "hook.h"


struct WORLDMAP_ITEM {
    int nX;
    int nY;
    int nType;
    int bNoToolTip;
    ZXString<char> sStreetName;
    ZXString<char> sMapName;
    ZXString<char> sMapDesc;
    int ePosType;
    IWzGr2DLayerPtr pLayer;
    int bHasQuestNpc;
    IWzGr2DLayerPtr pLayer_NpcMark;
    int bHasQuestMob;
    ZList<unsigned long> lQuestMobTemplateID;
    ZArray<unsigned short> aStartQuest;
    IWzGr2DLayerPtr pLayer_QuestMark;
    IWzCanvasPtr pPathImg;
    ZArray<unsigned long> adwField;
    ZList<long> lUserIdx;
    ZList<unsigned long> lNpcTemplateID;
    ZList<unsigned long> lMobTemplateID;
};
static_assert(sizeof(WORLDMAP_ITEM) == 0x90);

class CWorldMapDlg {
public:
    MEMBER_AT(ZArray<IWzCanvasPtr>, 0xAFC, m_apCanvasMI)
    MEMBER_AT(ZArray<WORLDMAP_ITEM>, 0xB00, m_aWMItem)
};


static auto CWorldMapDlg__OnMouseButton = reinterpret_cast<void (__thiscall*)(void*, unsigned int, unsigned int, int, int)>(0x009BF1C0);

void __fastcall CWorldMapDlg__OnMouseButton_hook(void* _ECX, void* _EDX, unsigned int msg, unsigned int wParam, int rx, int ry) {
    if (msg == WM_LBUTTONUP) {
        // Adjust ecx offset
        CWorldMapDlg* pThis = reinterpret_cast<CWorldMapDlg*>(reinterpret_cast<uintptr_t>(_ECX) - 4);
        size_t j = reinterpret_cast<size_t*>(pThis->m_aWMItem().a)[-1];
        for (size_t i = 0; i < j; i++) {
            // Logic from CWorldMapDlg::CheckSpotInfo
            WORLDMAP_ITEM* pWMI = &pThis->m_aWMItem().a[i];
            if (!pWMI || !pWMI->adwField.a || !pWMI->adwField.a[-1]) {
                continue;
            }
            // Get canvas width
            IWzCanvasPtr pCanvas = pThis->m_apCanvasMI().a[pWMI->nType];
            if (!pCanvas) {
                continue;
            }
            unsigned int uWidth;
            CHECK_HRESULT(pCanvas->get_width(&uWidth));
            // Compare width with dx, dy
            unsigned int dx = abs(pWMI->nX - (rx - 13));
            unsigned int dy = abs(pWMI->nY - (ry - 24));
            if (dx <= uWidth / 3 && dy <= uWidth / 3) {
                // Send transfer field request
                unsigned int dwTargetField = pWMI->adwField.a[0];
                // field = get_field();
                void* field = reinterpret_cast<void* (__cdecl*)()>(0x00439320)();
                if (field) {
                    // CField::SendTransferFieldRequest(field, dwTargetField, "", false, 0, 0);
                    reinterpret_cast<int (__thiscall*)(void*, unsigned int, const char*, unsigned char, unsigned int, unsigned int)>(0x005345C0)(field, dwTargetField, nullptr, false, 0, 0);
                }
            }
        }
    }
    CWorldMapDlg__OnMouseButton(_ECX, msg, wParam, rx, ry);
}


void AttachHyperTeleportMod() {
    ATTACH_HOOK(CWorldMapDlg__OnMouseButton, CWorldMapDlg__OnMouseButton_hook);
}