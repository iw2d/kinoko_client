#include "pch.h"
#include "hook.h"
#include "debug.h"
#include "wvs/wvsapp.h"
#include "wvs/inputsystem.h"
#include "wvs/config.h"
#include "wvs/stage.h"
#include "wvs/login.h"
#include "wvs/clientsocket.h"
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include "ztl/ztl.h"

#include <windows.h>
#include <timeapi.h>

#pragma warning(disable : 4996)

ZALLOC_GLOBAL
ZALLOCEX(ZAllocAnonSelector, 0x00C6E67C)
ZALLOCEX(ZAllocStrSelector<char>, 0x00C6E6A8)
ZALLOCEX(ZAllocStrSelector<wchar_t>, 0x00C6E64C)
ZRECYCLABLE(ZInetAddr, 0x00C63EE4)

static auto CWvsApp__ctor = 0x009CA8A0;

void __fastcall CWvsApp__ctor_hook(CWvsApp* pThis, void* _EDX, const char* sCmdLine) {
    DEBUG_MESSAGE("CWvsApp::CWvsApp");
    CWvsApp::ms_pInstance = pThis;
    pThis->m_hWnd = nullptr;
    pThis->m_bPCOMInitialized = 0;
    pThis->m_hHook = nullptr;
    pThis->m_nOSVersion = 0;
    pThis->m_nOSMinorVersion = 0;
    pThis->m_nOSBuildNumber = 0;
    construct<ZXString<char>>(&pThis->m_sCSDVersion);
    pThis->m_b64BitInfo = 0;
    pThis->m_tUpdateTime = 0;
    pThis->m_bFirstUpdate = 1;
    construct<ZXString<char>>(&pThis->m_sCmdLine);
    pThis->m_nGameStartMode = 0;
    pThis->m_bAutoConnect = 1;
    pThis->m_bShowAdBalloon = 0;
    pThis->m_bExitByTitleEscape = 0;
    pThis->m_hrZExceptionCode = 0;
    pThis->m_hrComErrorCode = 0;
    pThis->m_tNextSecurityCheck = 0;
    pThis->m_bEnabledDX9 = true;
    construct<ZArray<uint8_t>>(&pThis->m_pBackupBuffer);
    pThis->m_dwBackupBufferSize = 0;
    pThis->m_dwClearStackLog = 0;
    pThis->m_bWindowActive = 1;

    pThis->m_pBackupBuffer.Alloc(0x1000);
    pThis->m_nGameStartMode = 2;
    pThis->m_dwMainThreadId = GetCurrentThreadId();

    OSVERSIONINFOA ovi;
    ovi.dwOSVersionInfoSize = 148;
    GetVersionExA(&ovi);
    pThis->m_bWin9x = ovi.dwPlatformId == 1;
    if (ovi.dwMajorVersion >= 6 && !pThis->m_nGameStartMode) {
        pThis->m_nGameStartMode = 2;
    }

    typedef BOOL(WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    auto fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandleA("KERNEL32"), "IsWow64Process"));
    BOOL bIs64 = 0;
    if (fnIsWow64Process) {
        fnIsWow64Process(GetCurrentProcess(), &bIs64);
    }
    if (ovi.dwMajorVersion >= 6 && !bIs64) {
        // ResetLSP();
        reinterpret_cast<void(__cdecl*)()>(0x0045ECD0)();
    }

    // CWvsApp::SetClearStackLog(this, (bIs64 << 8) + (this->m_nOSVersion << 12));
    reinterpret_cast<void(__thiscall*)(CWvsApp*, unsigned int)>(0x009C1960)(pThis, (bIs64 << 8) + (pThis->m_nOSVersion << 12));
    pThis->m_nOSVersion = ovi.dwMajorVersion;
    pThis->m_nOSMinorVersion = ovi.dwMinorVersion;
    pThis->m_nOSBuildNumber = ovi.dwBuildNumber;
    pThis->m_sCSDVersion = ovi.szCSDVersion;
    pThis->m_b64BitInfo = bIs64;
}


static auto CWvsApp__SetUp = 0x009CAFB0;

void __fastcall CWvsApp__SetUp_hook(CWvsApp* pThis, void* _EDX) {
    DEBUG_MESSAGE("CWvsApp::SetUp");
    // CWvsApp::InitializeAuth(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C3AD0)(pThis);
    srand(timeGetTime());
    // GetSEPrivilege();
    reinterpret_cast<void(__cdecl*)()>(0x0045E030)();

    DEBUG_MESSAGE("CWvsApp::SetUp - Initializing...");
    // TSingleton<CConfig>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C2420)();
    // CWvsApp::InitializePCOM(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C16C0)(pThis);
    // CWvsApp::CreateMainWindow(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C74A0)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Connecting to server...");
    // TSingleton<CClientSocket>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C23A0)();
    // CWvsApp::ConnectLogin(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C1B30)(pThis);
    // TSingleton<CFuncKeyMappedMan>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C2510)();
    // TSingleton<CQuickslotKeyMappedMan>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C27E0)();
    // TSingleton<CMacroSysMan>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C2590)();
    // TSingleton<CBattleRecordMan>::CreateInstance();
    reinterpret_cast<void(__cdecl*)()>(0x009C2220)();
    // CWvsApp::InitializeResMan(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C9540)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Graphic & Sound...");
    // CWvsApp::InitializeGr2D(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C7670)(pThis);
    // CWvsApp::InitializeInput(this);
    reinterpret_cast<void(__cdecl*)()>(0x009C7C30)();
    reinterpret_cast<void(__thiscall*)(CInputSystem*, HWND, void**)>(0x00571A60)(CInputSystem::GetInstance(), pThis->m_hWnd, pThis->m_ahInput);

    ShowWindow(pThis->m_hWnd, SW_SHOW);
    UpdateWindow(pThis->m_hWnd);
    SetForegroundWindow(pThis->m_hWnd);
    CHECK_HR(get_gr()->raw_RenderFrame());
    // CWvsApp::InitializeSound(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009CA170)(pThis);

    DEBUG_MESSAGE("CWvsApp::SetUp - Loading Data...");
    // CWvsApp::InitializeGameData(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C8440)(pThis);
    // TSingleton<CQuestMan>::CreateInstance()->LoadDemand();
    auto pQuestMan = reinterpret_cast<void*(__cdecl*)()>(0x009C21A0)();
    if (!reinterpret_cast<int(__thiscall*)(void*)>(0x006C3D60)(pQuestMan)) {
        ErrorMessage("Failed to load quest data.");
    }
    // CQuestMan::LoadPartyQuestInfo(pQuestMan);
    reinterpret_cast<int(__thiscall*)(void*)>(0x006C5540)(pQuestMan);
    // CQuestMan::LoadExclusive(pQuestMan);
    reinterpret_cast<int(__thiscall*)(void*)>(0x006B9670)(pQuestMan);

    DEBUG_MESSAGE("CwvsApp::SetUp - Complete!");
    // TSingleton<CMonsterBookMan>::CreateInstance()->LoadBook();
    auto pMonsterBookMan = reinterpret_cast<void*(__cdecl*)()>(0x009CA820)();
    if (!reinterpret_cast<int(__thiscall*)(void*)>(0x00664C10)(pMonsterBookMan)) {
        ErrorMessage("Failed to load monster book data.");
    }
    // CWvsApp::CreateWndManager(this);
    reinterpret_cast<void(__thiscall*)(CWvsApp*)>(0x009C2060)(pThis);
    // CConfig::ApplySysOpt(TSingleton<CConfig>::GetInstance(), nullptr, 0);
    reinterpret_cast<void(__thiscall*)(CConfig*, CONFIG_SYSOPT*, int)>(0x004B2300)(CConfig::GetInstance(), nullptr, 0);
    // TSingleton<CActionMan>::CreateInstance()->Init();
    auto pActionMan = reinterpret_cast<void*(__cdecl*)()>(0x009C22A0)();
    reinterpret_cast<void(__thiscall*)(void*)>(0x0041BEB0)(pActionMan);
    // TSingleton<CAnimationDisplayer>::CreateInstance();
    reinterpret_cast<void*(__cdecl*)()>(0x009C2320)();
    // TSingleton<CMapleTVMan>::CreateInstance()->Init()
    auto pMapleTVMan = reinterpret_cast<void*(__cdecl*)()>(0x009C2680)();
    reinterpret_cast<void(__thiscall*)(void*)>(0x0060FBC0)(pMapleTVMan);
    // TSingleton<CRadioManager>::CreateInstance();
    reinterpret_cast<void*(__cdecl*)()>(0x009C2770)();

    // (CLogo*) operator new(0x48); -> (CLogin*) operator new(0x2C8);
    CStage* pStage = static_cast<CStage*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x2C8));
    if (pStage) {
        // CLogo::CLogo(pStage); -> CLogin::Clogin(pStage);
        reinterpret_cast<void(__thiscall*)(void*)>(0x005DB440)(pStage);
    }
    set_stage(pStage, nullptr);
}


static auto CWvsApp__Run = 0x009C5F00;

void __fastcall CWvsApp__CallUpdate_hook(CWvsApp* pThis, void* _EDX, int32_t tCurTime) {
    if (pThis->m_bFirstUpdate) {
        pThis->m_tUpdateTime = tCurTime;
        pThis->m_tLastServerIPCheck = tCurTime;
        pThis->m_tLastServerIPCheck2 = tCurTime;
        pThis->m_tLastGGHookingAPICheck = tCurTime;
        pThis->m_tLastSecurityCheck = tCurTime;
        pThis->m_bFirstUpdate = 0;
    }
    while (tCurTime - pThis->m_tUpdateTime > 0) {
        ZRef<CStage> pStage(get_stage());
        if (pStage) {
            pStage->Update();
        }
        // CWndMan::s_Update();
        reinterpret_cast<void(__cdecl*)()>(0x009B4B00)();
        pThis->m_tUpdateTime += 30;
        if (tCurTime - pThis->m_tUpdateTime > 0) {
            CHECK_HR(get_gr()->raw_UpdateCurrentTime(pThis->m_tUpdateTime));
        }
    }
    CHECK_HR(get_gr()->raw_UpdateCurrentTime(tCurTime));
}

void __fastcall CWvsApp__Run_hook(CWvsApp* pThis, void* _EDX, int32_t* pbTerminate) {
    HRESULT hr;
    tagMSG msg;
    ISMSG isMsg;
    memset(&msg, 0, sizeof(msg));
    memset(&isMsg, 0, sizeof(isMsg));
    if (CClientSocket::IsInstantiated()) {
        // CClientSocket::ManipulatePacket(TSingleton<CClientSocket>::GetInstance());
        reinterpret_cast<void(__thiscall*)(CClientSocket*)>(0x004B0220)(CClientSocket::GetInstance());
    }
    do {
        DWORD dwRet = MsgWaitForMultipleObjects(3, pThis->m_ahInput, 0, 0, 0xFF);
        if (dwRet <= 2) {
            // CInputSystem::UpdateDevice(TSingleton<CInputSystem>::GetInstance(), dwRet);
            reinterpret_cast<void(__thiscall*)(CInputSystem*, int32_t)>(0x00571710)(CInputSystem::GetInstance(), dwRet);
            do {
                // if (!CInputSystem::GetISMessage(TSingleton<CInputSystem>::GetInstance(), &isMsg))
                if (!reinterpret_cast<int(__thiscall*)(CInputSystem*, ISMSG*)>(0x005708F0)(CInputSystem::GetInstance(), &isMsg)) {
                    break;
                }
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void(__thiscall*)(CWvsApp*, uint32_t, uint32_t, int32_t)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            } while (!*pbTerminate);
        } else if (dwRet == 3) {
            do {
                if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
                // if (CWvsApp::ExtractComErrorCode(this, &hr))
                if (reinterpret_cast<int(__thiscall*)(CWvsApp*, HRESULT*)>(0x009C0860)(pThis, &hr)) {
                    _com_issue_error(hr);
                }
                // if (CWvsApp::ExtractZExceptionCode(this, &hr))
                if (reinterpret_cast<int(__thiscall*)(CWvsApp*, HRESULT*)>(0x009C0830)(pThis, &hr)) {
                    ZException exception(hr);
                    if (hr == 0x20000000) {
                        // CPatchException::CPatchException(&exception, this->m_nTargetVersion);
                        reinterpret_cast<void(__thiscall*)(void*, int32_t)>(0x00520FA0)(&exception, pThis->m_nTargetVersion);
                    } else if (hr >= 0x21000000 && hr <= 0x21000006) {
                        // CDisconnectException::CDisconnectException(&exception, hr);
                        reinterpret_cast<void(__thiscall*)(void*, HRESULT)>(0x00429860)(&exception, hr);
                    } else if (hr >= 0x22000000 && hr <= 0x2200000E) {
                        // CTerminateException::CTerminateException(&exception, hr);
                        reinterpret_cast<void(__thiscall*)(void*, HRESULT)>(0x00401D50)(&exception, hr);
                    }
                    throw exception;
                }
            } while (!*pbTerminate && msg.message != WM_QUIT);
        } else {
            // if (CInputSystem::GenerateAutoKeyDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int(__thiscall*)(CInputSystem*, ISMSG*)>(0x0056F990)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void(__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            // if (CInputSystem::GenerateAutoBtnDown(TSingleton<CInputSystem>::GetInstance(), &isMsg))
            if (reinterpret_cast<int(__thiscall*)(CInputSystem*, ISMSG*)>(0x0056FAC0)(CInputSystem::GetInstance(), &isMsg)) {
                // CWvsApp::ISMsgProc(this, isMsg.message, isMsg.wParam, isMsg.lParam);
                reinterpret_cast<void(__thiscall*)(CWvsApp*, unsigned int, unsigned int, int)>(0x009C1CE0)(pThis, isMsg.message, isMsg.wParam, isMsg.lParam);
            }
            int tCurTime;
            CHECK_HR(get_gr()->get_nextRenderTime(&tCurTime));
            CWvsApp__CallUpdate_hook(pThis, _EDX, tCurTime);
            // CWndMan::RedrawInvalidatedWindows();
            reinterpret_cast<void(__cdecl*)()>(0x009B2340)();
            CHECK_HR(get_gr()->raw_RenderFrame());
            Sleep(1);
        }
    } while (!*pbTerminate && msg.message != WM_QUIT);
    if (msg.message == WM_QUIT) {
        PostQuitMessage(0);
    }
}


static auto CClientSocket__Connect = 0x004B0340;

void __fastcall CClientSocket__Connect_hook(CClientSocket* pThis, void* _EDX, CClientSocket::CONNECTCONTEXT* ctx) {
    DEBUG_MESSAGE("CClientSocket::Connect");
    pThis->m_ctxConnect.lAddr.RemoveAll();
    pThis->m_ctxConnect.lAddr.AddTail(ctx->lAddr);
    pThis->m_ctxConnect.posList = ctx->posList;
    pThis->m_ctxConnect.bLogin = ctx->bLogin;
    pThis->m_ctxConnect.posList = &pThis->m_ctxConnect.lAddr.GetHead();
    ZInetAddr& next = ZList<ZInetAddr>::GetNext(pThis->m_ctxConnect.posList);

    DEBUG_MESSAGE("CClientSocket::Connect (addr)");
    // CClientSocket::ClearSendReceiveCtx(this);
    reinterpret_cast<void(__thiscall*)(CClientSocket*)>(0x004AE1A0)(pThis);
    // ZSocketBase::CloseSocket(&this->m_sock);
    reinterpret_cast<void(__thiscall*)(ZSocketBase*)>(0x004ACF30)(&pThis->m_sock);
    // ZSocketBase::Socket(&this->m_sock, 1, 2, 0);
    reinterpret_cast<void(__thiscall*)(ZSocketBase*, int, int, int)>(0x004ACF50)(&pThis->m_sock, 1, 2, 0);
    // CClientSocket::SetTimeout(this);
    reinterpret_cast<void(__thiscall*)(CClientSocket*)>(0x004ACBA0)(pThis);
    if (WSAAsyncSelect(pThis->m_sock, pThis->m_hWnd, 0x401, 0x33) == -1 ||
        connect(pThis->m_sock, next, 16) != -1 ||
        WSAGetLastError() != WSAEWOULDBLOCK) {
        // CClientSocket::OnConnect(this, 0);
        reinterpret_cast<void(__thiscall*)(CClientSocket*, int)>(0x004AEF10)(pThis, 0);
    }
}


static auto CLogin__SendCheckPasswordPacket = 0x005DB9D0;

int32_t __fastcall CLogin__SendCheckPasswordPacket_hook(CLogin* pThis, void* _EDX, char* sID, char* sPasswd) {
    if (pThis->m_bRequestSent) {
        return 0;
    }
    pThis->m_bRequestSent = 1;
    pThis->m_WorldItem.RemoveAll();
    pThis->m_aBalloon.RemoveAll();

    CSystemInfo si;
    // CSystemInfo::Init(&si);
    reinterpret_cast<void(__thiscall*)(CSystemInfo*)>(0x00A1F1B0)(&si);

    COutPacket oPacket(1); // CP_CheckPassword
    oPacket.EncodeStr(sID);
    oPacket.EncodeStr(sPasswd);
    oPacket.EncodeBuffer(si.MachineId, 16);
    oPacket.Encode4(0); // CSystemInfo::GetGameRoomClient(&v15)
    oPacket.Encode1(CWvsApp::GetInstance()->m_nGameStartMode);
    oPacket.Encode1(0);
    oPacket.Encode1(0);
    oPacket.Encode4(0); // CConfig::GetPartnerCode(TSingleton<CConfig>::ms_pInstance._m_pStr)
    CClientSocket::GetInstance()->SendPacket(oPacket);
    return 0;
}


void AttachClientBypass() {
    ATTACH_HOOK(CWvsApp__ctor, CWvsApp__ctor_hook);
    ATTACH_HOOK(CWvsApp__SetUp, CWvsApp__SetUp_hook);
    ATTACH_HOOK(CWvsApp__Run, CWvsApp__Run_hook);
    ATTACH_HOOK(CClientSocket__Connect, CClientSocket__Connect_hook);
    ATTACH_HOOK(CLogin__SendCheckPasswordPacket, CLogin__SendCheckPasswordPacket_hook);

    PatchJmp(0x009DBEEE, 0x009DC278); // CWvsContext::OnEnterField
    PatchJmp(0x004AFCF3, 0x004AFE30); // CClientSocket::OnAliveReq

    PatchRetZero(0x004AB900); // DR_check
    PatchRetZero(0x0045EBD0); // Hidedll
    PatchRetZero(0x009BF6C0); // SendHSLog
    PatchRetZero(0x009BF370); // CeTracer::Run
    PatchRetZero(0x009BF390); // ShowStartUpWndModal
    PatchRetZero(0x00429000); // ShowAdBalloon
}