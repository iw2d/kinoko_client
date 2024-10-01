#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#pragma warning(disable : 4996)


static volatile bool g_bGameDataLoaded = false;
static HANDLE g_hDataThread;
static DWORD g_dwDataThreadID;
static IWzResManPtr g_pDataThreadResMan;

typedef HRESULT (__stdcall* IWzResMan__raw_GetObject_t)(IWzResMan*, wchar_t*, tagVARIANT, tagVARIANT, tagVARIANT*);
static IWzResMan__raw_GetObject_t IWzResMan__raw_GetObject;

HRESULT __stdcall IWzResMan__raw_GetObject_hook(IWzResMan* pThis, wchar_t* sUOL, tagVARIANT vParam, tagVARIANT vAux, tagVARIANT* result) {
    if (GetCurrentThreadId() == g_dwDataThreadID) {
        return IWzResMan__raw_GetObject(g_pDataThreadResMan, sUOL, vParam, vAux, result);
    } else {
        return IWzResMan__raw_GetObject(pThis, sUOL, vParam, vAux, result);
    }
}

DWORD WINAPI DataThread(LPVOID lpParam) {
    DEBUG_MESSAGE("DataThread - CWvsApp::InitializeResMan");
    InitializeResMan(g_pDataThreadResMan);
    IWzResMan__raw_GetObject = reinterpret_cast<IWzResMan__raw_GetObject_t>(VMTHook(get_rm(), IWzResMan__raw_GetObject_hook, 7));
    DEBUG_MESSAGE("DataThread - CWvsApp::InitializeGameData");
    // CWvsApp::InitializeGameData(TSingleton<CWvsApp>::GetInstance());
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C8440)(CWvsApp::GetInstance());
    g_bGameDataLoaded = true;
    CloseHandle(g_hDataThread);
    DEBUG_MESSAGE("DataThread - Complete!");
    return 0;
}


typedef void (__thiscall* CWvsApp__ctor_t)(CWvsApp*, const char*);
static auto CWvsApp__ctor = reinterpret_cast<CWvsApp__ctor_t>(0x009CA8A0);

void __fastcall CWvsApp__ctor_hook(CWvsApp* pThis, void* _EDX, const char* sCmdLine) {
    DEBUG_MESSAGE("CWvsApp::CWvsApp");
    // TSingleton<CWvsApp>::TSingleton<CWvsApp>((TSingleton<CWvsApp>*) &this->m_hWnd);
    reinterpret_cast<void (__thiscall*)(void*)>(0x009C19C0)(&pThis->m_hWnd());
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
    reinterpret_cast<unsigned char* (__thiscall*)(ZArray<unsigned char>*, unsigned int, const ZAllocHelper*)>(0x004AE9B0)(&pThis->m_pBackupBuffer(), 0x1000, &alloc);
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
        reinterpret_cast<void (__cdecl*)()>(0x0045ECD0)();
    }

    // CWvsApp::SetClearStackLog(this, (bIs64 << 8) + (this->m_nOSVersion << 12));
    reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int)>(0x009C1960)(pThis, (bIs64 << 8) + (pThis->m_nOSVersion() << 12));
    pThis->m_nOSVersion() = ovi.dwMajorVersion;
    pThis->m_nOSMinorVersion() = ovi.dwMinorVersion;
    pThis->m_nOSBuildNumber() = ovi.dwBuildNumber;
    // ZXString<char>::operator=<char>(&this->m_sCSDVersion, ovi.szCSDVersion);
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*)>(0x0042C990)(&pThis->m_sCSDVersion(), ovi.szCSDVersion);
    pThis->m_b64BitInfo() = bIs64;
}


typedef void (__thiscall* CWvsApp__SetUp_t)(CWvsApp*);
static auto CWvsApp__SetUp = reinterpret_cast<CWvsApp__SetUp_t>(0x009CAFB0);

void __fastcall CWvsApp__SetUp_hook(CWvsApp* pThis, void* _EDX) {
    DEBUG_MESSAGE("CWvsApp::SetUp");
    // CWvsApp::InitializeAuth(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C3AD0)(pThis);
    srand(timeGetTime());
    // GetSEPrivilege();
    reinterpret_cast<void (__cdecl*)()>(0x0045E030)();

    DEBUG_MESSAGE("CWvsApp::SetUp - Initializing...");
    // TSingleton<CConfig>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C2420)();
    // CWvsApp::InitializePCOM(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C16C0)(pThis);
    // CWvsApp::CreateMainWindow(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C74A0)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Connecting to server...");
    // TSingleton<CClientSocket>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C23A0)();
    // CWvsApp::ConnectLogin(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C1B30)(pThis);
    // TSingleton<CFuncKeyMappedMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C2510)();
    // TSingleton<CQuickslotKeyMappedMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C27E0)();
    // TSingleton<CMacroSysMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C2590)();
    // TSingleton<CBattleRecordMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009C2220)();
    // CWvsApp::InitializeResMan(this);
    InitializeResMan(get_rm());

    DEBUG_MESSAGE("CWvsApp::SetUp - Graphic & Sound...");
    // CWvsApp::InitializeGr2D(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C7670)(pThis);
    // CWvsApp::InitializeInput(this);
    reinterpret_cast<void (__cdecl*)()>(0x009C7C30)();
    reinterpret_cast<void (__thiscall*)(CInputSystem*, HWND, void**)>(0x00571A60)(CInputSystem::GetInstance(), pThis->m_hWnd(), pThis->m_ahInput());

    ShowWindow(pThis->m_hWnd(), SW_SHOW);
    UpdateWindow(pThis->m_hWnd());
    SetForegroundWindow(pThis->m_hWnd());
    CHECK_HRESULT(get_gr()->raw_RenderFrame());

    // CWvsApp::InitializeSound(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009CA170)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Loading Data...");
    // CWvsApp::InitializeGameData(this); // ----------------------------------------------------- moved to data thread
    // TSingleton<CQuestMan>::CreateInstance()->LoadDemand();
    auto pQuestMan = reinterpret_cast<void* (__cdecl*)()>(0x009C21A0)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x006C3D60)(pQuestMan)) {
        ErrorMessage("Failed to load quest data.");
    }
    // CQuestMan::LoadPartyQuestInfo(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x006C5540)(pQuestMan);
    // CQuestMan::LoadExclusive(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x006B9670)(pQuestMan);

    DEBUG_MESSAGE("CwvsApp::SetUp - Complete!");
    // TSingleton<CMonsterBookMan>::CreateInstance()->LoadBook();
    auto pMonsterBookMan = reinterpret_cast<void* (__cdecl*)()>(0x009CA820)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x00664C10)(pMonsterBookMan)) {
        ErrorMessage("Failed to load monster book data.");
    }
    // CWvsApp::CreateWndManager(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009C2060)(pThis);
    // CConfig::ApplySysOpt(TSingleton<CConfig>::GetInstance(), nullptr, 0);
    reinterpret_cast<void (__thiscall*)(CConfig*, CONFIG_SYSOPT*, int)>(0x004B2300)(CConfig::GetInstance(), nullptr, 0);
    // TSingleton<CActionMan>::CreateInstance()->Init();
    auto pActionMan = reinterpret_cast<void* (__cdecl*)()>(0x009C22A0)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x0041BEB0)(pActionMan);
    // TSingleton<CAnimationDisplayer>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x009C2320)();
    // TSingleton<CMapleTVMan>::CreateInstance()->Init()
    auto pMapleTVMan = reinterpret_cast<void* (__cdecl*)()>(0x009C2680)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x0060FBC0)(pMapleTVMan);
    // TSingleton<CRadioManager>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x009C2770)();

    // (CLogo*) operator new(0x48); -> (CLogin*) operator new(0x2C8);
    void* pStage = ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x2C8);
    if (pStage) {
        // CLogo::CLogo(pStage); -> CLogin::Clogin(pStage);
        reinterpret_cast<void (__thiscall*)(void*)>(0x005DB440)(pStage);
    }
    // set_stage(pStage, nullptr);
    reinterpret_cast<void (__cdecl*)(void*, void*)>(0x00719C30)(pStage, nullptr);
    g_hDataThread = CreateThread(nullptr, 0, DataThread, nullptr, 0, &g_dwDataThreadID); // -------- create data thread
}


typedef void (__thiscall* CWvsApp__Run_t)(CWvsApp*, int*);
static auto CWvsApp__Run = reinterpret_cast<CWvsApp__Run_t>(0x009C5F00);

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
        void* pStage = *reinterpret_cast<void**>(0x00C6B638 + 4); // ZRef<CStage> g_pStage
        if (pStage) {
            // pStage->Update();
            void** vtable = *reinterpret_cast<void***>(pStage);
            reinterpret_cast<void (__thiscall*)(void*)>(vtable[0])(pStage);
        }
        // CWndMan::s_Update();
        reinterpret_cast<void (__cdecl*)()>(0x009B4B00)();
        pThis->m_tUpdateTime() += 30;
        if (tCurTime - pThis->m_tUpdateTime() > 0) {
            CHECK_HRESULT(get_gr()->raw_UpdateCurrentTime(pThis->m_tUpdateTime()));
        }
    }
    CHECK_HRESULT(get_gr()->raw_UpdateCurrentTime(tCurTime));
}

void __fastcall CWvsApp__Run_hook(CWvsApp* pThis, void* _EDX, int* pbTerminate) {
    HRESULT hr;
    tagMSG msg;
    ISMSG isMsg;
    memset(&msg, 0, sizeof(msg));
    memset(&isMsg, 0, sizeof(isMsg));
    if (CClientSocket::IsInstantiated()) {
        // CClientSocket::ManipulatePacket(TSingleton<CClientSocket>::GetInstance());
        reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004B0220)(CClientSocket::GetInstance());
    }
    do {
        DWORD dwRet = MsgWaitForMultipleObjects(3, pThis->m_ahInput(), 0, 0, 0xFF);
        if (dwRet <= 2) {
            // CInputSystem::UpdateDevice(TSingleton<CInputSystem>::GetInstance(), dwRet);
            reinterpret_cast<void (__thiscall*)(CInputSystem*, int)>(0x00571710)(CInputSystem::GetInstance(), dwRet);
            do {
                // if (!CInputSystem::GetISMessage(TSingleton<CInputSystem>::GetInstance(), &isMsg))
                if (!reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x005708F0)(CInputSystem::GetInstance(), &isMsg)) {
                    break;
                }
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            } while (!*pbTerminate);
        } else if (dwRet == 3) {
            do {
                if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
                // if (CWvsApp::ExtractComErrorCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x009C0860)(pThis, &hr)) {
                    _com_issue_error(hr);
                }
                // if (CWvsApp::ExtractZExceptionCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x009C0830)(pThis, &hr)) {
                    ZException exception(hr);
                    if (hr == 0x20000000) {
                        // CPatchException::CPatchException(&exception, this->m_nTargetVersion);
                        reinterpret_cast<void (__thiscall*)(void*, unsigned short)>(0x00520FA0)(&exception, pThis->m_nTargetVersion());
                    } else if (hr >= 0x21000000 && hr <= 0x21000006) {
                        // CDisconnectException::CDisconnectException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00429860)(&exception, hr);
                    } else if (hr >= 0x22000000 && hr <= 0x2200000E) {
                        // CTerminateException::CTerminateException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00401D50)(&exception, hr);
                    }
                    throw exception;
                }
            } while (!*pbTerminate && msg.message != WM_QUIT);
        } else {
            // if (CInputSystem::GenerateAutoKeyDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x0056F990)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            // if (CInputSystem::GenerateAutoBtnDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x0056FAC0)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            int tCurTime;
            CHECK_HRESULT(get_gr()->get_nextRenderTime(&tCurTime));
            CWvsApp__CallUpdate_hook(pThis, _EDX, tCurTime);
            // CWndMan::RedrawInvalidatedWindows();
            reinterpret_cast<void (__cdecl*)()>(0x009B2340)();
            CHECK_HRESULT(get_gr()->raw_RenderFrame());
            Sleep(1);
        }
    } while (!*pbTerminate && msg.message != WM_QUIT);
    if (msg.message == WM_QUIT) {
        PostQuitMessage(0);
    }
}


typedef void (__thiscall* CClientSocket__Connect_t)(CClientSocket*, CClientSocket::CONNECTCONTEXT*);
static auto CClientSocket__Connect = reinterpret_cast<CClientSocket__Connect_t>(0x004B0340);

void __fastcall CClientSocket__Connect_hook(CClientSocket* pThis, void* _EDX, CClientSocket::CONNECTCONTEXT* ctx) {
    DEBUG_MESSAGE("CClientSocket::Connect");
    // ZList<ZInetAddr>::RemoveAll(&this->m_ctxConnect.lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*)>(0x004ADF80)(&pThis->m_ctxConnect().lAddr);
    // ZList<ZInetAddr>::AddTail(&this->m_ctxConnect.lAddr, &ctx->lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*, ZList<ZInetAddr>*)>(0x004B0050)(&pThis->m_ctxConnect().lAddr, &ctx->lAddr);
    pThis->m_ctxConnect().posList = ctx->posList;
    pThis->m_ctxConnect().bLogin = ctx->bLogin;
    pThis->m_ctxConnect().posList = pThis->m_ctxConnect().lAddr._m_pHead;
    // ZList<ZInetAddr>::GetNext(&this->m_ctxConnect.posList);
    ZInetAddr* pAddr = reinterpret_cast<ZInetAddr* (__cdecl*)(ZInetAddr**)>(0x004ADF50)(&pThis->m_ctxConnect().posList);

    DEBUG_MESSAGE("CClientSocket::Connect (addr)");
    // CClientSocket::ClearSendReceiveCtx(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004AE1A0)(pThis);
    // ZSocketBase::CloseSocket(&this->m_sock);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*)>(0x004ACF30)(&pThis->m_sock());
    // ZSocketBase::Socket(&this->m_sock, 1, 2, 0);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*, int, int, int)>(0x004ACF50)(&pThis->m_sock(), 1, 2, 0);
    // CClientSocket::SetTimeout(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004ACBA0)(pThis);
    if (WSAAsyncSelect(pThis->m_sock()._m_hSocket, pThis->m_hWnd(), 0x401, 0x33) == -1 ||
            connect(pThis->m_sock()._m_hSocket, reinterpret_cast<const sockaddr*>(pAddr), 16) != -1 ||
            WSAGetLastError() != WSAEWOULDBLOCK) {
        // CClientSocket::OnConnect(this, 0);
        reinterpret_cast<void (__thiscall*)(CClientSocket*, int)>(0x004AEF10)(pThis, 0);
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

    MEMBER_AT(int, 0x1AC, m_bRequestSent)
    MEMBER_AT(ZArray<WORLDITEM>, 0x1CC, m_WorldItem)
    MEMBER_AT(ZArray<BALLOON>, 0x230, m_aBalloon)
};

typedef int (__thiscall* CLogin__SendCheckPasswordPacket_t)(CLogin*, char*, char*);
static auto CLogin__SendCheckPasswordPacket = reinterpret_cast<CLogin__SendCheckPasswordPacket_t>(0x005DB9D0);

int __fastcall CLogin__SendCheckPasswordPacket_hook(CLogin* pThis, void* _EDX, char* sID, char* sPasswd) {
    if (pThis->m_bRequestSent()) {
        return 0;
    }
    pThis->m_bRequestSent() = 1;
    // ZArray<CLogin::WORLDITEM>::RemoveAll(&this->m_WorldItem);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::WORLDITEM>*)>(0x005DA100)(&pThis->m_WorldItem());
    // ZArray<CLogin::BALLOON>::RemoveAll(&this->m_aBallon);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::BALLOON>*)>(0x005D68B0)(&pThis->m_aBalloon());

    CSystemInfo si;
    // CSystemInfo::Init(&si);
    reinterpret_cast<void (__thiscall*)(CSystemInfo*)>(0x00A1F1B0)(&si);

    COutPacket oPacket(1); // CP_CheckPassword
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


typedef void (__thiscall* CLogin__SendSelectCharPacket_t)(CLogin*);
static auto CLogin__SendSelectCharPacket = reinterpret_cast<CLogin__SendSelectCharPacket_t>(0x005DA2A0);

void __fastcall CLogin__SendSelectCharPacket_hook(CLogin* pThis, void* _EDX) {
    if (!g_bGameDataLoaded) {
        ZXString<char> sMsg;
        // ZXString<char>::ZXString<char>(&sMsg, message, -1);
        reinterpret_cast<void (__thiscall*)(ZXString<char>*, const char*, int)>(0x0042D230)(&sMsg, "Not all data has been loaded.", -1);
        // CUtilDlg::Notice(sMsg, nullptr, nullptr, true, false);
        reinterpret_cast<void (__cdecl*)(ZXString<char>, const wchar_t*, void*, int, int)>(0x00977220)(sMsg, nullptr, nullptr, true, false);
        return;
    }
    CLogin__SendSelectCharPacket(pThis);
}


void AttachClientBypass() {
    ATTACH_HOOK(CWvsApp__ctor, CWvsApp__ctor_hook);
    ATTACH_HOOK(CWvsApp__SetUp, CWvsApp__SetUp_hook);
    ATTACH_HOOK(CWvsApp__Run, CWvsApp__Run_hook);
    ATTACH_HOOK(CClientSocket__Connect, CClientSocket__Connect_hook);
    ATTACH_HOOK(CLogin__SendCheckPasswordPacket, CLogin__SendCheckPasswordPacket_hook);
    ATTACH_HOOK(CLogin__SendSelectCharPacket, CLogin__SendSelectCharPacket_hook);

    // CWvsContext::OnEnterField
    PatchJmp(0x009DBEEE, 0x009DC278);

    PatchRetZero(0x004AB900); // DR_check
    PatchRetZero(0x0045EBD0); // Hidedll
    PatchRetZero(0x009BF6C0); // SendHSLog
    PatchRetZero(0x009BF370); // CeTracer::Run
    PatchRetZero(0x009BF390); // ShowStartUpWndModal
    PatchRetZero(0x00429000); // ShowAdBalloon

    // CWzFileSystem::OpenArchive - disable memory mapping
    // LoadLibraryA("NAMESPACE.DLL");
    // Patch1(0x5081E59D + 1, 0x2);
    // Patch1(0x5081EFA3 + 1, 0x2);

    // CAESCipher::s_BasicKey - WZ encryption key
    // LoadLibraryA("PCOM.DLL");
    // PatchStr(0x50C2E000, "\00\00\00\00");
}