#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#define COOLTIME_OFFSET_HEIGHT 40


class UINT128 {
public:
    unsigned int m_data[4] = {};

    void setBitNumber(unsigned int bit, unsigned int value) {
        unsigned int flag = 1 << (31 - (bit & 31));
        this->m_data[bit >> 5] |= flag;
        if (!value) {
            this->m_data[bit >> 5] ^= flag;
        }
    }
};
static_assert(sizeof(UINT128) == 0x10);

enum {
    TSV_NONE = 0x0,
    TSV_ITEM = 0x1,
    TSV_SKILL = 0x2,
    TSV_ETC = 0x3,
    TSV_PRIVILEGE = 0x4,
};

class CTemporaryStatView {
public:
    struct TEMPORARY_STAT {
        MEMBER_AT(int, 0x20, nID)
        MEMBER_AT(IWzGr2DLayerPtr, 0x2C, pLayer)
        MEMBER_AT(IWzGr2DLayerPtr, 0x30, pLayerShadow)
        MEMBER_AT(int, 0x40, tLeft)
    };

    ZList<ZRef<TEMPORARY_STAT>> m_lTemporaryStat;

    virtual ~CTemporaryStatView() = default;
};
static_assert(sizeof(CTemporaryStatView) == 0x18);
static_assert(offsetof(CTemporaryStatView, m_lTemporaryStat) == 0x4);


static CTemporaryStatView g_tsvCooltime;

typedef void (__thiscall* CTemporaryStatView__Show_t)(CTemporaryStatView*);
static auto CTemporaryStatView__Show = reinterpret_cast<CTemporaryStatView__Show_t>(0x0075C6A0);

void __fastcall CTemporaryStatView__Show_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Show(pThis);
    CTemporaryStatView__Show(&g_tsvCooltime);
}

typedef void (__thiscall* CTemporaryStatView__Hide_t)(CTemporaryStatView*);
static auto CTemporaryStatView__Hide = reinterpret_cast<CTemporaryStatView__Hide_t>(0x0075C8C0);

void __fastcall CTemporaryStatView__Hide_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Hide(pThis);
    CTemporaryStatView__Hide(&g_tsvCooltime);
}

typedef void (__thiscall* CTemporaryStatView__Update_t)(CTemporaryStatView*);
static auto CTemporaryStatView__Update = reinterpret_cast<CTemporaryStatView__Update_t>(0x0075DC50);

void __fastcall CTemporaryStatView__Update_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__Update(pThis);
    CTemporaryStatView__Update(&g_tsvCooltime);
}

typedef void (__thiscall* CTemporaryStatView__AdjustPosition_t)(CTemporaryStatView*);
static auto CTemporaryStatView__AdjustPosition = reinterpret_cast<CTemporaryStatView__AdjustPosition_t>(0x0075CAD0);

void AdjustPositionOffset(CTemporaryStatView* tsv, int nOffset) {
    CTemporaryStatView__AdjustPosition(tsv);
    auto pNext = tsv->m_lTemporaryStat._m_pHead;
    while (pNext) {
        // auto pCurr = ZList<ZRef<CTemporaryStatView::TEMPORARY_STAT>>::GetNext(&pNext);
        auto pCurr = reinterpret_cast<ZRef<CTemporaryStatView::TEMPORARY_STAT>* (__cdecl*)(ZRef<CTemporaryStatView::TEMPORARY_STAT>**)>(0x0075C180)(&pNext);
        Ztl_variant_t vEmpty;
        CHECK_HRESULT(pCurr->p->pLayer()->raw_RelOffset(0, nOffset, vEmpty, vEmpty));
        CHECK_HRESULT(pCurr->p->pLayerShadow()->raw_RelOffset(0, nOffset, vEmpty, vEmpty));
    }
}

void __fastcall CTemporaryStatView__AdjustPosition_hook(CTemporaryStatView* pThis, void* _EDX) {
    CTemporaryStatView__AdjustPosition(pThis);
    AdjustPositionOffset(&g_tsvCooltime, COOLTIME_OFFSET_HEIGHT);
}



typedef void (__thiscall* CWvsContext__SetSkillCooltimeOver_t)(CWvsContext*, int, int);
static auto CWvsContext__SetSkillCooltimeOver = reinterpret_cast<CWvsContext__SetSkillCooltimeOver_t>(0x009DB0D0);

void __fastcall CWvsContext__SetSkillCooltimeOver_hook(CWvsContext* pThis, void* _EDX, int nSkillID, int tTimeOver) {
    CWvsContext__SetSkillCooltimeOver(pThis, nSkillID, tTimeOver);
    // Battleship Durability
    if (nSkillID == 5221999) {
        return;
    }
    int nRemain = tTimeOver - timeGetTime();
    auto pNext = g_tsvCooltime.m_lTemporaryStat._m_pHead;
    while (pNext) {
        // auto pCurr = ZList<ZRef<CTemporaryStatView::TEMPORARY_STAT>>::GetNext(&pNext);
        auto pCurr = reinterpret_cast<ZRef<CTemporaryStatView::TEMPORARY_STAT>* (__cdecl*)(ZRef<CTemporaryStatView::TEMPORARY_STAT>**)>(0x0075C180)(&pNext);
        if (pCurr->p->nID() == nSkillID) {
            pCurr->p->tLeft() = nRemain;
            return;
        }
    }
    UINT128 uFlag;
    uFlag.setBitNumber((nSkillID % 127) + 1, 1); // pray for no collisions
    // CTemporaryStat::SetTemporary(&g_tsvCooltime, TSV_SKILL, nSkillID, nRemain, uFlag, ZXString<char>(), 0, 0);
    reinterpret_cast<void (__thiscall*)(CTemporaryStatView*, int, int, int, UINT128, ZXString<char>, int, int)>(0x0075FA50)(&g_tsvCooltime, TSV_SKILL, nSkillID, nRemain, uFlag, ZXString<char>(), 0, 0);
    AdjustPositionOffset(&g_tsvCooltime, COOLTIME_OFFSET_HEIGHT);
}

typedef void (__thiscall* CWvsContext__RemoveSkillCooltimeReset_t)(CWvsContext*, int);
static auto CWvsContext__RemoveSkillCooltimeReset = reinterpret_cast<CWvsContext__RemoveSkillCooltimeReset_t>(0x009CCF80);

void __fastcall CWvsContext__RemoveSkillCooltimeReset_hook(CWvsContext* pThis, void* _EDX, int nSkillID) {
    CWvsContext__RemoveSkillCooltimeReset(pThis, nSkillID);
    // CTemporaryStat::ResetTemporary(&g_tsvCooltime, TSV_SKILL, nSkillID);
    reinterpret_cast<void (__thiscall*)(CTemporaryStatView*, int, int)>(0x0075D030)(&g_tsvCooltime, TSV_SKILL, nSkillID);
    AdjustPositionOffset(&g_tsvCooltime, COOLTIME_OFFSET_HEIGHT);
}


void AttachTemporaryStatMod() {
    ATTACH_HOOK(CTemporaryStatView__Show, CTemporaryStatView__Show_hook);
    ATTACH_HOOK(CTemporaryStatView__Hide, CTemporaryStatView__Hide_hook);
    ATTACH_HOOK(CTemporaryStatView__Update, CTemporaryStatView__Update_hook);
    ATTACH_HOOK(CTemporaryStatView__AdjustPosition, CTemporaryStatView__AdjustPosition_hook);

    ATTACH_HOOK(CWvsContext__SetSkillCooltimeOver, CWvsContext__SetSkillCooltimeOver_hook);
    ATTACH_HOOK(CWvsContext__RemoveSkillCooltimeReset, CWvsContext__RemoveSkillCooltimeReset_hook);
}