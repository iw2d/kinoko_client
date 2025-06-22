#include "pch.h"
#include "hook.h"
#include "ztl/zalloc.h"
#include "ztl/zstr.h"
#include "wzlib/shape2d.h"
#include "wzlib/gr2d.h"
#include "common/vecctrl.h"
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
        int nJob = pThis->GetJobCode();
        int nSkillID = 0;
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
    ATTACH_HOOK(get_weapon_attack_speed, get_weapon_attack_speed_hook); // append attack speed value to weapon speed string
}