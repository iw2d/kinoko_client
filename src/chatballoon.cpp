#include "pch.h"
#include "client.h"
#include "hook.h"


class CChatBalloon;

class CUser {
public:
    MEMBER_AT(CChatBalloon, 0x14, m_chatBalloon)
    MEMBER_AT(int, 0x88 + 0x5B8, m_nMoveAction)
    MEMBER_AT(IWzVector2DPtr, 0x88 + 0x18BC, m_pBodyOrigin)
    MEMBER_AT(IWzGr2DLayerPtr, 0x88 + 0x18D8, m_pLayerUnderFace)
};

class CUserPool : public TSingleton<CUserPool, 0x00C687B4> {
public:
    inline CUser* FindRemoteUserByName(const char* sName) {
        return reinterpret_cast<CUser* (__thiscall*)(CUserPool*, const char*)>(0x0094B650)(this, sName); // actually returns CUserRemote*
    }
};


enum {
    CG_Friend = 0x0,
    CG_Party = 0x1,
    CG_Guild = 0x2,
    CG_Alliance = 0x3,
    CG_Couple = 0x4,
    CG_ToCouple = 0x5,
    CG_Expedition = 0x6,
};

enum {
    CHAT_TYPE_NORMAL = 0x0,
    CHAT_TYPE_WHISPER = 0x1,
    CHAT_TYPE_GROUPPARTY = 0x2,
    CHAT_TYPE_GROUPFRIEND = 0x3,
    CHAT_TYPE_GROUPGUILD = 0x4,
    CHAT_TYPE_GROUPALLIANCE = 0x5,
    CHAT_TYPE_COUPLE = 0x6,
    CHAT_TYPE_GAMEDESC = 0x7,
    CHAT_TYPE_TIP = 0x8,
    CHAT_TYPE_NOTICE = 0x9,
    CHAT_TYPE_NOTICE2 = 0xA,
    CHAT_TYPE_ADMINCHAT = 0xB,
    CHAT_TYPE_SYSTEM = 0xC,
    CHAT_TYPE_SPEAKERCHANNEL = 0xD,
    CHAT_TYPE_SPEAKERWORLD = 0xE,
    CHAT_TYPE_SKULLSPEAKER = 0xF,
    CHAT_TYPE_ITEMSPEAKER = 0x10,
    CHAT_TYPE_ITEMSPEAKER_ITEM = 0x11,
    CHAT_TYPE_AVATARMEGAPHONE = 0x12,
    CHAT_TYPE_GACHAPONANNOUNCE = 0x13,
    CHAT_TYPE_CASHGACHAPONANNOUNCE = 0x14,
    CHAT_TYPE_CASHGACHAPON_OPEN_ANNOUNCE = 0x15,
    CHAT_TYPE_CASHGACHAPON_COPY_ANNOUNCE = 0x16,
    CHAT_TYPE_SPEAKERBRIDGE = 0x17,
    CHAT_TYPE_SPEAKERWORLDEX_PREVIEW = 0x18,
    CHAT_TYPE_MOB = 0x19,
    CHAT_TYPE_EXPEDITION = 0x1A,
    CHAT_TYPE_NO = 0x1B,
};


static int g_nNextChatTarget = -1;

void CreateChatBalloonMessage(int nChatTarget, CUser* pUser, ZXString<char>* sText) {
    switch (nChatTarget) {
        case CG_Friend:
        case CG_Party:
        case CG_Guild:
        case CG_Expedition:
            break;
        default:
            return;
    }
    g_nNextChatTarget = nChatTarget;
    bool bDead = (pUser->m_nMoveAction() & 0xFFFFFFFE) == 18;
    IWzGr2DLayerPtr pLayerOverlay = pUser->m_pLayerUnderFace();
    IWzVector2DPtr pVectorOrigin = pUser->m_pBodyOrigin();
    Ztl_bstr_t bsText(sText->_m_pStr);
    // CChatBalloon::MakeBalloon(&pUser->m_chatBalloon(), bsText, pLayerOverlay, pVectorOrigin, 5000, 0x3E8, 0, bDead, 0, 0);
    reinterpret_cast<void (__thiscall*)(CChatBalloon*, Ztl_bstr_t, IWzGr2DLayerPtr, IWzVector2DPtr, int, int, int, int, int, int)>(0x004A84F0)(
        &pUser->m_chatBalloon(),
        bsText,
        pLayerOverlay,
        pVectorOrigin,
        5000, // tTimeout
        0x3E8, // CHATBALLOON_CHARACTER
        0,
        bDead,
        0,
        0
    );
    g_nNextChatTarget = -1;
}

void CreateGroupChatMessage(int nChatTarget, ZXString<char>* sText) {
    int nChatType = CHAT_TYPE_NORMAL;
    switch (nChatTarget) {
        case CG_Friend:
            nChatType = CHAT_TYPE_GROUPFRIEND;
            break;
        case CG_Party:
            nChatType = CHAT_TYPE_GROUPPARTY;
            break;
        case CG_Guild:
            nChatType = CHAT_TYPE_GROUPGUILD;
            break;
        case CG_Alliance:
            nChatType = CHAT_TYPE_GROUPALLIANCE;
            break;
        case CG_Expedition:
            nChatType = CHAT_TYPE_EXPEDITION;
            break;
    }
    // CHATLOG_ADD(sText, nChatType);
    reinterpret_cast<void (__cdecl*)(ZXString<char>*, unsigned short)>(0x004AA000)(sText, nChatType);
}

static uintptr_t CChatBalloon__MakeBalloon_jmp = 0x004A8B02;
static uintptr_t CChatBalloon__MakeBalloon_ret = 0x004A8B07;
static auto get_int32 = reinterpret_cast<int (__cdecl*)(void*, int)>(0x00406830);

int __stdcall CChatBalloon__MakeBalloon_helper(void* v, int nDefault) {
    switch (g_nNextChatTarget) {
        case CG_Friend:
            return 0xFFDD4400; // 0xFFFF9900
        case CG_Party:
            return 0xFFCC3366; // 0xFFFF99CC
        case CG_Guild:
            return 0xFFB17CCE; // 0xFFE1ACFE
        case CG_Expedition:
            return 0xFF00CCCC; // 0xFF7DFFEE
        default:
            return get_int32(v, nDefault);
    }
}

void __declspec(naked) CChatBalloon__MakeBalloon_hook() {
    __asm {
        call    CChatBalloon__MakeBalloon_helper
        jmp     [ CChatBalloon__MakeBalloon_ret ]
    }
}


static uintptr_t CField__OnGroupMessage_jmp = 0x005356B9;
static uintptr_t CField__OnGroupMessage_ret = 0x005356F3;

void __stdcall CField__OnGroupMessage_helper(int nChatTarget, ZXString<char>* sFrom, ZXString<char>* sText) {
    // Add chat log
    CreateGroupChatMessage(nChatTarget, sText);
    // Create chat balloon
    if (!CUserPool::IsInstantiated()) {
        return;
    }
    CUser* pUser = CUserPool::GetInstance()->FindRemoteUserByName(sFrom->_m_pStr);
    if (!pUser) {
        return;
    }
    CreateChatBalloonMessage(nChatTarget, pUser, sText);
}

void __declspec(naked) CField__OnGroupMessage_hook() {
    __asm {
        lea     eax, [ esp + 0x60 - 0x4C ] ; ZXString<char> sText
        push    eax
        lea     eax, [ esp + 0x64 - 0x48 ] ; ZXString<char> sFrom
        push    eax
        push    ebp ; int nChatTarget
        call    CField__OnGroupMessage_helper
        jmp     [ CField__OnGroupMessage_ret ]
    }
}


static uintptr_t CUIStatusBar__SendGroupMessage_jmp = 0x0087FB0F;
static uintptr_t CUIStatusBar__SendGroupMessage_ret = 0x0087FBA0;

void __stdcall CUIStatusBar__SendGroupMessage_helper(int nChatTarget, ZXString<char>* sMsg) {
    ZXString<char> sText;
    // ZXString<char>::Format(&sText, "%s : %s", CUserLocal::GetInstance()->m_sCharacterName()->_m_pStr, sMsg->_m_pStr);
    reinterpret_cast<ZXString<char>* (__cdecl*)(ZXString<char>*, const char*, ...)>(0x00443C70)(&sText, "%s : %s", CUserLocal::GetInstance()->m_sCharacterName()._m_pStr, sMsg->_m_pStr);
    CreateGroupChatMessage(nChatTarget, &sText);
    CreateChatBalloonMessage(nChatTarget, reinterpret_cast<CUser*>(CUserLocal::GetInstance()), &sText);
    // ZXString<char>::~ZXString<char>(&sText);
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*)>(0x00403900)(&sText);
}

void __declspec(naked) CUIStatusBar__SendGroupMessage_hook() {
    __asm {
        mov     eax, [ esp + 0x50 + 8 ] ; const ZXString<char>* sText
        push    eax
        mov     eax, [ esp + 0x54 + 4 ] ; int nChatTarget
        push    eax
        call    CUIStatusBar__SendGroupMessage_helper
        jmp     [ CUIStatusBar__SendGroupMessage_ret ]
    }
}


void AttachChatBalloonMod() {
    PatchJmp(CChatBalloon__MakeBalloon_jmp, reinterpret_cast<uintptr_t>(&CChatBalloon__MakeBalloon_hook));
    PatchJmp(CField__OnGroupMessage_jmp, reinterpret_cast<uintptr_t>(&CField__OnGroupMessage_hook));
    PatchJmp(CUIStatusBar__SendGroupMessage_jmp, reinterpret_cast<uintptr_t>(&CUIStatusBar__SendGroupMessage_hook));
}