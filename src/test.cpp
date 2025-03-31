#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#define HOOK_FUNCTION(name, addr, ...) \
    typedef int (__fastcall* name##_t)(void*, void*, __VA_ARGS__); \
    static auto name##_ptr = reinterpret_cast<name##_t>(addr); \
    static name##_t name##_hook = [](void* _ECX, void* _EDX, __VA_ARGS__) -> int { \
        DEBUG_MESSAGE(#name " %d", *reinterpret_cast<int*>(pSkill)); \
        return 0; \
    }; \
    AttachHook(reinterpret_cast<void**>(&name##_ptr), CastHook(name##_hook)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", #name "_hook", name##_ptr), false)


class CSkillInfo : public TSingleton<CSkillInfo, 0x00C63EA8> {
public:
    void* GetSkill(int nSkillID) {
        return reinterpret_cast<void* (__thiscall*)(CSkillInfo*, int)>(0x006F1BB0)(this, nSkillID);
    }
};


static uintptr_t DoActiveSkill__EndPrologue = 0x009445F1;
static uintptr_t DoActiveSkill__SetUpVariable_s = 0x00944A4B;

static uintptr_t DoActiveSkill__SetUpVariable_e = 0x00944AD9;
static uintptr_t DoActiveSkill__GetSkillLevel_s = 0x00945980;

static uintptr_t DoActiveSkill__GetSkillLevel_e = 0x00945B5B;
static uintptr_t DoActiveSkill__JumpTable = 0x00945E02;

static uintptr_t DoActiveSkill__MonsterRiderCheck_s = 0x009474D6;
static uintptr_t DoActiveSkill__MonsterRiderCheck_e = 0x0094773F;


static auto CUserLocal__DoActiveSkill = reinterpret_cast<int (__thiscall*)(void*, int, unsigned int, int*)>(0x009445B0);

int __fastcall CUserLocal__DoActiveSkill_hook(void* _ECX, void* _EDX, int nSkillID, unsigned int nScanCode, int* pnConsumeCheck) {
    for (int i = 0; i < 99999999; ++i) {
        if (!CSkillInfo::GetInstance()->GetSkill(i)) {
            continue;
        }
        CUserLocal__DoActiveSkill(_ECX, i, nScanCode, pnConsumeCheck);
    }
    return 0;
}


void AttachTest() {
    // skip checks in CUserLocal::DoActiveSkill
    PatchJmp(DoActiveSkill__EndPrologue, DoActiveSkill__SetUpVariable_s);
    PatchJmp(DoActiveSkill__SetUpVariable_e, DoActiveSkill__GetSkillLevel_s);
    PatchJmp(DoActiveSkill__GetSkillLevel_e, DoActiveSkill__JumpTable);
    PatchJmp(DoActiveSkill__MonsterRiderCheck_s, DoActiveSkill__MonsterRiderCheck_e);

    // trigger
    ATTACH_HOOK(CUserLocal__DoActiveSkill, CUserLocal__DoActiveSkill_hook);


    // hook relevant functions
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_MagicAttack, 0x0093A010, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_Heal, 0x0093A830, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_MeleeAttack, 0x0093B210, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_ShootAttack, 0x0093B520, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_VehicleShootAttack, 0x0093BA30, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_OpenGate, 0x0093BB90, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_TownPortal, 0x0093BE00, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_Summon, 0x0093C0F0, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_RocketBooster, 0x0093C980, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_SmokeShell, 0x0093CC30, void* pSkill, int nSLV);
    // HOOK_FUNCTION(CUserLocal__DoActiveSkill_Flying, 0x0093CEC0, int nSkillID);
    // HOOK_FUNCTION(CUserLocal__DoActiveSkill_DamageMeter, 0x0093D0D0, int nSkillID);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_NotPrepare_Bomb, 0x0093D900, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_MobCapture, 0x0093DB00, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_StatChangeAdmin, 0x0093E0E0, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_RecoveryAura, 0x0093E5C0, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_CombatStep, 0x0093EE00, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_SummonMonster, 0x0093EFF0, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_WeaponBooster, 0x0093F960, void* pSkill, int nSLV, int nWT1, int nWT2, int nWT3, int nWT4);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_Aura, 0x0093FB10, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_RepeatSkill, 0x0093FC10, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_BoundJump, 0x0093FDF0, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_HookAndHit, 0x00941130, void* pSkill, int nSLV);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_Prepare, 0x00941710, void* pSkill, int nSLV, int nScanCode);
    HOOK_FUNCTION(CUserLocal__DoActiveSkill_MesoExplosion, 0x00942200, void* pSkill, int nSLV);
    // HOOK_FUNCTION(CUserLocal__DoActiveSkill_StatChange, 0x00943C20, void* pSkill, int nSLV, unsigned int dwTargetFlag);
    HOOK_FUNCTION(CUserLocal__TryRegisterTelepoort, 0x00913690, void* pSkill, int nSLV, char* sPortalName, char* sTargetPortalName, int bForced);


    // special handling
    typedef int (__fastcall* CUserLocal__DoActiveSkill_Flying_t)(void*, void*, int);
    static auto CUserLocal__DoActiveSkill_Flying_ptr = reinterpret_cast<CUserLocal__DoActiveSkill_Flying_t>(0x0093CEC0);
    static CUserLocal__DoActiveSkill_Flying_t CUserLocal__DoActiveSkill_Flying_hook = [](void* _ECX, void* _EDX, int nSkillID) -> int {
        DEBUG_MESSAGE("CUserLocal__DoActiveSkill_Flying %d", nSkillID);
        return 0;
    };
    AttachHook(reinterpret_cast<void**>(&CUserLocal__DoActiveSkill_Flying_ptr), CastHook(CUserLocal__DoActiveSkill_Flying_hook)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", "CUserLocal__DoActiveSkill_Flying_hook", CUserLocal__DoActiveSkill_Flying_ptr), false);

    typedef int (__fastcall* CUserLocal__DoActiveSkill_DamageMeter_t)(void*, void*, int);
    static auto CUserLocal__DoActiveSkill_DamageMeter_ptr = reinterpret_cast<CUserLocal__DoActiveSkill_DamageMeter_t>(0x0093D0D0);
    static CUserLocal__DoActiveSkill_DamageMeter_t CUserLocal__DoActiveSkill_DamageMeter_hook = [](void* _ECX, void* _EDX, int nSkillID) -> int {
        DEBUG_MESSAGE("CUserLocal__DoActiveSkill_DamageMeter %d", nSkillID);
        return 0;
    };
    AttachHook(reinterpret_cast<void**>(&CUserLocal__DoActiveSkill_DamageMeter_ptr), CastHook(CUserLocal__DoActiveSkill_DamageMeter_hook)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", "CUserLocal__DoActiveSkill_DamageMeter_hook", CUserLocal__DoActiveSkill_DamageMeter_ptr), false);

    typedef int (__fastcall* CUserLocal__DoActiveSkill_StatChange_t)(void*, void*, void*, int, unsigned int);
    static auto CUserLocal__DoActiveSkill_StatChange_ptr = reinterpret_cast<CUserLocal__DoActiveSkill_StatChange_t>(0x00943C20);
    static CUserLocal__DoActiveSkill_StatChange_t CUserLocal__DoActiveSkill_StatChange_hook = [](void* _ECX, void* _EDX, void* pSkill, int nSLV, unsigned int dwTargetFlag) -> int {
        DEBUG_MESSAGE("CUserLocal__DoActiveSkill_StatChange %d, dwTargetFlag %d", *reinterpret_cast<int*>(pSkill), dwTargetFlag);
        return 0;
    };
    AttachHook(reinterpret_cast<void**>(&CUserLocal__DoActiveSkill_StatChange_ptr), CastHook(CUserLocal__DoActiveSkill_StatChange_hook)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", "CUserLocal__DoActiveSkill_StatChange_hook", CUserLocal__DoActiveSkill_StatChange_ptr), false);
}
