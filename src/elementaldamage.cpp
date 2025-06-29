#include "pch.h"
#include "hook.h"
#include "debug.h"
#include "common/secure.h"
#include "common/dbbasic.h"
#include "common/iteminfo.h"
#include "wvs/wvscontext.h"
#include "wvs/uitooltip.h"
#include <cstdint>


enum {
    PHYSICAL = 0,
    ICE = 1,
    FIRE = 2,
    LIGHT = 3,
    POISON = 4,
    HOLY = 5,
    DARK = 6,
    UNDEAD = 7,
    COUNT = 8
};

static auto CWvsContext__ValidateAdditionalItemEffect = reinterpret_cast<void(__thiscall*)(CWvsContext*)>(0x009E6D30);

void __fastcall CWvsContext__ValidateAdditionalItemEffect_hook(CWvsContext* pThis, void* _EDX) {
    CWvsContext__ValidateAdditionalItemEffect(pThis);
    if (!pThis->m_pCharacterData) {
        return;
    }
    ZRef<GW_ItemSlotBase> pWeapon = pThis->m_pCharacterData->aEquipped[11];
    if (!pWeapon) {
        return;
    }
    int32_t nItemID = pWeapon->nItemID.GetData();
    EQUIPITEM* pEquipItem = CItemInfo::GetInstance()->GetEquipItem(nItemID);

    // Apply element boost
    auto aElemBoost = pThis->m_pCharacterData->aElemBoost;
    aElemBoost[POISON] = pEquipItem->nirPoison;
    aElemBoost[ICE] = pEquipItem->nirIce;
    aElemBoost[FIRE] = pEquipItem->nirFire;
    aElemBoost[LIGHT] = pEquipItem->nirLight;
    aElemBoost[HOLY] = pEquipItem->nirHoly;
}


static auto get_damage_adjusted_by_elemAttr = reinterpret_cast<double(__cdecl*)(double, int32_t, double, double)>(0x007231E0);

double __cdecl get_damage_adjusted_by_elemAttr_hook(double damage, int32_t nAttr, double dAdjust, double dBoost) {
    double result = get_damage_adjusted_by_elemAttr(damage, nAttr, dAdjust, 0.0);
    if (dBoost <= 0) {
        return result;
    }
    return result * dBoost;
}


static auto CUIToolTip__SetToolTip_Equip_Basic = reinterpret_cast<void(__thiscall*)(CUIToolTip*, void*)>(0x008A0BD0);
static auto CUIToolTip__PrintValue = reinterpret_cast<void(__thiscall*)(CUIToolTip*, int32_t, int32_t, ZXString<char>, int32_t)>(0x00891230);

void PrintElementDamage(CUIToolTip* pUIToolTip, const char* sText, int32_t nValue) {
    if (nValue == 100) {
        return;
    }
    ZXString<char> sProperty(sText);
    CUIToolTip__PrintValue(pUIToolTip, 2, nValue, sProperty, 1);
}

void __fastcall CUIToolTip__SetToolTip_Equip_Basic_hook(CUIToolTip* pThis, void* _EDX, GW_ItemSlotBase* pe) {
    CUIToolTip__SetToolTip_Equip_Basic(pThis, pe);
    int32_t nItemID = pe->nItemID.GetData();
    EQUIPITEM* pEquipItem = CItemInfo::GetInstance()->GetEquipItem(nItemID);
    if (!pEquipItem) {
        return;
    }
    PrintElementDamage(pThis, "POISON DAMAGE :", pEquipItem->nirPoison);   // incRMAS
    PrintElementDamage(pThis, "ICE DAMAGE :", pEquipItem->nirIce);         // incRMAI
    PrintElementDamage(pThis, "FIRE DAMAGE :", pEquipItem->nirFire);       // incRMAF
    PrintElementDamage(pThis, "LIGHTNING DAMAGE :", pEquipItem->nirLight); // incRMAL
    PrintElementDamage(pThis, "HOLY DAMAGE :", pEquipItem->nirHoly);       // incRMAH

    // Skip printing nRUC in original code with Patch1(0x008A13DB, 0xEB);
    uint8_t nRUC = pEquipItem->nRUC.Fuse();
    if (nRUC && (pEquipItem->nEnchantCategory & 2) == 0) {
        ZXString<char> sRUC("NUMBER OF UPGRADES AVAILABLE :");

        // Remaining upgrade count - we can't cast to ZtlSecure<unsigned char>* here because GW_ItemSlotBase is packed to 1 byte alignment
        nRUC = ZtlSecureFuse<unsigned char>(
                reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(pe) + 0x49),
                *reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(pe) + 0x4B));
        CUIToolTip__PrintValue(pThis, 1, nRUC, sRUC, 1);
    }
}


void AttachElementalDamageMod() {
    ATTACH_HOOK(CWvsContext__ValidateAdditionalItemEffect, CWvsContext__ValidateAdditionalItemEffect_hook); // Hijack CharacterData->aElemBoost to store incRMA_ properties
    ATTACH_HOOK(get_damage_adjusted_by_elemAttr, get_damage_adjusted_by_elemAttr_hook); // Fix damage calculation to use incRMA_ values correctly
    ATTACH_HOOK(CUIToolTip__SetToolTip_Equip_Basic, CUIToolTip__SetToolTip_Equip_Basic_hook); // Show elemental wand/staff effects in tooltip

    Patch1(0x008A13DB, 0xEB); // skip printing nRUC in CUIToolTip::SetToolTip_Equip_Basic
}