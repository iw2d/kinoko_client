#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


static uintptr_t CDropPool__TryPickUpDropByPet_jmp = 0x0053199D;
static uintptr_t CDropPool__TryPickUpDropByPet_ret = 0x005319AC;

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


typedef void (__thiscall* CCurseProcess__ProcessString_t)(void*, char*, int*, int);
static auto CCurseProcess__ProcessString = reinterpret_cast<CCurseProcess__ProcessString_t>(0x007B9A80);

int __fastcall CCurseProcess__ProcessString_hook(void* pThis, void* _EDX, char* sString, int* pbFiltered, int bIgnoreNewLine) {
    return 1;
}


typedef void (__thiscall* CInputSystem__DetectJoystick_t)(CInputSystem*);
static auto CInputSystem__DetectJoystick = reinterpret_cast<CInputSystem__DetectJoystick_t>(0x005A67C0);

void __fastcall CInputSystem__DetectJoystick_hook(void* pThis, void* _EDX) {
    // noop
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetMapString_t)(CItemInfo*, ZXString<char>*, unsigned int, const char*);
static auto CItemInfo__GetMapString = reinterpret_cast<CItemInfo__GetMapString_t>(0x005E3D60);

ZXString<char>* __fastcall CItemInfo__GetMapString_hook(CItemInfo* pThis, void* _EDX,  ZXString<char>* result, unsigned int dwFieldID, const char* sKey) {
    CItemInfo__GetMapString(pThis, result, dwFieldID, sKey);
    if (!strcmp(sKey, "mapName")) {
        char sFieldID[20];
        snprintf(sFieldID, 20, " (%d)", dwFieldID);
        // ZXString<char>::_Cat(result, sFieldID, strlen(sFieldID));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042E480)(result, sFieldID, strlen(sFieldID));
    }
    return result;
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetItemDesc_t)(CItemInfo*, ZXString<char>*, int);
static auto CItemInfo__GetItemDesc = reinterpret_cast<CItemInfo__GetItemDesc_t>(0x005EC6D0);

ZXString<char>* __fastcall CItemInfo__GetItemDesc_hook(CItemInfo* pThis, void* _EDX, ZXString<char>* result, int nItemID) {
    CItemInfo__GetItemDesc(pThis, result, nItemID);
    if (result->GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(result, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042E480)(result, sNewLine, strlen(sNewLine));
    }
    char sQuestID[30];
    snprintf(sQuestID, 30, "#cItem ID : %d#", nItemID);
    // ZXString<char>::_Cat(result, sQuestID, strlen(sQuestID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042E480)(result, sQuestID, strlen(sQuestID));
    return result;
}


class CSkillInfo;

struct SKILLENTRY {
    MEMBER_AT(int, 0x0, nSkillID)
    MEMBER_AT(ZXString<char>, 0x4, sName)
    MEMBER_AT(ZXString<char>, 0x8, sDescription)
};

typedef ZRef<SKILLENTRY>* (__thiscall* CSkillInfo__LoadSkill_t)(CSkillInfo*, ZRef<SKILLENTRY>*, int, void*, void*);
static auto CSkillInfo__LoadSkill = reinterpret_cast<CSkillInfo__LoadSkill_t>(0x00771D20);

ZRef<SKILLENTRY>* __fastcall CSkillInfo__LoadSkill_hook(CSkillInfo* pThis, void* _EDX, ZRef<SKILLENTRY>* result, int nSkillID, void* pSkill, void* pStrSR) {
    CSkillInfo__LoadSkill(pThis, result, nSkillID, pSkill, pStrSR);
    if (result->p->sDescription().GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(&result->p->sDescription, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042E480)(&result->p->sDescription(), sNewLine, strlen(sNewLine));
    }
    char sSkillID[30];
    snprintf(sSkillID, 30, "#cSkill ID : %d#", nSkillID);
    // ZXString<char>::_Cat(&result->p->sDescription, sSkillID, strlen(sSkillID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042E480)(&result->p->sDescription(), sSkillID, strlen(sSkillID));
    return result;
}


static uintptr_t CUIQuestInfoDetail__Draw_jmp = 0x008EF9A9;
static uintptr_t CUIQuestInfoDetail__Draw_ret = 0x008EFB13;

void __stdcall CUIQuestInfoDetail__Draw_helper(IWzCanvas* pCanvas, IWzFont* pFont, unsigned short usQuestID) {
    wchar_t sQuestID[30];
    swprintf(sQuestID, 30, L"Quest ID : %d", usQuestID);
    Ztl_variant_t vEmpty;
    unsigned int result[4];
    CHECK_HRESULT(pCanvas->raw_DrawText(35, 56, sQuestID, pFont, vEmpty, vEmpty, result));
}

void __declspec(naked) CUIQuestInfoDetail__Draw_hook() {
    __asm {
        mov     eax, [ ebp + 0x90 ]
        movzx   edx, word ptr [ eax ]
        push    edx ; this->m_pQI.p->usQuestID
        mov     eax, [ ebp + 0xF4 ]
        push    eax ; IWzFont*
        push    esi ; IWzCanvas*
        call    CUIQuestInfoDetail__Draw_helper
        jmp     [ CUIQuestInfoDetail__Draw_ret ]
    };
}


void AttachClientHelper() {
    // CChatHelper::TryChat
    Patch1(0x004BF817, 0xEB); // bypass chat cooldown
    Patch1(0x004BF772, 0xEB); // bypass chat repeat

    // CVecCtrl::IsFalling
    PatchRetZero(0x00A95390); // double jump while falling

    // CDropPool::TryPickUpDropByPet
    PatchJmp(CDropPool__TryPickUpDropByPet_jmp, reinterpret_cast<uintptr_t>(&CDropPool__TryPickUpDropByPet_hook)); // increase pet pickup range

    ATTACH_HOOK(CCurseProcess__ProcessString, CCurseProcess__ProcessString_hook); // disable profanity filter
    ATTACH_HOOK(CInputSystem__DetectJoystick, CInputSystem__DetectJoystick_hook); // fix stutter

#ifdef _DEBUG
    // Add ID to map name, item description, skill description, quest info
    ATTACH_HOOK(CItemInfo__GetMapString, CItemInfo__GetMapString_hook);
    ATTACH_HOOK(CItemInfo__GetItemDesc, CItemInfo__GetItemDesc_hook);
    ATTACH_HOOK(CSkillInfo__LoadSkill, CSkillInfo__LoadSkill_hook);
    PatchJmp(CUIQuestInfoDetail__Draw_jmp, reinterpret_cast<uintptr_t>(&CUIQuestInfoDetail__Draw_hook)); // replace "Low Level Quest"
#endif
}