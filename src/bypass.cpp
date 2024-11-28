#include "pch.h"
#include "client.h"
#include "hook.h"
#include "config.h"
#include "debug.h"

#pragma warning(disable : 4996)


typedef void (__thiscall* CWvsApp__ctor_t)(CWvsApp*, const char*);
static auto CWvsApp__ctor = reinterpret_cast<CWvsApp__ctor_t>(0x00ADC4F0);

void __fastcall CWvsApp__ctor_hook(CWvsApp* pThis, void* _EDX, const char* sCmdLine) {
    DEBUG_MESSAGE("CWvsApp::CWvsApp");
    CWvsApp::SetInstance(pThis);
    pThis->m_hWnd() = nullptr;
    pThis->m_bPCOMInitialized() = 0;
    pThis->m_hHook() = nullptr;
    pThis->m_nOSVersion() = 0;
    pThis->m_nOSMinorVersion() = 0;
    pThis->m_nOSBuildNumber() = 0;
    // ZXString<char>::ZXString<char>(&this->m_sCSDVersion);
    pThis->m_sCSDVersion()._m_pStr = nullptr;
    pThis->m_b64BitInfo() = 0;
    pThis->m_tUpdateTime() = 0;
    pThis->m_bFirstUpdate() = 1;
    // ZXString<char>::ZXString<char>(&this->m_sCmdLine);
    pThis->m_sCmdLine()._m_pStr = nullptr;
    pThis->m_nGameStartMode() = 0;
    pThis->m_bAutoConnect() = 1;
    pThis->m_bShowAdBalloon() = 0;
    pThis->m_bExitByTitleEscape() = 0;
    pThis->m_hrZExceptionCode() = 0;
    pThis->m_hrComErrorCode() = 0;
    pThis->m_tNextSecurityCheck() = 0;
    pThis->m_bEnabledDX9() = true;
    // ZArray<unsigned char>::ZArray<unsigned char>(&this->m_pBackupBuffer);
    pThis->m_pBackupBuffer().a = nullptr;
    pThis->m_dwBackupBufferSize() = 0;
    pThis->m_dwClearStackLog() = 0;
    pThis->m_bWindowActive() = 1;

    ZAllocHelper alloc(0);
    // ZArray<unsigned char>::Alloc(&this->m_pBackupBuffer, 0x1000u, alloc);
    reinterpret_cast<unsigned char* (__thiscall*)(ZArray<unsigned char>*, unsigned int, const ZAllocHelper*)>(0x004C34F0)(&pThis->m_pBackupBuffer(), 0x1000, &alloc);
    pThis->m_nGameStartMode() = 2;
    pThis->m_dwMainThreadId() = GetCurrentThreadId();

    OSVERSIONINFO ovi;
    ovi.dwOSVersionInfoSize = 148;
    GetVersionExA(&ovi);
    pThis->m_bWin9x() = ovi.dwPlatformId == 1;
    if (ovi.dwMajorVersion >= 6 && !pThis->m_nGameStartMode()) {
        pThis->m_nGameStartMode() = 2;
    }

    typedef BOOL (WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    auto fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandleA("KERNEL32"), "IsWow64Process"));
    BOOL bIs64 = 0;
    if (fnIsWow64Process){
        fnIsWow64Process(GetCurrentProcess(), &bIs64);
    }
    if (ovi.dwMajorVersion >= 6 && !bIs64) {
        // ResetLSP();
        reinterpret_cast<void (__cdecl*)()>(0x0046E900)();
    }

    // CWvsApp::SetClearStackLog(this, (bIs64 << 8) + (this->m_nOSVersion << 12));
    unsigned int dwLog = (bIs64 << 8) + (pThis->m_nOSVersion() << 12);
    if ((dwLog & pThis->m_dwClearStackLog()) == 0) {
        pThis->m_dwClearStackLog() += dwLog;
    }

    pThis->m_nOSVersion() = ovi.dwMajorVersion;
    pThis->m_nOSMinorVersion() = ovi.dwMinorVersion;
    pThis->m_nOSBuildNumber() = ovi.dwBuildNumber;
    // ZXString<char>::operator=<char>(&this->m_sCSDVersion, ovi.szCSDVersion);
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*)>(0x0042DDF0)(&pThis->m_sCSDVersion(), ovi.szCSDVersion);
    pThis->m_b64BitInfo() = bIs64;
}


typedef void (__thiscall* CWvsApp__SetUp_t)(CWvsApp*);
static auto CWvsApp__SetUp = reinterpret_cast<CWvsApp__SetUp_t>(0x00ADC9B0);

void __fastcall CWvsApp__SetUp_hook(CWvsApp* pThis, void* _EDX) {
    DEBUG_MESSAGE("CWvsApp::SetUp");
    // CWvsApp::InitializeAuth(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00AD4FE0)(pThis);
    srand(timeGetTime());
    // GetSEPrivilege();
    reinterpret_cast<void (__cdecl*)()>(0x0046DC60)();

    DEBUG_MESSAGE("CWvsApp::SetUp - Initializing...");
    // TSingleton<CConfig>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x00AD3850)();
    // CWvsApp::InitializePCOM(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00AD3030)(pThis);
    // CWvsApp::CreateMainWindow(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00ADAEF0)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Connecting to server...");
    // TSingleton<CClientSocket>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x00AD37D0)();
    // CWvsApp::ConnectLogin(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00AD32B0)(pThis);
    // TSingleton<CFuncKeyMappedMan>::CreateInstance();
    CFuncKeyMappedMan* pFuncKeyMappedMan = reinterpret_cast<CFuncKeyMappedMan*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x3CC));
    if (pFuncKeyMappedMan) {
        // CFuncKeyMappedMan::CFuncKeyMappedMan();
        CFuncKeyMappedMan::SetInstance(pFuncKeyMappedMan);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x0) = reinterpret_cast<void*>(0x00CCBA28); // CFuncKeyMappedMan::`vftable'
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x004), reinterpret_cast<void*>(0x00E039B0), 0x1BD); // s_aDefaultFKM
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x1C1), reinterpret_cast<void*>(0x00E039B0), 0x1BD); // s_aDefaultFKM
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x380), reinterpret_cast<void*>(0x00E03B70), 0x20); // s_aDefaultQKM
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x3A0), reinterpret_cast<void*>(0x00E03B70), 0x20); // s_aDefaultQKM
        *reinterpret_cast<int**>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x3C0) = 0; // nPetConsumeItemID
        *reinterpret_cast<int**>(reinterpret_cast<uintptr_t>(pFuncKeyMappedMan) + 0x3C4) = 0; // nPetConsumeMPItemID
    }

    // TSingleton<CQuickslotKeyMappedMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x00AD3C10)();
    // TSingleton<CMacroSysMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x00AD39C0)();
    // TSingleton<CBattleRecordMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x00AD35D0)();
    // CWvsApp::InitializeResMan(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00ADB0D0)(pThis); // InitializeResMan(get_rm());

    DEBUG_MESSAGE("CWvsApp::SetUp - Graphic & Sound...");
    // CWvsApp::InitializeGr2D(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00AD9530)(pThis);
    // CWvsApp::InitializeInput(this);
    reinterpret_cast<void (__cdecl*)()>(0x00AD9B50)(); // TSingleton<CInputSystem>::CreateInstance();
    reinterpret_cast<void (__thiscall*)(CInputSystem*, HWND, void**)>(0x005A6AE0)(CInputSystem::GetInstance(), pThis->m_hWnd(), pThis->m_ahInput()); // CInputSystem::Init(Instance, MainWindow, this->m_ahInput);

    ShowWindow(pThis->m_hWnd(), SW_SHOW);
    UpdateWindow(pThis->m_hWnd());
    SetForegroundWindow(pThis->m_hWnd());
    CHECK_HRESULT(get_gr()->raw_RenderFrame());

    // CWvsApp::InitializeSound(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00ADBC40)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Loading Data...");
    // CWvsApp::InitializeGameData(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00ADA430)(pThis);

    // TSingleton<CQuestMan>::CreateInstance()->LoadDemand();
    auto pQuestMan = reinterpret_cast<void* (__cdecl*)()>(0x00AD3550)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x0071ACE0)(pQuestMan)) {
        ErrorMessage("Failed to load quest data.");
    }
    // CQuestMan::LoadPartyQuestInfo(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x0071C380)(pQuestMan);
    // CQuestMan::LoadExclusive(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x0070F090)(pQuestMan);

    DEBUG_MESSAGE("CwvsApp::SetUp - Complete!");
    // TSingleton<CMonsterBookMan>::CreateInstance()->LoadBook();
    auto pMonsterBookMan = reinterpret_cast<void* (__cdecl*)()>(0x00ADC470)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x006B2880)(pMonsterBookMan)) {
        ErrorMessage("Failed to load monster book data.");
    }
    // CWvsApp::CreateWndManager(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x00AD3480)(pThis);
    // CConfig::ApplySysOpt(TSingleton<CConfig>::GetInstance(), nullptr, 0);
    reinterpret_cast<void (__thiscall*)(CConfig*, CONFIG_SYSOPT*, int)>(0x004C72F0)(CConfig::GetInstance(), nullptr, 0);
    // TSingleton<CActionMan>::CreateInstance()->Init();
    auto pActionMan = reinterpret_cast<void* (__cdecl*)()>(0x00AD36D0)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x0041CE20)(pActionMan);
    // TSingleton<CAnimationDisplayer>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x00AD3750)();
    // TSingleton<CMapleTVMan>::CreateInstance()->Init()
    auto pMapleTVMan = reinterpret_cast<void* (__cdecl*)()>(0x00AD3AB0)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x00654D60)(pMapleTVMan);
    // TSingleton<CRadioManager>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x00AD3BA0)();
    // TSingleton<CCurseProcess>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x00AD3C80)();

    // (CLogo*) operator new(0x48); -> (CLogin*) operator new(0x2F0);
    void* pStage = ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x2F0);
    if (pStage) {
        // CLogo::CLogo(pStage); -> CLogin::Clogin(pStage);
        reinterpret_cast<void (__thiscall*)(void*)>(0x0061BC20)(pStage);
    }
    // set_stage(pStage, nullptr);
    reinterpret_cast<void (__cdecl*)(void*, void*)>(0x00782B50)(pStage, nullptr);
}


typedef void (__thiscall* CWvsApp__Run_t)(CWvsApp*, int*);
static auto CWvsApp__Run = reinterpret_cast<CWvsApp__Run_t>(0x00AD73D0);

void __fastcall CWvsApp__CallUpdate_hook(CWvsApp* pThis, void* _EDX, int tCurTime) {
    if (pThis->m_bFirstUpdate()) {
        pThis->m_tUpdateTime() = tCurTime;
        pThis->m_tLastServerIPCheck() = tCurTime;
        pThis->m_tLastServerIPCheck2() = tCurTime;
        pThis->m_tLastGGHookingAPICheck() = tCurTime;
        pThis->m_tLastSecurityCheck() = tCurTime;
        pThis->m_bFirstUpdate() = 0;
    }
    while (tCurTime - pThis->m_tUpdateTime() > 0) {
        // pStage = get_stage()->p;
        void* pStage = *reinterpret_cast<void**>(0x00E19E10 + 4); // ZRef<CStage> g_pStage
        if (pStage) {
            // pStage->Update();
            void** vtable = *reinterpret_cast<void***>(pStage);
            reinterpret_cast<void (__thiscall*)(void*)>(vtable[0])(pStage);
        }
        // CWndMan::s_Update();
        reinterpret_cast<void (__cdecl*)()>(0x00ABE8F0)();
        pThis->m_tUpdateTime() += 30;
        if (tCurTime - pThis->m_tUpdateTime() > 0) {
            CHECK_HRESULT(get_gr()->raw_UpdateCurrentTime(pThis->m_tUpdateTime()));
        }
    }
    CHECK_HRESULT(get_gr()->raw_UpdateCurrentTime(tCurTime));
}

void __fastcall CWvsApp__ISMsgProc_hook(CWvsApp* pThis, unsigned int message, unsigned int wParam, int lParam) {
    if (CWndMan::IsInstantiated()) {
        if (message >= 0x200) {
            // CWndMan::ProcessMouse
            reinterpret_cast<void (__thiscall*)(CWndMan*, unsigned int, unsigned int, int)>(0x00ABDBD0)(CWndMan::GetInstance(), message, wParam, lParam);
        } else if (message == 0x100) {
            // CWndMan::ProcessKey
            reinterpret_cast<void (__thiscall*)(CWndMan*, unsigned int, unsigned int, int)>(0x00ABE1E0)(CWndMan::GetInstance(), message, wParam, lParam);
        }
    }
}

void __fastcall CWvsApp__Run_hook(CWvsApp* pThis, void* _EDX, int* pbTerminate) {
    HRESULT hr;
    tagMSG msg;
    ISMSG isMsg;
    memset(&msg, 0, sizeof(msg));
    memset(&isMsg, 0, sizeof(isMsg));
    if (CClientSocket::IsInstantiated()) {
        // CClientSocket::ManipulatePacket(TSingleton<CClientSocket>::GetInstance());
        reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004C51F0)(CClientSocket::GetInstance());
    }
    do {
        DWORD dwRet = MsgWaitForMultipleObjects(3, pThis->m_ahInput(), 0, 0, 0xFF);
        if (dwRet <= 2) {
            // CInputSystem::UpdateDevice(TSingleton<CInputSystem>::GetInstance(), dwRet);
            reinterpret_cast<void (__thiscall*)(CInputSystem*, int)>(0x005A6790)(CInputSystem::GetInstance(), dwRet);
            do {
                // if (!CInputSystem::GetISMessage(TSingleton<CInputSystem>::GetInstance(), &isMsg))
                if (!reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x005A5A60)(CInputSystem::GetInstance(), &isMsg)) {
                    break;
                }
                CWvsApp__ISMsgProc_hook(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            } while (!*pbTerminate);
        } else if (dwRet == 3) {
            do {
                if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
                // if (CWvsApp::ExtractComErrorCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x00AD2F10)(pThis, &hr)) {
                    _com_issue_error(hr);
                }
                // if (CWvsApp::ExtractZExceptionCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x00AD2EE0)(pThis, &hr)) {
                    ZException exception(hr);
                    if (hr == 0x20000000) {
                        // CPatchException::CPatchException(&exception, this->m_nTargetVersion);
                        reinterpret_cast<void (__thiscall*)(void*, unsigned short)>(0x00541440)(&exception, pThis->m_nTargetVersion());
                    } else if (hr >= 0x21000000 && hr <= 0x21000006) {
                        // CDisconnectException::CDisconnectException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x0042AE70)(&exception, hr);
                    } else if (hr >= 0x22000000 && hr <= 0x2200000E) {
                        // CTerminateException::CTerminateException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00401CE0)(&exception, hr);
                    }
                    throw exception;
                }
            } while (!*pbTerminate && msg.message != WM_QUIT);
        } else {
            // if (CInputSystem::GenerateAutoKeyDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x005A4A10)(CInputSystem::GetInstance(), &isMsg)) {
                CWvsApp__ISMsgProc_hook(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            // if (CInputSystem::GenerateAutoBtnDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x005A4B40)(CInputSystem::GetInstance(), &isMsg)) {
                CWvsApp__ISMsgProc_hook(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            int tCurTime;
            CHECK_HRESULT(get_gr()->get_nextRenderTime(&tCurTime));
            CWvsApp__CallUpdate_hook(pThis, _EDX, tCurTime);
            // CWndMan::RedrawInvalidatedWindows();
            reinterpret_cast<void (__cdecl*)()>(0x00ABC130)();
            CHECK_HRESULT(get_gr()->raw_RenderFrame());
            Sleep(1);
        }
    } while (!*pbTerminate && msg.message != WM_QUIT);
    if (msg.message == WM_QUIT) {
        PostQuitMessage(0);
    }
}


typedef void (__thiscall* CClientSocket__Connect_t)(CClientSocket*, CClientSocket::CONNECTCONTEXT*);
static auto CClientSocket__Connect = reinterpret_cast<CClientSocket__Connect_t>(0x004C5140);

void __fastcall CClientSocket__Connect_hook(CClientSocket* pThis, void* _EDX, CClientSocket::CONNECTCONTEXT* ctx) {
    DEBUG_MESSAGE("CClientSocket::Connect");
    // ZList<ZInetAddr>::RemoveAll(&this->m_ctxConnect.lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*)>(0x004C30E0)(&pThis->m_ctxConnect().lAddr);
    // ZList<ZInetAddr>::AddTail(&this->m_ctxConnect.lAddr, &ctx->lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*, ZList<ZInetAddr>*)>(0x004C4B20)(&pThis->m_ctxConnect().lAddr, &ctx->lAddr);
    pThis->m_ctxConnect().posList = ctx->posList;
    pThis->m_ctxConnect().bLogin = ctx->bLogin;
    pThis->m_ctxConnect().posList = pThis->m_ctxConnect().lAddr._m_pHead;
    // ZList<ZInetAddr>::GetNext(&this->m_ctxConnect.posList);
    ZInetAddr* pAddr = reinterpret_cast<ZInetAddr* (__cdecl*)(ZInetAddr**)>(0x004C30B0)(&pThis->m_ctxConnect().posList);

    DEBUG_MESSAGE("CClientSocket::Connect (addr)");
    // CClientSocket::ClearSendReceiveCtx(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004C3300)(pThis);
    // ZSocketBase::CloseSocket(&this->m_sock);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*)>(0x004C1F60)(&pThis->m_sock());
    // ZSocketBase::Socket(&this->m_sock, 1, 2, 0);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*, int, int, int)>(0x004C1F80)(&pThis->m_sock(), 1, 2, 0);
    // CClientSocket::SetTimeout(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004C1BE0)(pThis);
    if (WSAAsyncSelect(pThis->m_sock()._m_hSocket, pThis->m_hWnd(), 0x401, 0x33) == -1 ||
            connect(pThis->m_sock()._m_hSocket, reinterpret_cast<const sockaddr*>(pAddr), 16) != -1 ||
            WSAGetLastError() != WSAEWOULDBLOCK) {
        // CClientSocket::OnConnect(this, 0);
        DEBUG_MESSAGE("CClientSocket::OnConnect %d");
        reinterpret_cast<void (__thiscall*)(CClientSocket*, int)>(0x004C4270)(pThis, 0);
    }
}


class CSystemInfo {
public:
    unsigned char SupportId[16];
    unsigned char MachineId[16];

    virtual ~CSystemInfo() = default;
};
static_assert(sizeof(CSystemInfo) == 0x24);

class CLogin {
public:
    struct WORLDITEM;
    struct BALLOON;

    MEMBER_AT(int, 0x1C8, m_bRequestSent)
    MEMBER_AT(ZArray<WORLDITEM>, 0x1E8, m_WorldItem)
    MEMBER_AT(ZArray<BALLOON>, 0x24C, m_aBalloon)
};

typedef int (__thiscall* CLogin__SendCheckPasswordPacket_t)(CLogin*, char*, char*);
static auto CLogin__SendCheckPasswordPacket = reinterpret_cast<CLogin__SendCheckPasswordPacket_t>(0x0061C1E0);

int __fastcall CLogin__SendCheckPasswordPacket_hook(CLogin* pThis, void* _EDX, char* sID, char* sPasswd) {
    DEBUG_MESSAGE("CLogin::SendCheckPasswordPacket");
    if (pThis->m_bRequestSent()) {
        return 0;
    }
    pThis->m_bRequestSent() = 1;
    // ZArray<CLogin::WORLDITEM>::RemoveAll(&this->m_WorldItem);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::WORLDITEM>*)>(0x0061A8B0)(&pThis->m_WorldItem());
    // ZArray<CLogin::BALLOON>::RemoveAll(&this->m_aBallon);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::BALLOON>*)>(0x00617840)(&pThis->m_aBalloon());

    CSystemInfo si;
    // CSystemInfo::Init(&si);
    reinterpret_cast<void (__thiscall*)(CSystemInfo*)>(0x00B41B80)(&si);

    COutPacket oPacket(2); // CP_CheckPassword
    oPacket.EncodeStr(sID);
    oPacket.EncodeStr(sPasswd);
    oPacket.EncodeBuffer(si.MachineId, 16);
    oPacket.Encode4(0); // CSystemInfo::GetGameRoomClient(&v15)
    oPacket.Encode1(CWvsApp::GetInstance()->m_nGameStartMode());
    oPacket.Encode1(0);
    oPacket.Encode1(0);
    oPacket.Encode4(0); // CConfig::GetPartnerCode(TSingleton<CConfig>::ms_pInstance._m_pStr)
    CClientSocket::GetInstance()->SendPacket(&oPacket);
    return 0;
}


void AttachClientBypass() {
    ATTACH_HOOK(CWvsApp__ctor, CWvsApp__ctor_hook);
    ATTACH_HOOK(CWvsApp__SetUp, CWvsApp__SetUp_hook);
    ATTACH_HOOK(CWvsApp__Run, CWvsApp__Run_hook);
    ATTACH_HOOK(CClientSocket__Connect, CClientSocket__Connect_hook);
    ATTACH_HOOK(CLogin__SendCheckPasswordPacket, CLogin__SendCheckPasswordPacket_hook);

    PatchJmp(0x00AFAF0C, 0x00AFB018); // CWvsContext::OnEnterField

    PatchRetZero(0x004C0940); // DR_check
    PatchRetZero(0x0046E800); // Hidedll
    PatchRetZero(0x00AD1D90); // SendHSLog
    PatchRetZero(0x00AD1A40); // ShowStartUpWndModal
    PatchRetZero(0x0042A510); // ShowAdBalloon
}