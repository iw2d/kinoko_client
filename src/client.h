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


void InitializeResMan(const IWzResManPtr& rm); // resman.cpp


static inline IWzGr2DPtr& get_gr() {
    return *reinterpret_cast<IWzGr2DPtr*>(0x00E1FB18);
}

static inline IWzResManPtr& get_rm() {
    return *reinterpret_cast<IWzResManPtr*>(0x00E1FB1C);
}

static inline IWzNameSpacePtr& get_root() {
    return *reinterpret_cast<IWzNameSpacePtr*>(0x00E1FB24);
}

static inline IWzNameSpacePtr& get_sub(int nIdx) {
    return *reinterpret_cast<IWzNameSpacePtr*>(0x00E1FB28 + nIdx * 4);
}

static FARPROC* _g_apfnPCOMAPIs = reinterpret_cast<FARPROC*>(0x00E1D3E4);

template<typename T>
static void __cdecl PcCreateObject(const wchar_t* sUOL, T* pObj, IUnknown* pUnkOuter) {
    *pObj = nullptr;
    if (!_g_apfnPCOMAPIs[0]) {
        throw _com_error(CO_E_NOTINITIALIZED);
    }
    CHECK_HRESULT(reinterpret_cast<HRESULT (__cdecl*)(const wchar_t*, const GUID*, T*, void*)>(_g_apfnPCOMAPIs[0])(sUOL, &__uuidof(T), pObj, pUnkOuter));
}

static void __cdecl PcSetRootNameSpace(IUnknown* pNameSpace) {
    CHECK_HRESULT(reinterpret_cast<HRESULT (__cdecl*)(IUnknown**, int)>(_g_apfnPCOMAPIs[4])(&pNameSpace, 1));
}


class CWvsApp : public TSingleton<CWvsApp, 0x00E125EC> {
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

    static void Dir_BackSlashToSlash(char* sDir) {
        size_t uLen = strlen(sDir);
        for (size_t i = 0; i < uLen; ++i) {
            if (sDir[i] == '\\') {
                sDir[i] = '/';
            }
        }
    }
    static void Dir_upDir(char* sDir) {
        size_t uLen = strlen(sDir);
        if (uLen > 0 && sDir[uLen - 1] == '/') {
            sDir[uLen - 1] = 0;
        }
        for (size_t i = strlen(sDir) - 1; i > 0; --i) {
            if (sDir[i] == '/') {
                sDir[i] = 0;
                return;
            }
        }
    }
    static void Dir_SlashToBackSlash(char* sDir) {
        size_t uLen = strlen(sDir);
        for (size_t i = 0; i < uLen; ++i) {
            if (sDir[i] == '/') {
                sDir[i] = '\\';
            }
        }
    }
};


class CWvsContext : public TSingleton<CWvsContext, 0x00E12368> {
public:
    MEMBER_AT(int, 0x4768, m_nScreenWidth)
    MEMBER_AT(int, 0x476C, m_nScreenHeight)
    MEMBER_AT(int, 0x4770, m_nAdjustCenterY)
    MEMBER_AT(bool, 0x4774, m_bIsLargeScreen)
};


class CFuncKeyMappedMan : public TSingleton<CFuncKeyMappedMan, 0x00E191CC> {
};


class CWndMan : public TSingleton<CWndMan, 0x00E16DDC> {
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

class CConfig : public TSingleton<CConfig, 0x00E16AEC> {
public:
    MEMBER_AT(CONFIG_SYSOPT, 0x74, m_sysOpt)
};


class COutPacket {
public:
    int m_bLoopback;
    ZArray<unsigned char> m_aSendBuff;
    unsigned int m_uOffset;
    int m_bIsEncryptedByShanda;

    inline COutPacket(int nType) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, int)>(0x006DED70)(this, nType);
    }
    inline void Encode1(unsigned char n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned char)>(0x004163C0)(this, n);
    }
    inline void Encode2(unsigned short n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned short)>(0x0042DE70)(this, n);
    }
    inline void Encode4(unsigned int n) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned int)>(0x00416410)(this, n);
    }
    inline void EncodeBuffer(unsigned char* p, unsigned int uSize) {
        reinterpret_cast<void (__thiscall*)(COutPacket*, unsigned char*, unsigned int)>(0x00496670)(this, p, uSize);
    }
    inline void EncodeStr(char* s) {
        size_t uSize = strlen(s);
        this->Encode2(static_cast<unsigned short>(uSize));
        this->EncodeBuffer(reinterpret_cast<unsigned char*>(s), uSize);
    }
};
static_assert(sizeof(COutPacket) == 0x10);


class CClientSocket : public TSingleton<CClientSocket, 0x00E12364> {
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
    MEMBER_AT(ZList<ZRef<ZSocketBuffer>>, 0x54, m_lpSendBuff)
    MEMBER_AT(ZFatalSection, 0x80, m_lockSend)
    MEMBER_AT(unsigned int, 0x88, m_uSeqSnd)

    __forceinline void SendPacket(COutPacket* oPacket) {
        ZSynchronizedHelper<ZFatalSection> sync(&this->m_lockSend());
        if (this->m_sock()._m_hSocket && this->m_sock()._m_hSocket != INVALID_SOCKET && !this->m_ctxConnect().lAddr._m_uCount) {
            // COutPacket::MakeBufferList(oPacket, &this->m_lpSendBuff, 100, &this->m_uSeqSnd, 1, this->m_uSeqSnd);
            reinterpret_cast<void (__thiscall*)(COutPacket*, ZList<ZRef<ZSocketBuffer>>*, unsigned short, unsigned int*, int, unsigned int)>(0x006DEDE0)(oPacket, &this->m_lpSendBuff(), 100, &this->m_uSeqSnd(), 1, this->m_uSeqSnd());
            // this->m_uSeqSnd = CIGCipher::innoHash((unsigned __int8 *)&this->m_uSeqSnd, 4, 0);
            this->m_uSeqSnd() = reinterpret_cast<unsigned int (__cdecl*)(unsigned char*, int, unsigned int*)>(0x00B3E8E0)(reinterpret_cast<unsigned char*>(&this->m_uSeqSnd()), 4, nullptr);
            // CClientSocket::Flush(this);
            reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004C4980)(this);
        }
    }
};


struct ISMSG {
    unsigned int message;
    unsigned int wParam;
    int lParam;
};
static_assert(sizeof(ISMSG) == 0xC);

class CInputSystem : public TSingleton<CInputSystem, 0x00E16F80> {
public:
    inline int IsKeyPressed(unsigned int nVK) {
        return reinterpret_cast<int (__thiscall*)(CInputSystem*, unsigned int)>(0x005A4820)(this, nVK);
    }
};


class CUserLocal : public TSingleton<CUserLocal, 0x00E16A2C> {
public:
    MEMBER_AT(IWzVector2DPtr, 0x1C78, m_pvc)
    MEMBER_AT(unsigned int, 0x1C7C, m_dwCharacterId)
    MEMBER_AT(ZXString<char>, 0x1C80, m_sCharacterName)
    MEMBER_AT(int, 0x52F8, m_bJumpKeyUp)

    inline int GetJobCode() {
        return reinterpret_cast<int (__thiscall*)(CUserLocal*)>(0x009FF5B0)(this);
    }
    inline int DoActiveSkill(int nSkillID, unsigned int nScanCode, int* pnConsumeCheck) {
        return reinterpret_cast<int (__thiscall*)(CUserLocal*, int, unsigned int, int*)>(0x00A42E20)(this, nSkillID, nScanCode, pnConsumeCheck);
    }
};


class CItemInfo : public TSingleton<CItemInfo, 0x00E12114> {
public:
    inline IWzPropertyPtr* GetItemInfo(IWzPropertyPtr* result, int nItemID) {
        return reinterpret_cast<IWzPropertyPtr* (__thiscall*)(CItemInfo*, IWzPropertyPtr*, int)>(0x005E2CC0)(this, result, nItemID);
    }
};