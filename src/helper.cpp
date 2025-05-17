#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


static uintptr_t CDropPool__TryPickUpDropByPet_jmp = 0x00511AED;
static uintptr_t CDropPool__TryPickUpDropByPet_ret = 0x00511AFC;

void __declspec(naked) CDropPool__TryPickUpDropByPet_hook() {
    __asm {
        lea     edx, [ eax - 200 ]      ; x1
        mov     [ ebp - 0x34 ], edx
        lea     edx, [ ecx - 200 ]      ; y1
        add     eax, 200                ; x2
        add     ecx, 200                ; y2
        jmp     [ CDropPool__TryPickUpDropByPet_ret ]
    };
}


typedef void (__cdecl* CCurseProcess__ProcessString_t)(char*, int*, int);
static auto CCurseProcess__ProcessString = reinterpret_cast<CCurseProcess__ProcessString_t>(0x007477F0);

int __cdecl CCurseProcess__ProcessString_hook(char* sString, int* pbFiltered, int bIgnoreNewLine) {
    return 1;
}


typedef void (__thiscall* CInputSystem__DetectJoystick_t)(CInputSystem*);
static auto CInputSystem__DetectJoystick = reinterpret_cast<CInputSystem__DetectJoystick_t>(0x00571740);

void __fastcall CInputSystem__DetectJoystick_hook(void* pThis, void* _EDX) {
    // noop
}


class CVecCtrl;

typedef void (__thiscall* CVecCtrl__SetImpactNext_t)(CVecCtrl*, int, long double, long double);
static auto CVecCtrl__SetImpactNext = reinterpret_cast<CVecCtrl__SetImpactNext_t>(0x00905CD0);

void __fastcall CVecCtrl__SetImpactNext_hook(CVecCtrl* pThis, void* _EDX, int nAttr, long double vx, long double vy) {
    if (nAttr == 0x14 && CInputSystem::GetInstance()->IsKeyPressed(38)) {
        // MPA_FLASHJUMP + ArrowUp
        vx = 0.0;
        vy = -1000.0;
    }
    CVecCtrl__SetImpactNext(pThis, nAttr, vx, vy);
}


typedef void (__thiscall* CUserLocal__Jump_t)(CUserLocal*, int);
static auto CUserLocal__Jump = reinterpret_cast<CUserLocal__Jump_t>(0x0090A1D0);

class CVecCtrl {
public:
    MEMBER_AT(void*, 0x1A0, m_pfh)
};

void __fastcall CUserLocal__Jump_hook(CUserLocal* pThis, void* _EDX, int bEnforced) {
    CUserLocal__Jump(pThis, bEnforced);
    // Check if on the ground (has foothold), or on ladder/rope (CVecCtrl::GetLadderOrRope)
    CVecCtrl* pVecCtrl = reinterpret_cast<CVecCtrl*>(&static_cast<IWzVector2D*>(pThis->m_pvc())[-3]);
    if (pVecCtrl->m_pfh() || reinterpret_cast<void* (__thiscall*)(CVecCtrl*)>(0x004BBE80)(pVecCtrl)) {
        return;
    }
    // Trigger flash jump skill
    if (!bEnforced && pThis->m_bJumpKeyUp()) {
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
            pThis->m_bJumpKeyUp() = false;
            pThis->DoActiveSkill(nSkillID, 0, nullptr);
        }
    }
}


typedef ZXString<char>* (__cdecl* get_weapon_attack_speed_t)(ZXString<char>* result, int nItemID);
static auto get_weapon_attack_speed = reinterpret_cast<get_weapon_attack_speed_t>(0x005A66B0);

const char* get_attack_speed_string(int nAttackSpeed) {
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

ZXString<char>* __cdecl get_weapon_attack_speed_hook(ZXString<char>* result, int nItemID) {
    result->_m_pStr = nullptr;
    // get_weapon_type(nItemID)
    if (reinterpret_cast<int (__cdecl*)(int)>(0x0046F660)(nItemID)) {
        ZXString<wchar_t> sUOL;
        // get_equip_data_path(&sUOL, nItemID)
        reinterpret_cast<ZXString<wchar_t*> (__cdecl*)(ZXString<wchar_t>*, int)>(0x005A6060)(&sUOL, nItemID);
        if (sUOL._m_pStr && *sUOL._m_pStr) {
            Ztl_variant_t vEmpty;
            Ztl_variant_t vEquip;
            CHECK_HRESULT(get_rm()->raw_GetObject(sUOL._m_pStr, vEmpty, vEmpty, &vEquip));
            IWzPropertyPtr pEquip = IWzPropertyPtr(vEquip.GetUnknown(false, false));
            Ztl_variant_t vInfo;
            CHECK_HRESULT(pEquip->get_item(L"info", &vInfo));
            IWzPropertyPtr pInfo = IWzPropertyPtr(vInfo.GetUnknown(false, false));
            Ztl_variant_t vAttackSpeed;
            CHECK_HRESULT(pInfo->get_item(L"attackSpeed", &vAttackSpeed));

            // get_int32(&vAttackSpeed, 6);
            unsigned int nAttackSpeed = reinterpret_cast<unsigned int (__cdecl*)(Ztl_variant_t*, unsigned int)>(0x00406830)(&vAttackSpeed, 6);
            char sAttackSpeed[20];
            sprintf_s(sAttackSpeed, 20, "%s (%d)", get_attack_speed_string(nAttackSpeed), nAttackSpeed);

            // ZXString<char>::ZXString<char>(result, sAttackSpeed);
            reinterpret_cast<void (__thiscall*)(ZXString<char>*, const char*, int)>(0x0042D230)(result, sAttackSpeed, -1);
        }
    }
    return result;
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetMapString_t)(CItemInfo*, ZXString<char>*, unsigned int, const char*);
static auto CItemInfo__GetMapString = reinterpret_cast<CItemInfo__GetMapString_t>(0x005A9CA0);

ZXString<char>* __fastcall CItemInfo__GetMapString_hook(CItemInfo* pThis, void* _EDX,  ZXString<char>* result, unsigned int dwFieldID, const char* sKey) {
    CItemInfo__GetMapString(pThis, result, dwFieldID, sKey);
    if (!strcmp(sKey, "mapName")) {
        char sFieldID[20];
        snprintf(sFieldID, 20, " (%d)", dwFieldID);
        // ZXString<char>::_Cat(result, sFieldID, strlen(sFieldID));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sFieldID, strlen(sFieldID));
    }
    return result;
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetItemDesc_t)(CItemInfo*, ZXString<char>*, int);
static auto CItemInfo__GetItemDesc = reinterpret_cast<CItemInfo__GetItemDesc_t>(0x005B16E0);

ZXString<char>* __fastcall CItemInfo__GetItemDesc_hook(CItemInfo* pThis, void* _EDX, ZXString<char>* result, int nItemID) {
    CItemInfo__GetItemDesc(pThis, result, nItemID);
    if (result->GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(result, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sNewLine, strlen(sNewLine));
    }
    char sQuestID[30];
    snprintf(sQuestID, 30, "#cItem ID : %d#", nItemID);
    // ZXString<char>::_Cat(result, sQuestID, strlen(sQuestID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sQuestID, strlen(sQuestID));
    return result;
}


class CSkillInfo;

struct SKILLENTRY {
    unsigned char padding[0x264];

    MEMBER_AT(int, 0x0, nSkillID)
    MEMBER_AT(ZXString<char>, 0x4, sName)
    MEMBER_AT(ZXString<char>, 0x8, sDescription)
};

typedef ZRef<SKILLENTRY>* (__thiscall* CSkillInfo__LoadSkill_t)(CSkillInfo*, ZRef<SKILLENTRY>*, int, void*, void*);
static auto CSkillInfo__LoadSkill = reinterpret_cast<CSkillInfo__LoadSkill_t>(0x0070C190);

ZRef<SKILLENTRY>* __fastcall CSkillInfo__LoadSkill_hook(CSkillInfo* pThis, void* _EDX, ZRef<SKILLENTRY>* result, int nSkillID, void* pSkill, void* pStrSR) {
    CSkillInfo__LoadSkill(pThis, result, nSkillID, pSkill, pStrSR);
    if (result->p->sDescription().GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(&result->p->sDescription, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(&result->p->sDescription(), sNewLine, strlen(sNewLine));
    }
    char sSkillID[30];
    snprintf(sSkillID, 30, "#cSkill ID : %d#", nSkillID);
    // ZXString<char>::_Cat(&result->p->sDescription, sSkillID, strlen(sSkillID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(&result->p->sDescription(), sSkillID, strlen(sSkillID));
    return result;
}


static uintptr_t CUIQuestInfoDetail__Draw_jmp = 0x00824A93;
static uintptr_t CUIQuestInfoDetail__Draw_ret = 0x00824C04;

void __stdcall CUIQuestInfoDetail__Draw_helper(IWzCanvas* pCanvas, IWzFont* pFont, unsigned short usQuestID) {
    wchar_t sQuestID[30];
    swprintf(sQuestID, 30, L"Quest ID : %d", usQuestID);
    Ztl_variant_t vEmpty;
    unsigned int result[4];
    CHECK_HRESULT(pCanvas->raw_DrawText(35, 56, sQuestID, pFont, vEmpty, vEmpty, result));
}

void __declspec(naked) CUIQuestInfoDetail__Draw_hook() {
    __asm {
        mov     eax, [ ebp + 0x8C ]
        movzx   edx, word ptr [ eax ]
        push    edx ; this->m_pQI.p->usQuestID
        mov     eax, [ ebp + 0xF0 ]
        push    eax ; IWzFont*
        push    esi ; IWzCanvas*
        call    CUIQuestInfoDetail__Draw_helper
        jmp     [ CUIQuestInfoDetail__Draw_ret ]
    };
}


void AttachClientHelper() {
    // CChatHelper::TryChat
    Patch1(0x004AA7EF, 0xEB); // bypass chat cooldown
    Patch1(0x004AA74A, 0xEB); // bypass chat repeat

    // CVecCtrl::IsFalling
    PatchRetZero(0x0093A140); // double jump while falling

    // CDropPool::TryPickUpDropByPet
    PatchJmp(CDropPool__TryPickUpDropByPet_jmp, reinterpret_cast<uintptr_t>(&CDropPool__TryPickUpDropByPet_hook)); // increase pet pickup range

    ATTACH_HOOK(CCurseProcess__ProcessString, CCurseProcess__ProcessString_hook); // disable profanity filter
    ATTACH_HOOK(CInputSystem__DetectJoystick, CInputSystem__DetectJoystick_hook); // fix stutter
    ATTACH_HOOK(CVecCtrl__SetImpactNext, CVecCtrl__SetImpactNext_hook); // vertical double jump
    ATTACH_HOOK(CUserLocal__Jump, CUserLocal__Jump_hook); // double jump with jump key
    ATTACH_HOOK(get_weapon_attack_speed, get_weapon_attack_speed_hook); // append attack speed value to weapon speed string

#ifdef _DEBUG
    // Add ID to map name, item description, skill description, quest info
    ATTACH_HOOK(CItemInfo__GetMapString, CItemInfo__GetMapString_hook);
    ATTACH_HOOK(CItemInfo__GetItemDesc, CItemInfo__GetItemDesc_hook);
    ATTACH_HOOK(CSkillInfo__LoadSkill, CSkillInfo__LoadSkill_hook);
    PatchJmp(CUIQuestInfoDetail__Draw_jmp, reinterpret_cast<uintptr_t>(&CUIQuestInfoDetail__Draw_hook)); // replace "Low Level Quest"
#endif
}