#include <math.h>

#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


class CVecCtrlUser {
public:
    MEMBER_AT(ZtlSecure<double>, 0x50, m_apvx) // m_ap->_ZtlSecure_vx
    MEMBER_AT(ZtlSecure<double>, 0x68, m_apvy) // m_ap->_ZtlSecure_vy
    MEMBER_AT(void*, 0x1A0, m_pfh)
    MEMBER_AT(ZtlSecure<int>, 0x1E8, m_nInputX) // _ZtlSecure_nInputX
    MEMBER_AT(ZtlSecure<int>, 0x1F4, m_nInputY) // _ZtlSecure_nInputY
    MEMBER_AT(int, 0x2F8, m_bUserFlyingSkill)
};


void __stdcall HandleFlyingDash(CVecCtrlUser* pVecCtrl) {
    // pVecCtrl->m_pCurAttrShoe.p->flySpeed;
    void* pCurAttrShoe = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pVecCtrl) + 0x240);
    void* pFlySpeed = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pCurAttrShoe) + 0x84);
    // TSecType<double>::GetData(pFlySpeed);
    double dFlySpeed = reinterpret_cast<double (__thiscall*)(void*)>(0x0063F500)(pFlySpeed);
    // pCurAttrShoe->flySpeed * constants->dFlySpeed * constants->dFlyJumpDec * CONSTANT
    double dJumpVelocity = dFlySpeed * 200.0 * 0.35 * 10.0;

    int nInputX = pVecCtrl->m_nInputX().Fuse();
    int nInputY = pVecCtrl->m_nInputY().Fuse();
    double dModifier = sqrt(nInputX * nInputX + nInputY * nInputY);
    if (dModifier != 0) {
        pVecCtrl->m_apvx().Tear(dJumpVelocity * (nInputX / dModifier));
        pVecCtrl->m_apvy().Tear(dJumpVelocity * (nInputY / dModifier));
    }
}


static auto CVecCtrlUser__IsUserFlying = reinterpret_cast<int (__thiscall*)(void*)>(0x009A0CB0);

int __fastcall CVecCtrlUser__IsUserFlying_hook(void* _ECX, void* _EDX) {
    return reinterpret_cast<CVecCtrlUser*>(_ECX)->m_bUserFlyingSkill();
}


static auto CUserLocal__Jump = reinterpret_cast<void (__thiscall*)(void*, int)>(0x0090A1D0);

void __fastcall CUserLocal__Jump_hook(void* _ECX, void* _EDX, int bEnforced) {
    CUserLocal__Jump(_ECX, bEnforced);
    // Check if on the ground (has foothold), or on ladder/rope (CVecCtrl::GetLadderOrRope)
    CUserLocal* pUser = reinterpret_cast<CUserLocal*>(_ECX);
    CVecCtrlUser* pVecCtrl = reinterpret_cast<CVecCtrlUser*>(&static_cast<IWzVector2D*>(pUser->m_pvc())[-3]);
    if (pVecCtrl->m_pfh() || reinterpret_cast<void* (__thiscall*)(CVecCtrlUser*)>(0x004BBE80)(pVecCtrl)) {
        return;
    }
    if (!bEnforced) {
        pVecCtrl->m_bUserFlyingSkill() = 1;
    }
}


static auto CUserLocal__FallDown = reinterpret_cast<void (__thiscall*)(void*)>(0x0090A420);

void __fastcall CUserLocal__FallDown_hook(void* _ECX, void* _EDX) {
    CUserLocal__FallDown(_ECX);
    // Stop flying (falldown is called when down key is pressed during jump)
    CUserLocal* pUser = reinterpret_cast<CUserLocal*>(_ECX);
    CVecCtrlUser* pVecCtrl = reinterpret_cast<CVecCtrlUser*>(&static_cast<IWzVector2D*>(pUser->m_pvc())[-3]);
    if (pVecCtrl->m_bUserFlyingSkill()) {
        if (pVecCtrl->m_nInputX().Fuse()) {
            HandleFlyingDash(pVecCtrl); // downwards diagonal dash
        } else {
            pVecCtrl->m_bUserFlyingSkill() = 0;
        }
    } else {
        pVecCtrl->m_bUserFlyingSkill() = 1;
    }
}


static uintptr_t CVecCtrl__JustJump_jmp = 0x00994352;
static uintptr_t CVecCtrl__JustJump_ret = 0x009943B4;

void __declspec(naked) CVecCtrl__JustJump_hook() {
    __asm {
        push    esi                         ; CVecCtrl* pVecCtrl = this;
        call    HandleFlyingDash
        jmp     [ CVecCtrl__JustJump_ret ]
    }
}


void AttachFlyingMod() {
    ATTACH_HOOK(CVecCtrlUser__IsUserFlying, CVecCtrlUser__IsUserFlying_hook);
    ATTACH_HOOK(CUserLocal__Jump, CUserLocal__Jump_hook);
    ATTACH_HOOK(CUserLocal__FallDown, CUserLocal__FallDown_hook);
    PatchJmp(CVecCtrl__JustJump_jmp, reinterpret_cast<uintptr_t>(&CVecCtrl__JustJump_hook));

    // CVecCtrl::FallDown - fall through footholds instead of jumping down
    PatchStr(0x00993E5B, "\xD9\xEE\x90"); // fld qword ptr [edi+70h] -> fldz; nop : ignore jump speed
    PatchNop(0x00993E74, 0x00993E8A); // skip resetting vx on fall down
    PatchStr(0x00993E74, "\x89\x46\x78"); // overwritten instruction for this->m_ap._ZtlSecureTear_vy_CS = eax

    // CVecCtrlUser::CollisionDetectFloat - ignore footholds while flying
    PatchJmp(0x009A1BEE, 0x009A1C12); // skip check for nMapType
}