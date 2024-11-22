#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#pragma warning(disable : 4996)


typedef void (__thiscall* CWvsApp__ctor_t)(CWvsApp*, const char*);
static auto CWvsApp__ctor = reinterpret_cast<CWvsApp__ctor_t>(0x009F4E50);

void __fastcall CWvsApp__ctor_hook(CWvsApp* pThis, void* _EDX, const char* sCmdLine) {
    DEBUG_MESSAGE("CWvsApp::CWvsApp");
    // TSingleton<CWvsApp>::TSingleton<CWvsApp>((TSingleton<CWvsApp>*) &this->m_hWnd);
    reinterpret_cast<void (__thiscall*)(void*)>(0x009EAC40)(&pThis->m_hWnd());
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
    reinterpret_cast<unsigned char* (__thiscall*)(ZArray<unsigned char>*, unsigned int, const ZAllocHelper*)>(0x004AF8A0)(&pThis->m_pBackupBuffer(), 0x1000, &alloc);
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
        reinterpret_cast<void (__cdecl*)()>(0x0045F420)();
    }

    // CWvsApp::SetClearStackLog(this, (bIs64 << 8) + (this->m_nOSVersion << 12));
    reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int)>(0x009EABE0)(pThis, (bIs64 << 8) + (pThis->m_nOSVersion() << 12));
    pThis->m_nOSVersion() = ovi.dwMajorVersion;
    pThis->m_nOSMinorVersion() = ovi.dwMinorVersion;
    pThis->m_nOSBuildNumber() = ovi.dwBuildNumber;
    // ZXString<char>::operator=<char>(&this->m_sCSDVersion, ovi.szCSDVersion);
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*)>(0x0042C9F0)(&pThis->m_sCSDVersion(), ovi.szCSDVersion);
    pThis->m_b64BitInfo() = bIs64;
}


typedef void (__thiscall* CWvsApp__SetUp_t)(CWvsApp*);
static auto CWvsApp__SetUp = reinterpret_cast<CWvsApp__SetUp_t>(0x009F38C0);

void __fastcall CWvsApp__SetUp_hook(CWvsApp* pThis, void* _EDX) {
    DEBUG_MESSAGE("CWvsApp::SetUp");
    // CWvsApp::InitializeAuth(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009ECE30)(pThis);
    srand(timeGetTime());
    // GetSEPrivilege();
    reinterpret_cast<void (__cdecl*)()>(0x0045E780)();

    DEBUG_MESSAGE("CWvsApp::SetUp - Initializing...");
    // TSingleton<CConfig>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EB6A0)();
    // CWvsApp::InitializePCOM(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009EA940)(pThis);
    // CWvsApp::CreateMainWindow(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009F23F0)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Connecting to server...");
    // TSingleton<CClientSocket>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EB620)();
    // CWvsApp::ConnectLogin(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009EADB0)(pThis);
    // TSingleton<CFuncKeyMappedMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EB790)();
    // TSingleton<CQuickslotKeyMappedMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EBA60)();
    // TSingleton<CMacroSysMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EB810)();
    // TSingleton<CBattleRecordMan>::CreateInstance();
    reinterpret_cast<void (__cdecl*)()>(0x009EB4A0)();
    // CWvsApp::InitializeResMan(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009F2730)(pThis); // InitializeResMan(get_rm());

    DEBUG_MESSAGE("CWvsApp::SetUp - Graphic & Sound...");
    // CWvsApp::InitializeGr2D(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009F0AE0)(pThis);
    // CWvsApp::InitializeInput(this);
    reinterpret_cast<void (__cdecl*)()>(0x009F10B0)(); // TSingleton<CInputSystem>::CreateInstance();
    reinterpret_cast<void (__thiscall*)(CInputSystem*, HWND, void**)>(0x00573D90)(CInputSystem::GetInstance(), pThis->m_hWnd(), pThis->m_ahInput()); // CInputSystem::Init(Instance, MainWindow, this->m_ahInput);

    ShowWindow(pThis->m_hWnd(), SW_SHOW);
    UpdateWindow(pThis->m_hWnd());
    SetForegroundWindow(pThis->m_hWnd());
    CHECK_HRESULT(get_gr()->raw_RenderFrame());

    // CWvsApp::InitializeSound(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009F33C0)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Loading Data...");
    // CWvsApp::InitializeGameData(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009F18C0)(pThis);

    // TSingleton<CQuestMan>::CreateInstance()->LoadDemand();
    auto pQuestMan = reinterpret_cast<void* (__cdecl*)()>(0x009EB420)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x006CA320)(pQuestMan)) {
        ErrorMessage("Failed to load quest data.");
    }
    // CQuestMan::LoadPartyQuestInfo(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x006CBB00)(pQuestMan);
    // CQuestMan::LoadExclusive(pQuestMan);
    reinterpret_cast<int (__thiscall*)(void*)>(0x006BF910)(pQuestMan);

    DEBUG_MESSAGE("CwvsApp::SetUp - Complete!");
    // TSingleton<CMonsterBookMan>::CreateInstance()->LoadBook();
    auto pMonsterBookMan = reinterpret_cast<void* (__cdecl*)()>(0x009F37C0)();
    if (!reinterpret_cast<int (__thiscall*)(void*)>(0x0066A8F0)(pMonsterBookMan)) {
        ErrorMessage("Failed to load monster book data.");
    }
    // CWvsApp::CreateWndManager(this);
    reinterpret_cast<void (__thiscall*)(CWvsApp*)>(0x009EB2E0)(pThis);
    // CConfig::ApplySysOpt(TSingleton<CConfig>::GetInstance(), nullptr, 0);
    reinterpret_cast<void (__thiscall*)(CConfig*, CONFIG_SYSOPT*, int)>(0x004B3020)(CConfig::GetInstance(), nullptr, 0);
    // TSingleton<CActionMan>::CreateInstance()->Init();
    auto pActionMan = reinterpret_cast<void* (__cdecl*)()>(0x009EB520)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x0041BD80)(pActionMan);
    // TSingleton<CAnimationDisplayer>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x009EB5A0)();
    // TSingleton<CMapleTVMan>::CreateInstance()->Init()
    auto pMapleTVMan = reinterpret_cast<void* (__cdecl*)()>(0x009EB900)();
    reinterpret_cast<void (__thiscall*)(void*)>(0x00613E70)(pMapleTVMan);
    // TSingleton<CRadioManager>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x009EB9F0)();
    // TSingleton<CCurseProcess>::CreateInstance();
    reinterpret_cast<void* (__cdecl*)()>(0x009EBAD0)();

    // (CLogo*) operator new(0x48); -> (CLogin*) operator new(0x2C8);
    void* pStage = ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x2C8);
    if (pStage) {
        // CLogo::CLogo(pStage); -> CLogin::Clogin(pStage);
        reinterpret_cast<void (__thiscall*)(void*)>(0x005DFAA0)(pStage);
    }
    // set_stage(pStage, nullptr);
    reinterpret_cast<void (__cdecl*)(void*, void*)>(0x007253F0)(pStage, nullptr);
}


typedef void (__thiscall* CWvsApp__Run_t)(CWvsApp*, int*);
static auto CWvsApp__Run = reinterpret_cast<CWvsApp__Run_t>(0x009EF310);

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
        void* pStage = *reinterpret_cast<void**>(0x00CAAAD8 + 4); // ZRef<CStage> g_pStage
        if (pStage) {
            // pStage->Update();
            void** vtable = *reinterpret_cast<void***>(pStage);
            reinterpret_cast<void (__thiscall*)(void*)>(vtable[0])(pStage);
        }
        // CWndMan::s_Update();
        reinterpret_cast<void (__cdecl*)()>(0x009D6490)();
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
        reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004B0F40)(CClientSocket::GetInstance());
    }
    do {
        DWORD dwRet = MsgWaitForMultipleObjects(3, pThis->m_ahInput(), 0, 0, 0xFF);
        if (dwRet <= 2) {
            // CInputSystem::UpdateDevice(TSingleton<CInputSystem>::GetInstance(), dwRet);
            reinterpret_cast<void (__thiscall*)(CInputSystem*, int)>(0x00573A40)(CInputSystem::GetInstance(), dwRet);
            do {
                // if (!CInputSystem::GetISMessage(TSingleton<CInputSystem>::GetInstance(), &isMsg))
                if (!reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x00572C20)(CInputSystem::GetInstance(), &isMsg)) {
                    break;
                }
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009EAF60)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            } while (!*pbTerminate);
        } else if (dwRet == 3) {
            do {
                if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
                // if (CWvsApp::ExtractComErrorCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x009EA630)(pThis, &hr)) {
                    _com_issue_error(hr);
                }
                // if (CWvsApp::ExtractZExceptionCode(this, &hr))
                if (reinterpret_cast<int (__thiscall*)(CWvsApp*, HRESULT*)>(0x009EA600)(pThis, &hr)) {
                    ZException exception(hr);
                    if (hr == 0x20000000) {
                        // CPatchException::CPatchException(&exception, this->m_nTargetVersion);
                        reinterpret_cast<void (__thiscall*)(void*, unsigned short)>(0x005228B0)(&exception, pThis->m_nTargetVersion());
                    } else if (hr >= 0x21000000 && hr <= 0x21000006) {
                        // CDisconnectException::CDisconnectException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00429850)(&exception, hr);
                    } else if (hr >= 0x22000000 && hr <= 0x2200000E) {
                        // CTerminateException::CTerminateException(&exception, hr);
                        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00401C80)(&exception, hr);
                    }
                    throw exception;
                }
            } while (!*pbTerminate && msg.message != WM_QUIT);
        } else {
            // if (CInputSystem::GenerateAutoKeyDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x00571CC0)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009EAF60)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            // if (CInputSystem::GenerateAutoBtnDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int (__thiscall*)(CInputSystem*, ISMSG*)>(0x00571DF0)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void (__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009EAF60)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            int tCurTime;
            CHECK_HRESULT(get_gr()->get_nextRenderTime(&tCurTime));
            CWvsApp__CallUpdate_hook(pThis, _EDX, tCurTime);
            // CWndMan::RedrawInvalidatedWindows();
            reinterpret_cast<void (__cdecl*)()>(0x009D3CD0)();
            CHECK_HRESULT(get_gr()->raw_RenderFrame());
            Sleep(1);
        }
    } while (!*pbTerminate && msg.message != WM_QUIT);
    if (msg.message == WM_QUIT) {
        PostQuitMessage(0);
    }
}


typedef void (__thiscall* CClientSocket__Connect_t)(CClientSocket*, CClientSocket::CONNECTCONTEXT*);
static auto CClientSocket__Connect = reinterpret_cast<CClientSocket__Connect_t>(0x004B1060);

void __fastcall CClientSocket__Connect_hook(CClientSocket* pThis, void* _EDX, CClientSocket::CONNECTCONTEXT* ctx) {
    DEBUG_MESSAGE("CClientSocket::Connect");
    // ZList<ZInetAddr>::RemoveAll(&this->m_ctxConnect.lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*)>(0x004AEE70)(&pThis->m_ctxConnect().lAddr);
    // ZList<ZInetAddr>::AddTail(&this->m_ctxConnect.lAddr, &ctx->lAddr);
    reinterpret_cast<void (__thiscall*)(ZList<ZInetAddr>*, ZList<ZInetAddr>*)>(0x004B0D70)(&pThis->m_ctxConnect().lAddr, &ctx->lAddr);
    pThis->m_ctxConnect().posList = ctx->posList;
    pThis->m_ctxConnect().bLogin = ctx->bLogin;
    pThis->m_ctxConnect().posList = pThis->m_ctxConnect().lAddr._m_pHead;
    // ZList<ZInetAddr>::GetNext(&this->m_ctxConnect.posList);
    ZInetAddr* pAddr = reinterpret_cast<ZInetAddr* (__cdecl*)(ZInetAddr**)>(0x004AEE40)(&pThis->m_ctxConnect().posList);

    DEBUG_MESSAGE("CClientSocket::Connect (addr)");
    // CClientSocket::ClearSendReceiveCtx(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004AF090)(pThis);
    // ZSocketBase::CloseSocket(&this->m_sock);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*)>(0x004ADD00)(&pThis->m_sock());
    // ZSocketBase::Socket(&this->m_sock, 1, 2, 0);
    reinterpret_cast<void (__thiscall*)(ZSocketBase*, int, int, int)>(0x004ADD20)(&pThis->m_sock(), 1, 2, 0);
    // CClientSocket::SetTimeout(this);
    reinterpret_cast<void (__thiscall*)(CClientSocket*)>(0x004AD970)(pThis);
    if (WSAAsyncSelect(pThis->m_sock()._m_hSocket, pThis->m_hWnd(), 0x401, 0x33) == -1 ||
            connect(pThis->m_sock()._m_hSocket, reinterpret_cast<const sockaddr*>(pAddr), 16) != -1 ||
            WSAGetLastError() != WSAEWOULDBLOCK) {
        // CClientSocket::OnConnect(this, 0);
    DEBUG_MESSAGE("CClientSocket::OnConnect");
        reinterpret_cast<void (__thiscall*)(CClientSocket*, int)>(0x004AFE00)(pThis, 0);
    DEBUG_MESSAGE("CClientSocket::OnConnect done");
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
static auto CLogin__SendCheckPasswordPacket = reinterpret_cast<CLogin__SendCheckPasswordPacket_t>(0x005E0040);

int __fastcall CLogin__SendCheckPasswordPacket_hook(CLogin* pThis, void* _EDX, char* sID, char* sPasswd) {
    if (pThis->m_bRequestSent()) {
        return 0;
    }
    pThis->m_bRequestSent() = 1;
    // ZArray<CLogin::WORLDITEM>::RemoveAll(&this->m_WorldItem);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::WORLDITEM>*)>(0x005DE760)(&pThis->m_WorldItem());
    // ZArray<CLogin::BALLOON>::RemoveAll(&this->m_aBallon);
    reinterpret_cast<void (__thiscall*)(ZArray<CLogin::BALLOON>*)>(0x005DA620)(&pThis->m_aBalloon());

    CSystemInfo si;
    // CSystemInfo::Init(&si);
    reinterpret_cast<void (__thiscall*)(CSystemInfo*)>(0x00A4C3C0)(&si);

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


static uintptr_t CWvsContext__OnEnterField_jmp = 0x00A06627;
static uintptr_t CWvsContext__OnEnterField_ret = 0x00A06A9D;

void __fastcall CWvsContext__OnEnterField_helper(CWvsContext* pThis, void* _EDX) {
    // CTemporaryStatView::Show(&this->m_temporaryStatView);
    reinterpret_cast<void (__thiscall*)(void*)>(0x00768AA0)(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pThis) + 0x3610));
    // TODO - implement other skipped functions
}

void __declspec(naked) CWvsContext__OnEnterField_hook() {
    __asm {
        mov     ecx, [ ebp - 0x54 ] ; CWvsContext *
        call    CWvsContext__OnEnterField_helper
        jmp     [ CWvsContext__OnEnterField_ret ]
    };
}


void AttachClientBypass() {
    ATTACH_HOOK(CWvsApp__ctor, CWvsApp__ctor_hook);
    ATTACH_HOOK(CWvsApp__SetUp, CWvsApp__SetUp_hook);
    ATTACH_HOOK(CWvsApp__Run, CWvsApp__Run_hook);
    ATTACH_HOOK(CClientSocket__Connect, CClientSocket__Connect_hook);
    ATTACH_HOOK(CLogin__SendCheckPasswordPacket, CLogin__SendCheckPasswordPacket_hook);

    PatchJmp(CWvsContext__OnEnterField_jmp, reinterpret_cast<uintptr_t>(&CWvsContext__OnEnterField_hook));

    PatchRetZero(0x004AC6D0); // DR_check
    PatchRetZero(0x0045F320); // Hidedll
    PatchRetZero(0x009E93E0); // SendHSLog
    PatchRetZero(0x009E9080); // CeTracer::Run
    PatchRetZero(0x009E90A0); // ShowStartUpWndModal
    PatchRetZero(0x00428F30); // ShowAdBalloon

    // CWzFileSystem::OpenArchive - disable memory mapping
    // LoadLibraryA("NAMESPACE.DLL");
    // Patch1(0x5081E59D + 1, 0x2);
    // Patch1(0x5081EFA3 + 1, 0x2);

    // CAESCipher::s_BasicKey - WZ encryption key
    // LoadLibraryA("PCOM.DLL");
    // PatchStr(0x50C2E000, "\00\00\00\00");
}