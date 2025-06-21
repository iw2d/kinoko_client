#include "pch.h"
#include "hook.h"
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zcom.h"
#include "wzlib/gr2d.h"
#include "wvs/util.h"
#include "wvs/wvscontext.h"
#include "wvs/temporarystatview.h"
#include <timeapi.h>
#include <cstdint>

#define COOLTIME_OFFSET_HEIGHT 40


static CTemporaryStatView g_tsvCooltime;

static auto CTemporaryStatView__Clear = reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075C330);

void __fastcall CTemporaryStatView__Clear_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Clear(pThis);
    CTemporaryStatView__Clear(&g_tsvCooltime);
}

static auto CTemporaryStatView__Show = reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075C6A0);

void __fastcall CTemporaryStatView__Show_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Show(pThis);
    CTemporaryStatView__Show(&g_tsvCooltime);
}

static auto CTemporaryStatView__Hide = reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075C8C0);

void __fastcall CTemporaryStatView__Hide_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Hide(pThis);
    CTemporaryStatView__Hide(&g_tsvCooltime);
}

static auto CTemporaryStatView__Update = reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075DC50);

void __fastcall CTemporaryStatView__Update_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Update(pThis);
    CTemporaryStatView__Update(&g_tsvCooltime);
}

static auto CTemporaryStatView__AdjustPosition = reinterpret_cast<void(__thiscall*)(CTemporaryStatView*)>(0x0075CAD0);

void AdjustPositionWithOffset(CTemporaryStatView* tsv, int32_t nOffset) {
    CTemporaryStatView__AdjustPosition(tsv);
    auto pos = tsv->m_lTemporaryStat.GetHeadPosition();
    while (pos) {
        auto pNext = ZList<ZRef<CTemporaryStatView::TEMPORARY_STAT>>::GetNext(pos);
        DEBUG_MESSAGE("NEXT 0x%08X", &pNext);
        Ztl_variant_t vEmpty;
        CHECK_HR(pNext->pLayer->raw_RelOffset(0, nOffset, vEmpty, vEmpty));
        CHECK_HR(pNext->pLayerShadow->raw_RelOffset(0, nOffset, vEmpty, vEmpty));
    }
}

void __fastcall CTemporaryStatView__AdjustPosition_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__AdjustPosition(pThis);
    AdjustPositionWithOffset(&g_tsvCooltime, COOLTIME_OFFSET_HEIGHT);
}


typedef void(__thiscall* CWvsContext__SetSkillCooltimeOver_t)(CWvsContext*, int, int);
static auto CWvsContext__SetSkillCooltimeOver = reinterpret_cast<void(__thiscall*)(CWvsContext*, int32_t, int32_t)>(0x009DB0D0);

void __fastcall CWvsContext__SetSkillCooltimeOver_hook(CWvsContext* pThis, void* _EDX, int32_t nSkillID, int32_t tTimeOver) {
    CWvsContext__SetSkillCooltimeOver(pThis, nSkillID, tTimeOver);
    // Battleship durability
    if (nSkillID == 5221999) {
        return;
    }
    int32_t nRemain = tTimeOver - timeGetTime();
    auto pos = g_tsvCooltime.m_lTemporaryStat.GetHeadPosition();
    while (pos) {
        auto pNext = ZList<ZRef<CTemporaryStatView::TEMPORARY_STAT>>::GetNext(pos);
        if (pNext->nID == nSkillID) {
            pNext->tLeft = nRemain;
            return;
        }
    }
    UINT128 uFlag;
    uFlag.setBitNumber((nSkillID % 127) + 1, 1); // hope for no collisions
    g_tsvCooltime.SetTemporary(TSV_SKILL, nSkillID, nRemain, uFlag, ZXString<char>(), 0, 0);
    AdjustPositionWithOffset(&g_tsvCooltime, COOLTIME_OFFSET_HEIGHT);
}

static auto CWvsContext__RemoveSkillCooltimeReset = reinterpret_cast<void(__thiscall*)(CWvsContext*, int32_t)>(0x009CCF80);

void __fastcall CWvsContext__RemoveSkillCooltimeReset_hook(CWvsContext* pThis, void* _EDX, int nSkillID) {
    CWvsContext__RemoveSkillCooltimeReset(pThis, nSkillID);
    g_tsvCooltime.ResetTemporary(TSV_SKILL, nSkillID);
}


static auto TEMPORARY_STAT__UpdateShadowIndex = reinterpret_cast<void(__thiscall*)(CTemporaryStatView::TEMPORARY_STAT*)>(0x0075D560);

void __fastcall TEMPORARY_STAT__UpdateShadowIndex_hook(CTemporaryStatView::TEMPORARY_STAT* pThis, void* _EDX) {
    TEMPORARY_STAT__UpdateShadowIndex(pThis);
}


void AttachTemporaryStatMod() {
    ATTACH_HOOK(CTemporaryStatView__Clear, CTemporaryStatView__Clear_hook);
    ATTACH_HOOK(CTemporaryStatView__Show, CTemporaryStatView__Show_hook);
    ATTACH_HOOK(CTemporaryStatView__Hide, CTemporaryStatView__Hide_hook);
    ATTACH_HOOK(CTemporaryStatView__Update, CTemporaryStatView__Update_hook);
    ATTACH_HOOK(CTemporaryStatView__AdjustPosition, CTemporaryStatView__AdjustPosition_hook);
    ATTACH_HOOK(CWvsContext__SetSkillCooltimeOver, CWvsContext__SetSkillCooltimeOver_hook);
    ATTACH_HOOK(CWvsContext__RemoveSkillCooltimeReset, CWvsContext__RemoveSkillCooltimeReset_hook);
    ATTACH_HOOK(TEMPORARY_STAT__UpdateShadowIndex, TEMPORARY_STAT__UpdateShadowIndex_hook);
}