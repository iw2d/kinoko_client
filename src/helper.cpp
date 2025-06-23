#include "pch.h"
#include "hook.h"
#include "ztl/zalloc.h"
#include "ztl/zstr.h"
#include "wzlib/property.h"
#include "wzlib/shape2d.h"
#include "wzlib/gr2d.h"
#include "common/vecctrl.h"
#include "common/iteminfo.h"
#include "common/skillinfo.h"
#include "wvs/util.h"
#include "wvs/userlocal.h"
#include "wvs/inputsystem.h"
#include "wvs/animationdisplayer.h"


static auto CUserLocal__Jump = reinterpret_cast<void(__thiscall*)(CUserLocal*, int32_t)>(0x0090A1D0);

void __fastcall CUserLocal__Jump_hook(CUserLocal* pThis, void* _EDX, int32_t bEnforced) {
    CUserLocal__Jump(pThis, bEnforced);
    CVecCtrl* pVecCtrl = reinterpret_cast<CVecCtrl*>(&static_cast<IWzVector2D*>(pThis->m_pvc)[-3]);
    // CVecCtrl::GetFoothold(pVecCtrl) || CVecCtrl::GetLadderOrRope(pVecCtrl)
    if (reinterpret_cast<CStaticFoothold*(__thiscall*)(CVecCtrl*)>(0x00639F20)(pVecCtrl) ||
            reinterpret_cast<CLadderOrRope*(__thiscall*)(CVecCtrl*)>(0x004BBE80)(pVecCtrl)) {
        return;
    }
    if (!bEnforced && pThis->m_bJumpKeyUp) {
        int32_t nJob = pThis->GetJobCode();
        int32_t nSkillID = 0;
        if (nJob / 10 == 41) {
            nSkillID = 4111006; // Night Lord
        } else if (nJob / 10 == 42) {
            nSkillID = 4211009; // Shadower
        } else if (nJob / 10 == 43) {
            nSkillID = 4321003; // Dual Blade
        } else if (nJob / 100 == 14) {
            nSkillID = 14101004; // Night Walker
        }
        if (nSkillID) {
            pThis->m_bJumpKeyUp = false;
            pThis->DoActiveSkill(nSkillID, 0, nullptr);
        }
    }
}


static auto CVecCtrl__SetImpactNext = reinterpret_cast<void(__thiscall*)(CVecCtrl*, int32_t, double, double)>(0x00905CD0);

void __fastcall CVecCtrl__SetImpactNext_hook(CVecCtrl* pThis, void* _EDX, int32_t nAttr, double vx, double vy) {
    int32_t nInputX;
    int32_t nInputY;
    // CVecCtrl::GetInput(pThis, nInputX, nInputY);
    reinterpret_cast<void(__thiscall*)(CVecCtrl*, int32_t&, int32_t&)>(0x009049C0)(pThis, nInputX, nInputY);
    if (nAttr == 0x14 && nInputY < 0) {
        // MPA_FLASHJUMP
        vx = 0.0;
        vy = -1000.0;
    }
    CVecCtrl__SetImpactNext(pThis, nAttr, vx, vy);
}


static auto CItemInfo__GetMapString = reinterpret_cast<ZXString<char>*(__thiscall*)(CItemInfo*, ZXString<char>*, uint32_t, const char*)>(0x005A9CA0);

ZXString<char>* __fastcall CItemInfo__GetMapString_hook(CItemInfo* pThis, void* _EDX, ZXString<char>* result, uint32_t dwFieldID, const char* sKey) {
    CItemInfo__GetMapString(pThis, result, dwFieldID, sKey);
    if (!strcmp(sKey, "mapName")) {
        ZXString<char> sFieldID;
        sFieldID.Format(" (%d)", dwFieldID);
        result->Cat(sFieldID);
    }
    return result;
}

static auto CItemInfo__GetItemDesc = reinterpret_cast<ZXString<char>*(__thiscall*)(CItemInfo*, ZXString<char>*, int32_t)>(0x005B16E0);

ZXString<char>* __fastcall CItemInfo__GetItemDesc_hook(CItemInfo* pThis, void* _EDX, ZXString<char>* result, int32_t nItemID) {
    CItemInfo__GetItemDesc(pThis, result, nItemID);
    if (result->GetLength() > 0) {
        result->Cat("\r\n");
    }
    ZXString<char> sItemID;
    sItemID.Format("#cItem ID : %d#", nItemID);
    result->Cat(sItemID);
    return result;
}

static auto CSkillInfo__LoadSkill = reinterpret_cast<ZRef<SKILLENTRY>*(__thiscall*)(CSkillInfo*, ZRef<SKILLENTRY>*, int32_t, IWzPropertyPtr, IWzPropertyPtr)>(0x0070C190);

ZRef<SKILLENTRY>* __fastcall CSkillInfo__LoadSkill_hook(CSkillInfo* pThis, void* _EDX, ZRef<SKILLENTRY>* result, int32_t nSkillID, IWzPropertyPtr pSkill, IWzPropertyPtr pStrSR) {
    CSkillInfo__LoadSkill(pThis, result, nSkillID, pSkill, pStrSR);
    auto entry = *result;
    if (entry->sDescription.GetLength() > 0) {
        entry->sDescription.Cat("\r\n");
    }
    ZXString<char> sSkillID;
    sSkillID.Format("#cSkill ID : %d#", nSkillID);
    entry->sDescription.Cat(sSkillID);
    return result;
}

static auto get_weapon_attack_speed = 0x005A66B0;

const char* get_attack_speed_string(int32_t nAttackSpeed) {
    switch (nAttackSpeed) {
    case 0:
    case 1:
        return " FASTEST";
    case 2:
    case 3:
        return " FASTER";
    case 4:
    case 5:
        return " FAST";
    case 6:
        return " NORMAL";
    case 7:
    case 8:
        return " SLOW";
    case 9:
    case 10:
        return " SLOWER";
    case 11:
    case 12:
        return " SLOWEST";
    default:
        return "";
    }
}

ZXString<char>* __cdecl get_weapon_attack_speed_hook(ZXString<char>* result, int32_t nItemID) {
    construct(result);
    // get_weapon_type(nItemID)
    if (reinterpret_cast<int32_t(__cdecl*)(int32_t)>(0x0046F660)(nItemID)) {
        ZXString<wchar_t> sUOL;
        // get_equip_data_path(&sUOL, nItemID)
        reinterpret_cast<ZXString<wchar_t>*(__cdecl*)(ZXString<wchar_t>*, int32_t)>(0x005A6060)(&sUOL, nItemID);
        if (!sUOL.IsEmpty()) {
            Ztl_variant_t vEquip;
            CHECK_HR(get_rm()->raw_GetObject(sUOL, vtEmpty, vtEmpty, &vEquip));
            IWzPropertyPtr pEquip = vEquip.GetUnknown(false, false);

            Ztl_variant_t vInfo;
            CHECK_HR(pEquip->get_item(L"info", &vInfo));
            IWzPropertyPtr pInfo = vInfo.GetUnknown(false, false);
            Ztl_variant_t vAttackSpeed;
            CHECK_HR(pInfo->get_item(L"attackSpeed", &vAttackSpeed));

            uint32_t nAttackSpeed = get_int32(vAttackSpeed, 6);
            result->Format("%s (%d)", get_attack_speed_string(nAttackSpeed), nAttackSpeed);
        }
    }
    return result;
}

static uintptr_t CUIQuestInfoDetail__Draw_jmp = 0x00824A93;
static uintptr_t CUIQuestInfoDetail__Draw_ret = 0x00824C04;

void __stdcall CUIQuestInfoDetail__Draw_helper(IWzCanvas* pCanvas, IWzFont* pFont, uint16_t usQuestID) {
    wchar_t sQuestID[256];
    swprintf_s(sQuestID, 256, L"Quest ID : %d", usQuestID);
    uint32_t result[4];
    CHECK_HR(pCanvas->raw_DrawText(35, 56, sQuestID, pFont, vtEmpty, vtEmpty, result));
}

void __declspec(naked) CUIQuestInfoDetail__Draw_hook() {
    __asm {
        mov     eax, [ ebp + 0x8C ]
        movzx   edx, word ptr [ eax ]
        push    edx                     ; this->m_pQI.p->usQuestID
        mov     eax, [ ebp + 0xF0 ]
        push    eax                     ; IWzFont*
        push    esi                     ; IWzCanvas*
        call    CUIQuestInfoDetail__Draw_helper
        jmp     [ CUIQuestInfoDetail__Draw_ret ]
    }
    ;
}


void AttachClientHelper() {
    // CChatHelper::TryChat
    Patch1(0x004AA7EF, 0xEB); // bypass chat cooldown
    Patch1(0x004AA74A, 0xEB); // bypass chat repeat

    // CCurseProcess::ProcessString
    PatchStr(0x007477F0, "\x31\xC0\x40\xC3"); // disable profanity filter

    // CVecCtrl::IsFalling
    PatchRetZero(0x0093A140); // double jump while falling

    // CUserLocal::UseFuncKeyMapped
    PatchCall(0x009336D3, reinterpret_cast<uintptr_t>(&CUserLocal__Jump_hook));

    ATTACH_HOOK(CVecCtrl__SetImpactNext, CVecCtrl__SetImpactNext_hook); // vertical double jump
    ATTACH_HOOK(CItemInfo__GetMapString, CItemInfo__GetMapString_hook); // append map ID to map name
    ATTACH_HOOK(CItemInfo__GetItemDesc, CItemInfo__GetItemDesc_hook);   // append item ID to item description
    ATTACH_HOOK(CSkillInfo__LoadSkill, CSkillInfo__LoadSkill_hook);     // append skill ID to skill description
    ATTACH_HOOK(get_weapon_attack_speed, get_weapon_attack_speed_hook); // append attack speed value to weapon speed string

    PatchJmp(CUIQuestInfoDetail__Draw_jmp, reinterpret_cast<uintptr_t>(&CUIQuestInfoDetail__Draw_hook)); // replace "Low Level Quest"
}