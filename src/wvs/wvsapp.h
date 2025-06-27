#pragma once
#include "hook.h"
#include "ztl/tsingleton.h"
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include "wzlib/gr2d.h"
#include "wzlib/pcom.h"

#include <windows.h>
#include <cstdint>


class CWvsApp : public TSingleton<CWvsApp, 0x00C64314> {
public:
    MEMBER_AT(HWND, 0x4, m_hWnd)
    MEMBER_AT(int32_t, 0x8, m_bPCOMInitialized)
    MEMBER_AT(uint32_t, 0xC, m_dwMainThreadId)
    MEMBER_AT(HHOOK, 0x10, m_hHook)
    MEMBER_AT(int32_t, 0x14, m_bWin9x)
    MEMBER_AT(int32_t, 0x18, m_nOSVersion)
    MEMBER_AT(int32_t, 0x1C, m_nOSMinorVersion)
    MEMBER_AT(int32_t, 0x20, m_nOSBuildNumber)
    MEMBER_AT(ZXString<char>, 0x24, m_sCSDVersion)
    MEMBER_AT(int32_t, 0x28, m_b64BitInfo)
    MEMBER_AT(int32_t, 0x2C, m_tUpdateTime)
    MEMBER_AT(int32_t, 0x30, m_bFirstUpdate)
    MEMBER_AT(ZXString<char>, 0x34, m_sCmdLine)
    MEMBER_AT(int32_t, 0x38, m_nGameStartMode)
    MEMBER_AT(int32_t, 0x3C, m_bAutoConnect)
    MEMBER_AT(int32_t, 0x40, m_bShowAdBalloon)
    MEMBER_AT(int32_t, 0x44, m_bExitByTitleEscape)
    MEMBER_AT(HRESULT, 0x48, m_hrZExceptionCode)
    MEMBER_AT(HRESULT, 0x4C, m_hrComErrorCode)
    MEMBER_AT(int32_t, 0x54, m_nTargetVersion)
    MEMBER_AT(int32_t, 0x58, m_tLastServerIPCheck)
    MEMBER_AT(int32_t, 0x5C, m_tLastServerIPCheck2)
    MEMBER_AT(int32_t, 0x60, m_tLastGGHookingAPICheck)
    MEMBER_AT(int32_t, 0x64, m_tLastSecurityCheck)
    MEMBER_ARRAY_AT(void*, 0x68, m_ahInput, 3)
    MEMBER_AT(int32_t, 0x74, m_tNextSecurityCheck)
    MEMBER_AT(bool, 0x78, m_bEnabledDX9)
    MEMBER_AT(ZArray<uint8_t>, 0x7C, m_pBackupBuffer)
    MEMBER_AT(uint32_t, 0x80, m_dwBackupBufferSize)
    MEMBER_AT(uint32_t, 0x84, m_dwClearStackLog)
    MEMBER_AT(int32_t, 0x88, m_bWindowActive)

    // called in bypass.cpp -> CWvsApp::SetUp
    void InitializeResMan();

    static void Dir_BackSlashToSlash(char* sDir);
    static void Dir_SlashToBackSlash(char* sDir);
    static void Dir_upDir(char* sDir);
};