#pragma once
#include "ztl/ztl.h"
#include "com/com.h"

#define CHECK_HRESULT(HR) \
    do { \
        HRESULT _hr = (HR); \
        if (FAILED(_hr)) { \
            _com_issue_error(_hr); \
        } \
    } while (0)

#define MEMBER_AT(T, OFFSET, NAME) \
    __forceinline auto& NAME() { \
        return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }


static IWzGr2DPtr& get_gr() {
    return *reinterpret_cast<IWzGr2DPtr*>(0x00C6F430);
}


class CWvsApp : public TSingleton<CWvsApp, 0x00C64314> {
public:
    MEMBER_AT(HWND, 0x4, m_hWnd)
    MEMBER_AT(int, 0x8, m_bPCOMInitialized)
    MEMBER_AT(unsigned int, 0xC, m_dwMainThreadId)
    MEMBER_AT(HHOOK, 0x10, m_hHook)
    MEMBER_AT(int, 0x14, m_bWin9x)
    MEMBER_AT(int, 0x18, m_nOSVersion)
    MEMBER_AT(int, 0x1C, m_nOSMinorVersion)
    MEMBER_AT(int, 0x20, m_nOSBuildNumber)
    MEMBER_AT(ZXString<char>, 0x24, m_sCSDVersion)
    MEMBER_AT(int, 0x28, m_b64BitInfo)
    MEMBER_AT(int, 0x2C, m_tUpdateTime)
    MEMBER_AT(int, 0x30, m_bFirstUpdate)
    MEMBER_AT(ZXString<char>, 0x34, m_sCmdLine)
    MEMBER_AT(int, 0x38, m_nGameStartMode)
    MEMBER_AT(int, 0x3C, m_bAutoConnect)
    MEMBER_AT(int, 0x40, m_bShowAdBalloon)
    MEMBER_AT(int, 0x44, m_bExitByTitleEscape)
    MEMBER_AT(HRESULT, 0x48, m_hrZExceptionCode)
    MEMBER_AT(HRESULT, 0x4C, m_hrComErrorCode)
    MEMBER_AT(int, 0x54, m_nTargetVersion)
    MEMBER_AT(int, 0x58, m_tLastServerIPCheck)
    MEMBER_AT(int, 0x5C, m_tLastServerIPCheck2)
    MEMBER_AT(int, 0x60, m_tLastGGHookingAPICheck)
    MEMBER_AT(int, 0x64, m_tLastSecurityCheck)
    MEMBER_AT(void**, 0x68, m_ahInput)
    MEMBER_AT(int, 0x74, m_tNextSecurityCheck)
    MEMBER_AT(bool, 0x78, m_bEnabledDX9)
    MEMBER_AT(ZArray<unsigned char>, 0x7C, m_pBackupBuffer)
    MEMBER_AT(unsigned int, 0x80, m_dwBackupBufferSize)
    MEMBER_AT(unsigned int, 0x84, m_dwClearStackLog)
    MEMBER_AT(int, 0x88, m_bWindowActive)
};


struct CONFIG_SYSOPT {
    int nSysOpt_Video;
    int nSysOpt_BGMVol;
    int bSysOpt_BGMMute;
    int nSysOpt_SEVol;
    int bSysOpt_SEMute;
    int nSysOpt_ScreenShot;
    int nSysOpt_MouseSpeed;
    int nSysOpt_HPFlash;
    int nSysOpt_MPFlash;
    int bSysOpt_Tremble;
    int nSysOpt_MobInfo;
    int bSysOpt_LargeScreen;
    int bSysOpt_WindowedMode;
    int bSysOpt_Minimap_Normal;
};
static_assert(sizeof(CONFIG_SYSOPT) == 0x38);

class CConfig : public TSingleton<CConfig, 0x00C687AC> {
public:
    void ApplySysOpt(CONFIG_SYSOPT* pSysOpt, int bApplyVideo) {
        reinterpret_cast<void (__thiscall*)(CConfig*, CONFIG_SYSOPT*, int)>(0x004B2300)(this, pSysOpt, bApplyVideo);
    }
};


class COutPacket {
public:
    int m_bLoopback;
    ZArray<unsigned char> m_aSendBuff;
    unsigned int m_uOffset;
    int m_bIsEncryptedByShanda;

    COutPacket(int nType) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, int)>(0x0068D090)(this, nType);
    }
    void Encode1(unsigned char n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned char)>(0x00415360)(this, n);
    }
    void Encode2(unsigned short n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned short)>(0x0042CA10)(this, n);
    }
    void Encode4(unsigned int n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned int)>(0x004153B0)(this, n);
    }
    void EncodeBuffer(unsigned char* p, unsigned int uSize) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned char*, unsigned int)>(0x00482200)(this, p, uSize);
    }
    void EncodeStr(char* s) {
        size_t uSize = strlen(s);
        this->Encode2(static_cast<unsigned short>(uSize));
        this->EncodeBuffer(reinterpret_cast<unsigned char*>(s), uSize);
    }
};
static_assert(sizeof(COutPacket) == 0x10);


class CClientSocket : public TSingleton<CClientSocket, 0x00C64064> {
public:
    struct CONNECTCONTEXT {
        ZList<ZInetAddr> lAddr;
        ZInetAddr* posList;
        int bLogin;
    };
    static_assert(sizeof(CONNECTCONTEXT) == 0x1C);

    MEMBER_AT(HWND, 0x4, m_hWnd)
    MEMBER_AT(ZSocketBase, 0x8, m_sock)
    MEMBER_AT(CONNECTCONTEXT, 0xC, m_ctxConnect)
    MEMBER_AT(ZList<ZRef<ZSocketBuffer>>, 0x50, m_lpSendBuff)
    MEMBER_AT(ZFatalSection, 0x7C, m_lockSend)
    MEMBER_AT(unsigned int, 0x84, m_uSeqSnd)

    __forceinline void SendPacket(COutPacket* oPacket) {
        ZSynchronizedHelper<ZFatalSection> sync(&this->m_lockSend());
        if (this->m_sock()._m_hSocket && this->m_sock()._m_hSocket != INVALID_SOCKET && !this->m_ctxConnect().lAddr._m_uCount) {
            // COutPacket::MakeBufferList(oPacket, &this->m_lpSendBuff, 95, &this->m_uSeqSnd, 1, this->m_uSeqSnd);
            reinterpret_cast<void (__thiscall*)(COutPacket*, ZList<ZRef<ZSocketBuffer>>*, unsigned short, unsigned int*, int, unsigned int)>(0x0068D100)(oPacket, &this->m_lpSendBuff(), 95, &this->m_uSeqSnd(), 1, this->m_uSeqSnd());
            // this->m_uSeqSnd = CIGCipher::innoHash((unsigned __int8 *)&this->m_uSeqSnd, 4, 0);
            this->m_uSeqSnd() = reinterpret_cast<unsigned int (__cdecl*)(unsigned char*, int, unsigned int*)>(0x00A1BF30)(reinterpret_cast<unsigned char*>(&this->m_uSeqSnd()), 4, nullptr);
            // CClientSocket::Flush(this);
            reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004AF6A0)(this);
        }
    }
};


struct ISMSG {
    unsigned int message;
    unsigned int wParam;
    int lParam;
};
static_assert(sizeof(ISMSG) == 0xC);

class CInputSystem : public TSingleton<CInputSystem, 0x00C68C20> {
};