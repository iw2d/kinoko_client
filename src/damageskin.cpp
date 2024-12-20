#include <map>

#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


struct DamageSkinProp {
    IWzPropertyPtr pNoRed0;
    IWzPropertyPtr pNoRed1;
    IWzPropertyPtr pNoCri0;
    IWzPropertyPtr pNoCri1;
};

static std::map<int, DamageSkinProp> g_mDamageSkinProp;
static int g_nDamageSkin = 0;

void LoadDamageSkin() {
    DEBUG_MESSAGE("LoadDamageSkin");
    Ztl_variant_t vEmpty;
    Ztl_variant_t vDamageSkin;
    CHECK_HRESULT(get_rm()->raw_GetObject(L"Effect/BasicEff.img/damageSkin", vEmpty, vEmpty, &vDamageSkin));
    IWzPropertyPtr pDamageSkin = IWzPropertyPtr(vDamageSkin.GetUnknown(false, false));
    // iterate damage skins
    IUnknownPtr pEnumUnknown;
    CHECK_HRESULT(pDamageSkin->get__NewEnum(&pEnumUnknown));
    IEnumVARIANTPtr pEnum = IEnumVARIANTPtr(pEnumUnknown);
    while (1) {
        VARIANT rgVar[1];
        ULONG uCeltFetched;
        if (FAILED(pEnum->Next(1, rgVar, &uCeltFetched)) || uCeltFetched == 0) {
            break;
        }
        // load damage skin props
        int nID = wcstol(rgVar[0].bstrVal, nullptr, 10);
        Ztl_variant_t vProp;
        CHECK_HRESULT(pDamageSkin->get_item(rgVar[0].bstrVal, &vProp));
        IWzPropertyPtr pProp = IWzPropertyPtr(vProp.GetUnknown(false, false));
        Ztl_variant_t vNoRed0, vNoRed1, vNoCri0, vNoCri1;
        CHECK_HRESULT(pProp->get_item(L"NoRed0", &vNoRed0));
        CHECK_HRESULT(pProp->get_item(L"NoRed1", &vNoRed1));
        CHECK_HRESULT(pProp->get_item(L"NoCri0", &vNoCri0));
        CHECK_HRESULT(pProp->get_item(L"NoCri1", &vNoCri1));
        g_mDamageSkinProp[nID] = DamageSkinProp {
            IWzPropertyPtr(vNoRed0.GetUnknown(false, false)),
            IWzPropertyPtr(vNoRed1.GetUnknown(false, false)),
            IWzPropertyPtr(vNoCri0.GetUnknown(false, false)),
            IWzPropertyPtr(vNoCri1.GetUnknown(false, false))
        };
    }
}


static uintptr_t CMob__OnHit_jmp = 0x006534DD; // replace CMob::ShowDamage call
static uintptr_t CMob__OnHit_ret = 0x006534E2;

static auto CMob__ShowDamage = reinterpret_cast<void (__thiscall*)(void*, int, int, int, int, int, int)>(0x0063C950);

void __stdcall CMob__ShowDamage_helper(void* pMob, unsigned int dwCharacterId, int nDamage, int nAttackIdx, int bCriticalAttack, int bHalfHeight, int ZigZagDamage, int bAdjustHeight) {
    g_nDamageSkin = 0; // set damage skin here using dwCharacterId
    CMob__ShowDamage(pMob, nDamage, nAttackIdx, bCriticalAttack, bHalfHeight, ZigZagDamage, bAdjustHeight);
    g_nDamageSkin = 0;
}

void __declspec(naked) CMob__OnHit_hook() {
    __asm {
        mov     eax, [ esp + 0x7C + 0x4 ]       ; dwCharacterId
        push    eax
        push    ecx                             ; CMob*
        call    CMob__ShowDamage_helper
        jmp     [ CMob__OnHit_ret ]
    };
}


static uintptr_t CAnimationDisplayer__Effect_HP_jmp = 0x00444F58; // bCriticalAttack check
static uintptr_t CAnimationDisplayer__Effect_HP_ret = 0x00444F91;

void __stdcall CAnimationDisplayer__Effect_HP_helper(void* pThis, int bCriticalAttack, IWzPropertyPtr& pEffSmall, IWzPropertyPtr& pEffLarge) {
    if (auto search = g_mDamageSkinProp.find(g_nDamageSkin); search != g_mDamageSkinProp.end()) {
        if (bCriticalAttack & 1) {
            pEffSmall = search->second.pNoCri0;
            pEffLarge = search->second.pNoCri1;
        } else {
            pEffSmall = search->second.pNoRed0;
            pEffLarge = search->second.pNoRed1;
        }
    } else {
        if (bCriticalAttack & 1) {
            pEffSmall = *reinterpret_cast<IWzPropertyPtr*>(reinterpret_cast<uintptr_t>(pThis) + 0x1BC); // this->m_pEffNo_Cri0
            pEffLarge = *reinterpret_cast<IWzPropertyPtr*>(reinterpret_cast<uintptr_t>(pThis) + 0x1C0); // this->m_pEffNo_Cri1
        } else {
            pEffSmall = *reinterpret_cast<IWzPropertyPtr*>(reinterpret_cast<uintptr_t>(pThis) + 0x1A4); // this->m_pEffNo_Red0
            pEffLarge = *reinterpret_cast<IWzPropertyPtr*>(reinterpret_cast<uintptr_t>(pThis) + 0x1A8); // this->m_pEffNo_Red1
        }
    }
}

void __declspec(naked) CAnimationDisplayer__Effect_HP_hook() {
    __asm {
        // save registers
        pushad
        pushfd
        // call helper function
        lea     eax, [ ebp + 0x5C - 0x40 ]  ; pEffLarge
        push    eax
        lea     eax, [ ebp + 0x5C - 0x48 ]  ; pEffSmall
        push    eax
        push    edi                         ; bCriticalAttack
        push    esi                         ; CAnimationDisplayer*
        call    CAnimationDisplayer__Effect_HP_helper
        // restore registers and return
        popfd
        popad
        jmp     [ CAnimationDisplayer__Effect_HP_ret ]
    };
}


void AttachDamageSkinMod() {
    PatchJmp(CMob__OnHit_jmp, reinterpret_cast<uintptr_t>(&CMob__OnHit_hook));
    PatchJmp(CAnimationDisplayer__Effect_HP_jmp, reinterpret_cast<uintptr_t>(&CAnimationDisplayer__Effect_HP_hook));
}