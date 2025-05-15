#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

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

struct EQUIPITEM {
    MEMBER_AT(ZtlSecure<unsigned char>, 0xA0, nRUC)
    MEMBER_AT(short, 0x1C0, nirPoison)
    MEMBER_AT(short, 0x1C2, nirIce)
    MEMBER_AT(short, 0x1C4, nirFire)
    MEMBER_AT(short, 0x1C6, nirLight)
    MEMBER_AT(short, 0x1C8, nirHoly)
    MEMBER_AT(int, 0x264, nEnchantCategory)
};

class GW_ItemSlotBase;

struct CharacterData;

static auto CWvsContext__ValidateAdditionalItemEffect = reinterpret_cast<void (__thiscall*)(void*)>(0x009E6D30);

void __fastcall CWvsContext__ValidateAdditionalItemEffect_hook(void* _ECX, void* _EDX) {
    CWvsContext__ValidateAdditionalItemEffect(_ECX);
    CharacterData* cd = reinterpret_cast<ZRef<CharacterData>*>(reinterpret_cast<uintptr_t>(_ECX) + 0x20C8)->p;
    if (!cd) {
        return;
    }
    GW_ItemSlotBase* pWeapon = reinterpret_cast<ZRef<GW_ItemSlotBase>*>(reinterpret_cast<uintptr_t>(cd) + 0xF9)[11].p;
    if (!pWeapon) {
        return;
    }

    // TSecType<long>::GetData(&pe->nItemID);
    int nItemID = reinterpret_cast<int (__thiscall*)(void*)>(0x0042B750)(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pWeapon) + 0xC));

    // CItemInfo::GetEquipItem(CItemInfo::GetInstance(), nItemID);
    EQUIPITEM* pEquipItem = reinterpret_cast<EQUIPITEM* (__thiscall*)(CItemInfo*, int)>(0x005C0820)(CItemInfo::GetInstance(), nItemID);

    // Apply element boost
    int* aElemBoost = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(cd) + 0x74D);
    aElemBoost[POISON] = pEquipItem->nirPoison();
    aElemBoost[ICE] = pEquipItem->nirIce();
    aElemBoost[FIRE] = pEquipItem->nirFire();
    aElemBoost[LIGHT] = pEquipItem->nirLight();
    aElemBoost[HOLY] = pEquipItem->nirHoly();
}


static auto get_damage_adjusted_by_elemAttr = reinterpret_cast<long double (__cdecl*)(long double, int, long double, long double)>(0x007231E0);

long double __cdecl get_damage_adjusted_by_elemAttr_hook(long double damage, int nAttr, long double dAdjust, long double dBoost) {
    long double result = get_damage_adjusted_by_elemAttr(damage, nAttr, dAdjust, 0.0);
    if (dBoost <= 0) {
        return result;
    }
    return result * dBoost;
}


class CUIToolTip;

static auto CUIToolTip__SetToolTip_Equip_Basic = reinterpret_cast<void (__thiscall*)(CUIToolTip*, void*)>(0x008A0BD0);

static auto CUIToolTip__PrintValue = reinterpret_cast<void (__thiscall*)(CUIToolTip*, int, int, ZXString<char>, int)>(0x00891230);

void PrintElementDamage(CUIToolTip* pUIToolTip, const char* sText, int nValue) {
    if (nValue == 100) {
        return;
    }
    ZXString<char> sProperty;
    // ZXString<char>::ZXString<char>(&sProperty, sText, -1);
    reinterpret_cast<void (__thiscall*)(ZXString<char>*, const char*, int)>(0x0042D230)(&sProperty, sText, -1);
    CUIToolTip__PrintValue(pUIToolTip, 2, nValue, sProperty, 1);
}

void __fastcall CUIToolTip__SetToolTip_Equip_Basic_hook(CUIToolTip* pThis, void* _EDX, GW_ItemSlotBase* pe) {
    CUIToolTip__SetToolTip_Equip_Basic(pThis, pe);

    // TSecType<long>::GetData(&pe->nItemID);
    int nItemID = reinterpret_cast<int (__thiscall*)(void*)>(0x0042B750)(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pe) + 0xC));

    // CItemInfo::GetEquipItem(CItemInfo::GetInstance(), nItemID);
    EQUIPITEM* pEquipItem = reinterpret_cast<EQUIPITEM* (__thiscall*)(CItemInfo*, int)>(0x005C0820)(CItemInfo::GetInstance(), nItemID);
    PrintElementDamage(pThis, "POISON DAMAGE :", pEquipItem->nirPoison());      // incRMAS
    PrintElementDamage(pThis, "ICE DAMAGE :", pEquipItem->nirIce());            // incRMAI
    PrintElementDamage(pThis, "FIRE DAMAGE :", pEquipItem->nirFire());          // incRMAF
    PrintElementDamage(pThis, "LIGHTNING DAMAGE :", pEquipItem->nirLight());    // incRMAL
    PrintElementDamage(pThis, "HOLY DAMAGE :", pEquipItem->nirHoly());          // incRMAH

    // Skip printing nRUC with Patch1(0x008A13DB, 0xEB);
    unsigned char nRUC = pEquipItem->nRUC().Fuse();
    if (nRUC && (pEquipItem->nEnchantCategory() & 2) == 0) {
         ZXString<char> sRUC;
        // ZXString<char>::ZXString<char>(&sRUC, "NUMBER OF UPGRADES AVAILABLE :", -1);
        reinterpret_cast<void (__thiscall*)(ZXString<char>*, const char*, int)>(0x0042D230)(&sRUC, "NUMBER OF UPGRADES AVAILABLE :", -1);
        CUIToolTip__PrintValue(pThis, 1, nRUC, sRUC, 1);
    }
}


void AttachElementalDamageMod() {
    // Hijack CharacterData->aElemBoost to store incRMA_ properties
    ATTACH_HOOK(CWvsContext__ValidateAdditionalItemEffect, CWvsContext__ValidateAdditionalItemEffect_hook);

    // Fix damage calculation to use incRMA_ values correctly
    ATTACH_HOOK(get_damage_adjusted_by_elemAttr, get_damage_adjusted_by_elemAttr_hook);

    // Show elemental wand/staff effects in tooltip
    ATTACH_HOOK(CUIToolTip__SetToolTip_Equip_Basic, CUIToolTip__SetToolTip_Equip_Basic_hook);
    Patch1(0x008A13DB, 0xEB); // skip printing nRUC in CUIToolTip::SetToolTip_Equip_Basic
}