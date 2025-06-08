#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"

#include <map>
#include <fstream>


static uintptr_t CDropPool__TryPickUpDropByPet_jmp = 0x00511AED;
static uintptr_t CDropPool__TryPickUpDropByPet_ret = 0x00511AFC;

void __declspec(naked) CDropPool__TryPickUpDropByPet_hook() {
    __asm {
        lea     edx, [ eax - 200 ]      ; x1
        mov     [ ebp - 0x34 ], edx
        lea     edx, [ ecx - 200 ]      ; y1
        add     eax, 200                ; x2
        add     ecx, 200                ; y2
        jmp     [ CDropPool__TryPickUpDropByPet_ret ]
    };
}


typedef void (__cdecl* CCurseProcess__ProcessString_t)(char*, int*, int);
static auto CCurseProcess__ProcessString = reinterpret_cast<CCurseProcess__ProcessString_t>(0x007477F0);

int __cdecl CCurseProcess__ProcessString_hook(char* sString, int* pbFiltered, int bIgnoreNewLine) {
    return 1;
}


typedef void (__thiscall* CInputSystem__DetectJoystick_t)(CInputSystem*);
static auto CInputSystem__DetectJoystick = reinterpret_cast<CInputSystem__DetectJoystick_t>(0x00571740);

void __fastcall CInputSystem__DetectJoystick_hook(void* pThis, void* _EDX) {
    // noop
}


class CVecCtrl;

typedef void (__thiscall* CVecCtrl__SetImpactNext_t)(CVecCtrl*, int, long double, long double);
static auto CVecCtrl__SetImpactNext = reinterpret_cast<CVecCtrl__SetImpactNext_t>(0x00905CD0);

void __fastcall CVecCtrl__SetImpactNext_hook(CVecCtrl* pThis, void* _EDX, int nAttr, long double vx, long double vy) {
    if (nAttr == 0x14 && CInputSystem::GetInstance()->IsKeyPressed(38)) {
        // MPA_FLASHJUMP + ArrowUp
        vx = 0.0;
        vy = -1000.0;
    }
    CVecCtrl__SetImpactNext(pThis, nAttr, vx, vy);
}


typedef void (__thiscall* CUserLocal__Jump_t)(CUserLocal*, int);
static auto CUserLocal__Jump = reinterpret_cast<CUserLocal__Jump_t>(0x0090A1D0);

class CVecCtrl {
public:
    MEMBER_AT(void*, 0x1A0, m_pfh)
};

void __fastcall CUserLocal__Jump_hook(CUserLocal* pThis, void* _EDX, int bEnforced) {
    CUserLocal__Jump(pThis, bEnforced);
    // Check if on the ground (has foothold), or on ladder/rope (CVecCtrl::GetLadderOrRope)
    CVecCtrl* pVecCtrl = reinterpret_cast<CVecCtrl*>(&static_cast<IWzVector2D*>(pThis->m_pvc())[-3]);
    if (pVecCtrl->m_pfh() || reinterpret_cast<void* (__thiscall*)(CVecCtrl*)>(0x004BBE80)(pVecCtrl)) {
        return;
    }
    // Trigger flash jump skill
    if (!bEnforced && pThis->m_bJumpKeyUp()) {
        int nJob = pThis->GetJobCode();
        int nSkillID = 0;
        if (nJob / 10 == 41) {
            nSkillID = 4111006; // Night Lord
        } else if (nJob / 10 == 42) {
            nSkillID = 4211009; // Shadower
        } else if (nJob / 10 == 43) {
            nSkillID = 4321003; // Dual Blade
        } else if (nJob / 100 == 14) {
            nSkillID = 14101004; // Night Walker
        }
        if (nSkillID) {
            pThis->m_bJumpKeyUp() = false;
            pThis->DoActiveSkill(nSkillID, 0, nullptr);
        }
    }
}


typedef ZXString<char>* (__cdecl* get_weapon_attack_speed_t)(ZXString<char>* result, int nItemID);
static auto get_weapon_attack_speed = reinterpret_cast<get_weapon_attack_speed_t>(0x005A66B0);

const char* get_attack_speed_string(int nAttackSpeed) {
    switch (nAttackSpeed) {
        case 0:
        case 1:
            return " FASTEST";
        case 2:
        case 3:
            return " FASTER";
        case 4:
        case 5:
            return " FAST";
        case 6:
            return " NORMAL";
        case 7:
        case 8:
            return " SLOW";
        case 9:
        case 10:
            return " SLOWER";
        case 11:
        case 12:
            return " SLOWEST";
        default:
            return "";
    }
}

ZXString<char>* __cdecl get_weapon_attack_speed_hook(ZXString<char>* result, int nItemID) {
    result->_m_pStr = nullptr;
    // get_weapon_type(nItemID)
    if (reinterpret_cast<int (__cdecl*)(int)>(0x0046F660)(nItemID)) {
        ZXString<wchar_t> sUOL;
        // get_equip_data_path(&sUOL, nItemID)
        reinterpret_cast<ZXString<wchar_t*> (__cdecl*)(ZXString<wchar_t>*, int)>(0x005A6060)(&sUOL, nItemID);
        if (sUOL._m_pStr && *sUOL._m_pStr) {
            Ztl_variant_t vEmpty;
            Ztl_variant_t vEquip;
            CHECK_HRESULT(get_rm()->raw_GetObject(sUOL._m_pStr, vEmpty, vEmpty, &vEquip));
            IWzPropertyPtr pEquip = IWzPropertyPtr(vEquip.GetUnknown(false, false));
            Ztl_variant_t vInfo;
            CHECK_HRESULT(pEquip->get_item(L"info", &vInfo));
            IWzPropertyPtr pInfo = IWzPropertyPtr(vInfo.GetUnknown(false, false));
            Ztl_variant_t vAttackSpeed;
            CHECK_HRESULT(pInfo->get_item(L"attackSpeed", &vAttackSpeed));

            // get_int32(&vAttackSpeed, 6);
            unsigned int nAttackSpeed = reinterpret_cast<unsigned int (__cdecl*)(Ztl_variant_t*, unsigned int)>(0x00406830)(&vAttackSpeed, 6);
            char sAttackSpeed[20];
            sprintf_s(sAttackSpeed, 20, "%s (%d)", get_attack_speed_string(nAttackSpeed), nAttackSpeed);

            // ZXString<char>::ZXString<char>(result, sAttackSpeed);
            reinterpret_cast<void (__thiscall*)(ZXString<char>*, const char*, int)>(0x0042D230)(result, sAttackSpeed, -1);
        }
    }
    return result;
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetMapString_t)(CItemInfo*, ZXString<char>*, unsigned int, const char*);
static auto CItemInfo__GetMapString = reinterpret_cast<CItemInfo__GetMapString_t>(0x005A9CA0);

ZXString<char>* __fastcall CItemInfo__GetMapString_hook(CItemInfo* pThis, void* _EDX,  ZXString<char>* result, unsigned int dwFieldID, const char* sKey) {
    CItemInfo__GetMapString(pThis, result, dwFieldID, sKey);
    if (!strcmp(sKey, "mapName")) {
        char sFieldID[20];
        snprintf(sFieldID, 20, " (%d)", dwFieldID);
        // ZXString<char>::_Cat(result, sFieldID, strlen(sFieldID));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sFieldID, strlen(sFieldID));
    }
    return result;
}


typedef ZXString<char>* (__thiscall* CItemInfo__GetItemDesc_t)(CItemInfo*, ZXString<char>*, int);
static auto CItemInfo__GetItemDesc = reinterpret_cast<CItemInfo__GetItemDesc_t>(0x005B16E0);

ZXString<char>* __fastcall CItemInfo__GetItemDesc_hook(CItemInfo* pThis, void* _EDX, ZXString<char>* result, int nItemID) {
    CItemInfo__GetItemDesc(pThis, result, nItemID);
    if (result->GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(result, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sNewLine, strlen(sNewLine));
    }
    char sQuestID[30];
    snprintf(sQuestID, 30, "#cItem ID : %d#", nItemID);
    // ZXString<char>::_Cat(result, sQuestID, strlen(sQuestID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(result, sQuestID, strlen(sQuestID));
    return result;
}


class CSkillInfo;

struct SKILLENTRY {
    unsigned char padding[0x264];

    MEMBER_AT(int, 0x0, nSkillID)
    MEMBER_AT(ZXString<char>, 0x4, sName)
    MEMBER_AT(ZXString<char>, 0x8, sDescription)
};

typedef ZRef<SKILLENTRY>* (__thiscall* CSkillInfo__LoadSkill_t)(CSkillInfo*, ZRef<SKILLENTRY>*, int, void*, void*);
static auto CSkillInfo__LoadSkill = reinterpret_cast<CSkillInfo__LoadSkill_t>(0x0070C190);

ZRef<SKILLENTRY>* __fastcall CSkillInfo__LoadSkill_hook(CSkillInfo* pThis, void* _EDX, ZRef<SKILLENTRY>* result, int nSkillID, void* pSkill, void* pStrSR) {
    CSkillInfo__LoadSkill(pThis, result, nSkillID, pSkill, pStrSR);
    if (result->p->sDescription().GetLength() > 0) {
        char* sNewLine = "\r\n";
        // ZXString<char>::_Cat(&result->p->sDescription, sNewLine, strlen(sNewLine));
        reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(&result->p->sDescription(), sNewLine, strlen(sNewLine));
    }
    char sSkillID[30];
    snprintf(sSkillID, 30, "#cSkill ID : %d#", nSkillID);
    // ZXString<char>::_Cat(&result->p->sDescription, sSkillID, strlen(sSkillID));
    reinterpret_cast<ZXString<char>* (__thiscall*)(ZXString<char>*, char*, int)>(0x0042D020)(&result->p->sDescription(), sSkillID, strlen(sSkillID));
    return result;
}


static uintptr_t CUIQuestInfoDetail__Draw_jmp = 0x00824A93;
static uintptr_t CUIQuestInfoDetail__Draw_ret = 0x00824C04;

void __stdcall CUIQuestInfoDetail__Draw_helper(IWzCanvas* pCanvas, IWzFont* pFont, unsigned short usQuestID) {
    wchar_t sQuestID[30];
    swprintf(sQuestID, 30, L"Quest ID : %d", usQuestID);
    Ztl_variant_t vEmpty;
    unsigned int result[4];
    CHECK_HRESULT(pCanvas->raw_DrawText(35, 56, sQuestID, pFont, vEmpty, vEmpty, result));
}

void __declspec(naked) CUIQuestInfoDetail__Draw_hook() {
    __asm {
        mov     eax, [ ebp + 0x8C ]
        movzx   edx, word ptr [ eax ]
        push    edx ; this->m_pQI.p->usQuestID
        mov     eax, [ ebp + 0xF0 ]
        push    eax ; IWzFont*
        push    esi ; IWzCanvas*
        call    CUIQuestInfoDetail__Draw_helper
        jmp     [ CUIQuestInfoDetail__Draw_ret ]
    };
}


void AttachClientHelper() {
    static std::map<std::string, std::string> sZAPI;

    sZAPI["LoadLibraryA"] = "HINSTANCE__ *(__stdcall *LoadLibraryA)(const char *);";
    sZAPI["GetProcAddress"] = "int (__stdcall *(__stdcall *GetProcAddress)(HINSTANCE__ *, const wchar_t *))();";
    sZAPI["GetModuleHandleA"] = "HINSTANCE__ *(__stdcall *GetModuleHandleA)(const char *);";
    sZAPI["GetModuleFileNameA"] = "unsigned int (__stdcall *GetModuleFileNameA)(HINSTANCE__ *, char *, unsigned int);";
    sZAPI["CloseHandle"] = "int (__stdcall *CloseHandle)(void *);";
    sZAPI["GetCurrentProcess"] = "void *(__stdcall *GetCurrentProcess)();";
    sZAPI["GetCurrentProcessId"] = "unsigned int (__stdcall *GetCurrentProcessId)();";
    sZAPI["OpenProcess"] = "void *(__stdcall *OpenProcess)(unsigned int, int, unsigned int);";
    sZAPI["ExitProcess"] = "void (__stdcall *ExitProcess)(unsigned int);";
    sZAPI["TerminateProcess"] = "int (__stdcall *TerminateProcess)(void *, unsigned int);";
    sZAPI["GetExitCodeProcess"] = "int (__stdcall *GetExitCodeProcess)(void *, unsigned int *);";
    sZAPI["CreateProcessA"] = "int (__stdcall *CreateProcessA)(const char *, char *, _SECURITY_ATTRIBUTES *, _SECURITY_ATTRIBUTES *, int, unsigned int, void *, const char *, _STARTUPINFOA *, _PROCESS_INFORMATION *);";
    sZAPI["GetCurrentThread"] = "void *(__stdcall *GetCurrentThread)();";
    sZAPI["GetCurrentThreadId"] = "unsigned int (__stdcall *GetCurrentThreadId)();";
    sZAPI["SetThreadPriority"] = "unsigned int (__stdcall *SetThreadPriority)(void *, int);";
    sZAPI["ResumeThread"] = "unsigned int (__stdcall *ResumeThread)(void *);";
    sZAPI["SuspendThread"] = "unsigned int (__stdcall *SuspendThread)(void *);";
    sZAPI["InterlockedIncrement"] = "int (__stdcall *InterlockedIncrement)(int *);";
    sZAPI["InterlockedDecrement"] = "int (__stdcall *InterlockedDecrement)(int *);";
    sZAPI["InterlockedExchange"] = "int (__stdcall *InterlockedExchange)(int *, int);";
    sZAPI["InterlockedExchangeAdd"] = "int (__stdcall *InterlockedExchangeAdd)(int *, int);";
    sZAPI["CreateSemaphoreA"] = "void *(__stdcall *CreateSemaphoreA)(_SECURITY_ATTRIBUTES *, int, int, const char *);";
    sZAPI["ReleaseSemaphore"] = "int (__stdcall *ReleaseSemaphore)(void *, int, int *);";
    sZAPI["CreateEventA"] = "void *(__stdcall *CreateEventA)(_SECURITY_ATTRIBUTES *, int, int, const char *);";
    sZAPI["SetEvent"] = "int (__stdcall *SetEvent)(void *);";
    sZAPI["ResetEvent"] = "int (__stdcall *ResetEvent)(void *);";
    sZAPI["CreateMutexA"] = "void *(__stdcall *CreateMutexA)(_SECURITY_ATTRIBUTES *, int, const char *);";
    sZAPI["WaitForSingleObject"] = "unsigned int (__stdcall *WaitForSingleObject)(void *, unsigned int);";
    sZAPI["WaitForMultipleObjects"] = "unsigned int (__stdcall *WaitForMultipleObjects)(unsigned int, void *const *, int, unsigned int);";
    sZAPI["Sleep"] = "void (__stdcall *Sleep)(unsigned int);";
    sZAPI["CreateIoCompletionPort"] = "void *(__stdcall *CreateIoCompletionPort)(void *, void *, unsigned int, unsigned int);";
    sZAPI["GetQueuedCompletionStatus"] = "int (__stdcall *GetQueuedCompletionStatus)(void *, unsigned int *, unsigned int *, _OVERLAPPED **, unsigned int);";
    sZAPI["PostQueuedCompletionStatus"] = "int (__stdcall *PostQueuedCompletionStatus)(void *, unsigned int, unsigned int, _OVERLAPPED *);";
    sZAPI["GlobalAlloc"] = "void *(__stdcall *GlobalAlloc)(unsigned int, unsigned int);";
    sZAPI["GlobalFree"] = "void *(__stdcall *GlobalFree)(void *);";
    sZAPI["GlobalLock"] = "void *(__stdcall *GlobalLock)(void *);";
    sZAPI["GlobalUnlock"] = "int (__stdcall *GlobalUnlock)(void *);";
    sZAPI["LocalAlloc"] = "void *(__stdcall *LocalAlloc)(unsigned int, unsigned int);";
    sZAPI["LocalFree"] = "void *(__stdcall *LocalFree)(void *);";
    sZAPI["HeapCreate"] = "void *(__stdcall *HeapCreate)(unsigned int, unsigned int, unsigned int);";
    sZAPI["HeapDestroy"] = "int (__stdcall *HeapDestroy)(void *);";
    sZAPI["HeapAlloc"] = "void *(__stdcall *HeapAlloc)(void *, unsigned int, unsigned int);";
    sZAPI["HeapFree"] = "int (__stdcall *HeapFree)(void *, unsigned int, void *);";
    sZAPI["GetProcessHeap"] = "void *(__stdcall *GetProcessHeap)();";
    sZAPI["CreateFileMappingA"] = "void *(__stdcall *CreateFileMappingA)(void *, _SECURITY_ATTRIBUTES *, unsigned int, unsigned int, unsigned int, const char *);";
    sZAPI["OpenFileMappingA"] = "void *(__stdcall *OpenFileMappingA)(unsigned int, int, const char *);";
    sZAPI["MapViewOfFile"] = "void *(__stdcall *MapViewOfFile)(void *, unsigned int, unsigned int, unsigned int, unsigned int);";
    sZAPI["UnmapViewOfFile"] = "int (__stdcall *UnmapViewOfFile)(const void *);";
    sZAPI["FlushViewOfFile"] = "int (__stdcall *FlushViewOfFile)(const void *, unsigned int);";
    sZAPI["CreateDirectoryA"] = "int (__stdcall *CreateDirectoryA)(const char *, _SECURITY_ATTRIBUTES *);";
    sZAPI["RemoveDirectoryA"] = "int (__stdcall *RemoveDirectoryA)(const char *);";
    sZAPI["GetFileAttributesA"] = "unsigned int (__stdcall *GetFileAttributesA)(const char *);";
    sZAPI["GetFileAttributesExA"] = "int (__stdcall *GetFileAttributesExA)(const char *, _GET_FILEEX_INFO_LEVELS, void *);";
    sZAPI["CreateFileA"] = "void *(__stdcall *CreateFileA)(const char *, unsigned int, unsigned int, _SECURITY_ATTRIBUTES *, unsigned int, unsigned int, void *);";
    sZAPI["ReadFile"] = "int (__stdcall *ReadFile)(void *, void *, unsigned int, unsigned int *, _OVERLAPPED *);";
    sZAPI["WriteFile"] = "int (__stdcall *WriteFile)(void *, const void *, unsigned int, unsigned int *, _OVERLAPPED *);";
    sZAPI["FlushFileBuffers"] = "int (__stdcall *FlushFileBuffers)(void *);";
    sZAPI["GetFileSize"] = "unsigned int (__stdcall *GetFileSize)(void *, unsigned int *);";
    sZAPI["SetFilePointer"] = "unsigned int (__stdcall *SetFilePointer)(void *, int, int *, unsigned int);";
    sZAPI["SetEndOfFile"] = "int (__stdcall *SetEndOfFile)(void *);";
    sZAPI["DeleteFileA"] = "int (__stdcall *DeleteFileA)(const char *);";
    sZAPI["FindFirstFileA"] = "void *(__stdcall *FindFirstFileA)(const char *, _WIN32_FIND_DATAA *);";
    sZAPI["FindNextFileA"] = "int (__stdcall *FindNextFileA)(void *, _WIN32_FIND_DATAA *);";
    sZAPI["FindClose"] = "int (__stdcall *FindClose)(void *);";
    sZAPI["LoadResource"] = "void *(__stdcall *LoadResource)(HINSTANCE__ *, HRSRC__ *);";
    sZAPI["SizeofResource"] = "unsigned int (__stdcall *SizeofResource)(HINSTANCE__ *, HRSRC__ *);";
    sZAPI["LockResource"] = "void *(__stdcall *LockResource)(void *);";
    sZAPI["FindResourceA"] = "HRSRC__ *(__stdcall *FindResourceA)(HINSTANCE__ *, const char *, const char *);";
    sZAPI["FindResourceW"] = "HRSRC__ *(__stdcall *FindResourceW)(HINSTANCE__ *, const wchar_t *, const wchar_t *);";
    sZAPI["GetCurrentDirectoryA"] = "unsigned int (__stdcall *GetCurrentDirectoryA)(unsigned int, char *);";
    sZAPI["GetWindowsDirectoryA"] = "unsigned int (__stdcall *GetWindowsDirectoryA)(char *, unsigned int);";
    sZAPI["GetSystemDirectoryA"] = "unsigned int (__stdcall *GetSystemDirectoryA)(char *, unsigned int);";
    sZAPI["GetTempPathA"] = "unsigned int (__stdcall *GetTempPathA)(unsigned int, char *);";
    sZAPI["GetLastError"] = "unsigned int (__stdcall *GetLastError)();";
    sZAPI["SetLastError"] = "void (__stdcall *SetLastError)(unsigned int);";
    sZAPI["GetTickCount"] = "unsigned int (__stdcall *GetTickCount)();";
    sZAPI["GetLocalTime"] = "void (__stdcall *GetLocalTime)(_SYSTEMTIME *);";
    sZAPI["CompareFileTime"] = "int (__stdcall *CompareFileTime)(const _FILETIME *, const _FILETIME *);";
    sZAPI["FileTimeToSystemTime"] = "int (__stdcall *FileTimeToSystemTime)(const _FILETIME *, _SYSTEMTIME *);";
    sZAPI["lstrcatA"] = "char *(__stdcall *lstrcatA)(char *, const char *);";
    sZAPI["lstrcpyA"] = "char *(__stdcall *lstrcpyA)(char *, const char *);";
    sZAPI["lstrcmpiA"] = "int (__stdcall *lstrcmpiA)(const char *, const char *);";
    sZAPI["lstrlenA"] = "int (__stdcall *lstrlenA)(const char *);";
    sZAPI["lstrlenW"] = "int (__stdcall *lstrlenW)(const wchar_t *);";
    sZAPI["MultiByteToWideChar"] = "int (__stdcall *MultiByteToWideChar)(unsigned int, unsigned int, const char *, int, wchar_t *, int);";
    sZAPI["WideCharToMultiByte"] = "int (__stdcall *WideCharToMultiByte)(unsigned int, unsigned int, const wchar_t *, int, char *, int, const char *, int *);";
    sZAPI["WinExec"] = "unsigned int (__stdcall *WinExec)(const char *, unsigned int);";
    sZAPI["GetACP"] = "unsigned int (__stdcall *GetACP)();";
    sZAPI["GetSystemInfo"] = "void (__stdcall *GetSystemInfo)(_SYSTEM_INFO *);";
    sZAPI["GetVersionExA"] = "int (__stdcall *GetVersionExA)(_OSVERSIONINFOA *);";
    sZAPI["GetCommandLineA"] = "char *(__stdcall *GetCommandLineA)();";
    sZAPI["GetVolumeInformationA"] = "int (__stdcall *GetVolumeInformationA)(const char *, char *, unsigned int, unsigned int *, unsigned int *, unsigned int *, char *, unsigned int);";
    sZAPI["FormatMessageA"] = "unsigned int (__stdcall *FormatMessageA)(unsigned int, const void *, unsigned int, unsigned int, char *, unsigned int, char **);";
    sZAPI["OutputDebugStringA"] = "void (__stdcall *OutputDebugStringA)(const char *);";
    sZAPI["VirtualQuery"] = "unsigned int (__stdcall *VirtualQuery)(const void *, _MEMORY_BASIC_INFORMATION *, unsigned int);";
    sZAPI["DisableThreadLibraryCalls"] = "int (__stdcall *DisableThreadLibraryCalls)(HINSTANCE__ *);";
    sZAPI["QueryPerformanceFrequency"] = "void *(__stdcall *QueryPerformanceFrequency)(_LARGE_INTEGER *);";
    sZAPI["QueryPerformanceCounter"] = "int (__stdcall *QueryPerformanceCounter)(_LARGE_INTEGER *);";
    sZAPI["UnionRect"] = "int (__stdcall *UnionRect)(tagRECT *, const tagRECT *, const tagRECT *);";
    sZAPI["SetRect"] = "int (__stdcall *SetRect)(tagRECT *, int, int, int, int);";
    sZAPI["SetRectEmpty"] = "int (__stdcall *SetRectEmpty)(tagRECT *);";
    sZAPI["wsprintfA"] = "int (*wsprintfA)(char *, const char *, ...);";
    sZAPI["DefWindowProcA"] = "int (__stdcall *DefWindowProcA)(HWND__ *, unsigned int, unsigned int, int);";
    sZAPI["PostQuitMessage"] = "void (__stdcall *PostQuitMessage)(int);";
    sZAPI["DestroyWindow"] = "int (__stdcall *DestroyWindow)(HWND__ *);";
    sZAPI["KillTimer"] = "int (__stdcall *KillTimer)(HWND__ *, unsigned int);";
    sZAPI["SetTimer"] = "unsigned int (__stdcall *SetTimer)(HWND__ *, unsigned int, unsigned int, void (__stdcall *)(HWND__ *, unsigned int, unsigned int, unsigned int));";
    sZAPI["DispatchMessageA"] = "int (__stdcall *DispatchMessageA)(const tagMSG *);";
    sZAPI["TranslateMessage"] = "int (__stdcall *TranslateMessage)(const tagMSG *);";
    sZAPI["GetMessageA"] = "int (__stdcall *GetMessageA)(tagMSG *, HWND__ *, unsigned int, unsigned int);";
    sZAPI["UpdateWindow"] = "int (__stdcall *UpdateWindow)(HWND__ *);";
    sZAPI["ShowWindow"] = "int (__stdcall *ShowWindow)(HWND__ *, int);";
    sZAPI["CreateWindowExA"] = "HWND__ *(__stdcall *CreateWindowExA)(unsigned int, const char *, const char *, unsigned int, int, int, int, int, HWND__ *, HMENU__ *, HINSTANCE__ *, void *);";
    sZAPI["GetWindowRect"] = "int (__stdcall *GetWindowRect)(HWND__ *, tagRECT *);";
    sZAPI["GetDesktopWindow"] = "HWND__ *(__stdcall *GetDesktopWindow)();";
    sZAPI["GetSystemMetrics"] = "int (__stdcall *GetSystemMetrics)(int);";
    sZAPI["RegisterClassExA"] = "unsigned __int16 (__stdcall *RegisterClassExA)(const tagWNDCLASSEXA *);";
    sZAPI["OffsetRect"] = "int (__stdcall *OffsetRect)(tagRECT *, int, int);";
    sZAPI["SetWindowLongA"] = "int (__stdcall *SetWindowLongA)(HWND__ *, int, int);";
    sZAPI["RegisterClassA"] = "unsigned __int16 (__stdcall *RegisterClassA)(const tagWNDCLASSA *);";
    sZAPI["LoadCursorA"] = "HICON__ *(__stdcall *LoadCursorA)(HINSTANCE__ *, const char *);";
    sZAPI["SetFocus"] = "HWND__ *(__stdcall *SetFocus)(HWND__ *);";
    sZAPI["IsWindowVisible"] = "int (__stdcall *IsWindowVisible)(HWND__ *);";
    sZAPI["GetKeyState"] = "__int16 (__stdcall *GetKeyState)(int);";
    sZAPI["SendMessageA"] = "int (__stdcall *SendMessageA)(HWND__ *, unsigned int, unsigned int, int);";
    sZAPI["GetClientRect"] = "int (__stdcall *GetClientRect)(HWND__ *, tagRECT *);";
    sZAPI["GetWindowLongA"] = "int (__stdcall *GetWindowLongA)(HWND__ *, int);";
    sZAPI["EndPaint"] = "int (__stdcall *EndPaint)(HWND__ *, const tagPAINTSTRUCT *);";
    sZAPI["BeginPaint"] = "HDC__ *(__stdcall *BeginPaint)(HWND__ *, tagPAINTSTRUCT *);";
    sZAPI["PtInRect"] = "int (__stdcall *PtInRect)(const tagRECT *, tagPOINT);";
    sZAPI["CharPrevA"] = "char *(__stdcall *CharPrevA)(const char *, const char *);";
    sZAPI["CharNextA"] = "char *(__stdcall *CharNextA)(const char *);";
    sZAPI["InflateRect"] = "int (__stdcall *InflateRect)(tagRECT *, int, int);";
    sZAPI["GetAsyncKeyState"] = "__int16 (__stdcall *GetAsyncKeyState)(int);";
    sZAPI["GetDoubleClickTime"] = "unsigned int (__stdcall *GetDoubleClickTime)();";
    sZAPI["SystemParametersInfoA"] = "int (__stdcall *SystemParametersInfoA)(unsigned int, unsigned int, void *, unsigned int);";
    sZAPI["SetCursorPos"] = "int (__stdcall *SetCursorPos)(int, int);";
    sZAPI["ClientToScreen"] = "int (__stdcall *ClientToScreen)(HWND__ *, tagPOINT *);";
    sZAPI["IntersectRect"] = "int (__stdcall *IntersectRect)(tagRECT *, const tagRECT *, const tagRECT *);";
    sZAPI["IsRectEmpty"] = "int (__stdcall *IsRectEmpty)(const tagRECT *);";
    sZAPI["DialogBoxParamA"] = "int (__stdcall *DialogBoxParamA)(HINSTANCE__ *, const char *, HWND__ *, int (__stdcall *)(HWND__ *, unsigned int, unsigned int, int), int);";
    sZAPI["ScreenToClient"] = "int (__stdcall *ScreenToClient)(HWND__ *, tagPOINT *);";
    sZAPI["GetCursorPos"] = "int (__stdcall *GetCursorPos)(tagPOINT *);";
    sZAPI["SetCursor"] = "HICON__ *(__stdcall *SetCursor)(HICON__ *);";
    sZAPI["EndDialog"] = "int (__stdcall *EndDialog)(HWND__ *, int);";
    sZAPI["SendDlgItemMessageA"] = "int (__stdcall *SendDlgItemMessageA)(HWND__ *, int, unsigned int, unsigned int, int);";
    sZAPI["ReleaseDC"] = "int (__stdcall *ReleaseDC)(HWND__ *, HDC__ *);";
    sZAPI["SetWindowPos"] = "int (__stdcall *SetWindowPos)(HWND__ *, HWND__ *, int, int, int, int, unsigned int);";
    sZAPI["GetDlgItem"] = "HWND__ *(__stdcall *GetDlgItem)(HWND__ *, int);";
    sZAPI["DrawTextA"] = "int (__stdcall *DrawTextA)(HDC__ *, const char *, int, tagRECT *, unsigned int);";
    sZAPI["GetDC"] = "HDC__ *(__stdcall *GetDC)(HWND__ *);";
    sZAPI["MessageBeep"] = "int (__stdcall *MessageBeep)(unsigned int);";
    sZAPI["GetFocus"] = "HWND__ *(__stdcall *GetFocus)();";
    sZAPI["GetKeyboardLayout"] = "HKL__ *(__stdcall *GetKeyboardLayout)(unsigned int);";
    sZAPI["PeekMessageA"] = "int (__stdcall *PeekMessageA)(tagMSG *, HWND__ *, unsigned int, unsigned int, unsigned int);";
    sZAPI["MsgWaitForMultipleObjects"] = "unsigned int (__stdcall *MsgWaitForMultipleObjects)(unsigned int, void **, int, unsigned int, unsigned int);";
    sZAPI["UnregisterClassA"] = "int (__stdcall *UnregisterClassA)(const char *, HINSTANCE__ *);";
    sZAPI["LoadIconA"] = "HICON__ *(__stdcall *LoadIconA)(HINSTANCE__ *, const char *);";
    sZAPI["CloseClipboard"] = "int (__stdcall *CloseClipboard)();";
    sZAPI["SetClipboardData"] = "void *(__stdcall *SetClipboardData)(unsigned int, void *);";
    sZAPI["EmptyClipboard"] = "int (__stdcall *EmptyClipboard)();";
    sZAPI["OpenClipboard"] = "int (__stdcall *OpenClipboard)(HWND__ *);";
    sZAPI["GetClipboardData"] = "void *(__stdcall *GetClipboardData)(unsigned int);";
    sZAPI["IsClipboardFormatAvailable"] = "int (__stdcall *IsClipboardFormatAvailable)(unsigned int);";
    sZAPI["ValidateRect"] = "int (__stdcall *ValidateRect)(HWND__ *, const tagRECT *);";
    sZAPI["InSendMessage"] = "int (__stdcall *InSendMessage)();";
    sZAPI["GetWindowTextA"] = "int (__stdcall *GetWindowTextA)(HWND__ *, char *, int);";
    sZAPI["EnumThreadWindows"] = "int (__stdcall *EnumThreadWindows)(unsigned int, int (__stdcall *)(HWND__ *, int), int);";
    sZAPI["SetForegroundWindow"] = "int (__stdcall *SetForegroundWindow)(HWND__ *);";
    sZAPI["FindWindowA"] = "HWND__ *(__stdcall *FindWindowA)(const char *, const char *);";
    sZAPI["GetWindowTextLengthA"] = "int (__stdcall *GetWindowTextLengthA)(HWND__ *);";
    sZAPI["GetWindowTextLengthW"] = "int (__stdcall *GetWindowTextLengthW)(HWND__ *);";
    sZAPI["CopyRect"] = "int (__stdcall *CopyRect)(tagRECT *, const tagRECT *);";
    sZAPI["ShowCursor"] = "int (__stdcall *ShowCursor)(int);";
    sZAPI["IsWindow"] = "int (__stdcall *IsWindow)(HWND__ *);";
    sZAPI["GetForegroundWindow"] = "HWND__ *(__stdcall *GetForegroundWindow)();";
    sZAPI["EqualRect"] = "int (__stdcall *EqualRect)(const tagRECT *, const tagRECT *);";
    sZAPI["CallNextHookEx"] = "int (__stdcall *CallNextHookEx)(HHOOK__ *, int, unsigned int, int);";
    sZAPI["SetWindowsHookExA"] = "HHOOK__ *(__stdcall *SetWindowsHookExA)(int, int (__stdcall *)(int, unsigned int, int), HINSTANCE__ *, unsigned int);";
    sZAPI["UnhookWindowsHookEx"] = "int (__stdcall *UnhookWindowsHookEx)(HHOOK__ *);";
    sZAPI["CoTaskMemAlloc"] = "void *(__stdcall *CoTaskMemAlloc)(unsigned int);";
    sZAPI["CoTaskMemRealloc"] = "void *(__stdcall *CoTaskMemRealloc)(void *, unsigned int);";
    sZAPI["CoTaskMemFree"] = "void (__stdcall *CoTaskMemFree)(void *);";
    sZAPI["OleUninitialize"] = "void (__stdcall *OleUninitialize)();";
    sZAPI["OleInitialize"] = "HRESULT (__stdcall *OleInitialize)(void *);";
    sZAPI["CoCreateInstance"] = "HRESULT (__stdcall *CoCreateInstance)(const _GUID *, IUnknown *, unsigned int, const _GUID *, void **);";
    sZAPI["CoUninitialize"] = "void (__stdcall *CoUninitialize)();";
    sZAPI["CoInitialize"] = "HRESULT (__stdcall *CoInitialize)(void *);";
    sZAPI["GetStockObject"] = "void *(__stdcall *GetStockObject)(int);";
    sZAPI["SetTextColor"] = "unsigned int (__stdcall *SetTextColor)(HDC__ *, unsigned int);";
    sZAPI["DeleteObject"] = "int (__stdcall *DeleteObject)(void *);";
    sZAPI["SelectObject"] = "void *(__stdcall *SelectObject)(HDC__ *, void *);";
    sZAPI["CreateFontA"] = "HFONT__ *(__stdcall *CreateFontA)(int, int, int, int, int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, const char *);";
    sZAPI["SetBkMode"] = "int (__stdcall *SetBkMode)(HDC__ *, int);";
    sZAPI["DeleteDC"] = "int (__stdcall *DeleteDC)(HDC__ *);";
    sZAPI["TextOutA"] = "int (__stdcall *TextOutA)(HDC__ *, int, int, const char *, int);";
    sZAPI["GetTextExtentPoint32A"] = "int (__stdcall *GetTextExtentPoint32A)(HDC__ *, const char *, int, tagSIZE *);";
    sZAPI["GetTextExtentPoint32W"] = "int (__stdcall *GetTextExtentPoint32W)(HDC__ *, const char *, int, tagSIZE *);";
    sZAPI["CreateDIBSection"] = "HBITMAP__ *(__stdcall *CreateDIBSection)(HDC__ *, const tagBITMAPINFO *, unsigned int, void **, void *, unsigned int);";
    sZAPI["GetObjectA"] = "int (__stdcall *GetObjectA)(void *, int, void *);";
    sZAPI["GetTextMetricsA"] = "int (__stdcall *GetTextMetricsA)(HDC__ *, tagTEXTMETRICA *);";
    sZAPI["SetTextAlign"] = "unsigned int (__stdcall *SetTextAlign)(HDC__ *, unsigned int);";
    sZAPI["CreateCompatibleDC"] = "HDC__ *(__stdcall *CreateCompatibleDC)(HDC__ *);";
    sZAPI["CreateFontIndirectA"] = "HFONT__ *(__stdcall *CreateFontIndirectA)(const tagLOGFONTA *);";
    sZAPI["RegCreateKeyExA"] = "int (__stdcall *RegCreateKeyExA)(HKEY__ *, const char *, unsigned int, char *, unsigned int, unsigned int, _SECURITY_ATTRIBUTES *, HKEY__ **, unsigned int *);";
    sZAPI["RegCloseKey"] = "int (__stdcall *RegCloseKey)(HKEY__ *);";
    sZAPI["RegDeleteValueA"] = "int (__stdcall *RegDeleteValueA)(HKEY__ *, const char *);";
    sZAPI["RegEnumValueA"] = "int (__stdcall *RegEnumValueA)(HKEY__ *, unsigned int, char *, unsigned int *, unsigned int *, unsigned int *, unsigned __int8 *, unsigned int *);";
    sZAPI["RegQueryValueExA"] = "int (__stdcall *RegQueryValueExA)(HKEY__ *, const char *, unsigned int *, unsigned int *, unsigned __int8 *, unsigned int *);";
    sZAPI["RegSetValueExA"] = "int (__stdcall *RegSetValueExA)(HKEY__ *, const char *, unsigned int, unsigned int, const unsigned __int8 *, unsigned int);";
    sZAPI["RegQueryValueA"] = "int (__stdcall *RegQueryValueA)(HKEY__ *, const char *, char *, int *);";
    sZAPI["RegOpenKeyExA"] = "int (__stdcall *RegOpenKeyExA)(HKEY__ *, const char *, unsigned int, unsigned int, HKEY__ **);";
    sZAPI["SHGetSpecialFolderPathA"] = "int (__stdcall *SHGetSpecialFolderPathA)(HWND__ *, char *, int, int);";
    sZAPI["ImmGetProperty"] = "unsigned int (__stdcall *ImmGetProperty)(HKL__ *, unsigned int);";
    sZAPI["ImmAssociateContext"] = "HIMC__ *(__stdcall *ImmAssociateContext)(HWND__ *, HIMC__ *);";
    sZAPI["ImmCreateContext"] = "HIMC__ *(__stdcall *ImmCreateContext)();";
    sZAPI["ImmDestroyContext"] = "int (__stdcall *ImmDestroyContext)(HIMC__ *);";
    sZAPI["ImmGetCandidateWindow"] = "int (__stdcall *ImmGetCandidateWindow)(HIMC__ *, unsigned int, tagCANDIDATEFORM *);";
    sZAPI["ImmSetCandidateWindow"] = "int (__stdcall *ImmSetCandidateWindow)(HIMC__ *, tagCANDIDATEFORM *);";
    sZAPI["ImmReleaseContext"] = "int (__stdcall *ImmReleaseContext)(HWND__ *, HIMC__ *);";
    sZAPI["ImmSetCompositionWindow"] = "int (__stdcall *ImmSetCompositionWindow)(HIMC__ *, tagCOMPOSITIONFORM *);";
    sZAPI["ImmGetContext"] = "HIMC__ *(__stdcall *ImmGetContext)(HWND__ *);";
    sZAPI["ImmNotifyIME"] = "int (__stdcall *ImmNotifyIME)(HIMC__ *, unsigned int, unsigned int, unsigned int);";
    sZAPI["ImmGetCandidateListA"] = "unsigned int (__stdcall *ImmGetCandidateListA)(HIMC__ *, unsigned int, tagCANDIDATELIST *, unsigned int);";
    sZAPI["ImmGetCompositionStringA"] = "int (__stdcall *ImmGetCompositionStringA)(HIMC__ *, unsigned int, void *, unsigned int);";
    sZAPI["ImmGetConversionStatus"] = "int (__stdcall *ImmGetConversionStatus)(HIMC__ *, unsigned int *, unsigned int *);";
    sZAPI["ImmGetOpenStatus"] = "int (__stdcall *ImmGetOpenStatus)(HIMC__ *);";
    sZAPI["timeGetTime"] = "unsigned int (__stdcall *timeGetTime)();";
    sZAPI["WSAStartup"] = "int (__stdcall *WSAStartup)(unsigned __int16, void *);";
    sZAPI["WSACleanup"] = "int (__stdcall *WSACleanup)();";
    sZAPI["WSACreateEvent"] = "void *(__stdcall *WSACreateEvent)();";
    sZAPI["WSASetEvent"] = "int (__stdcall *WSASetEvent)(void *);";
    sZAPI["WSAResetEvent"] = "int (__stdcall *WSAResetEvent)(void *);";
    sZAPI["WSACloseEvent"] = "int (__stdcall *WSACloseEvent)(void *);";
    sZAPI["WSAEventSelect"] = "int (__stdcall *WSAEventSelect)(unsigned int, void *, int);";
    sZAPI["WSAAsyncSelect"] = "int (__stdcall *WSAAsyncSelect)(unsigned int, HWND__ *, unsigned int, int);";
    sZAPI["WSAWaitForMultipleEvents"] = "unsigned int (__stdcall *WSAWaitForMultipleEvents)(unsigned int, void *const *, int, unsigned int, int);";
    sZAPI["WSAEnumNetworkEvents"] = "int (__stdcall *WSAEnumNetworkEvents)(unsigned int, void *, void *);";
    sZAPI["WSASend"] = "int (__stdcall *WSASend)(unsigned int, void *, unsigned int, unsigned int *, unsigned int, void *, void *);";
    sZAPI["WSARecv"] = "int (__stdcall *WSARecv)(unsigned int, void *, unsigned int, unsigned int *, unsigned int *, void *, void *);";
    sZAPI["WSAGetLastError"] = "int (__stdcall *WSAGetLastError)();";
    sZAPI["WSASetLastError"] = "void (__stdcall *WSASetLastError)(int);";
    sZAPI["socket"] = "int (__stdcall *socket)(int, int, int);";
    sZAPI["connect"] = "int (__stdcall *connect)(unsigned int, const void *, int);";
    sZAPI["bind"] = "int (__stdcall *bind)(unsigned int, const void *, int);";
    sZAPI["listen"] = "int (__stdcall *listen)(unsigned int, int);";
    sZAPI["accept"] = "int (__stdcall *accept)(unsigned int, const void *, int *);";
    sZAPI["ioctlsocket"] = "int (__stdcall *ioctlsocket)(unsigned int, int, unsigned int *);";
    sZAPI["closesocket"] = "int (__stdcall *closesocket)(unsigned int);";
    sZAPI["getsockopt"] = "int (__stdcall *getsockopt)(unsigned int, int, int, char *, int *);";
    sZAPI["setsockopt"] = "int (__stdcall *setsockopt)(unsigned int, int, int, const char *, int);";
    sZAPI["send"] = "int (__stdcall *send)(unsigned int, const char *, int, int);";
    sZAPI["sendto"] = "int (__stdcall *sendto)(unsigned int, const char *, int, int, const void *, int);";
    sZAPI["recv"] = "int (__stdcall *recv)(unsigned int, const char *, int, int);";
    sZAPI["recvfrom"] = "int (__stdcall *recvfrom)(unsigned int, char *, int, int, const void *, int *);";

    std::ofstream outFile("output.txt", std::ios::out | std::ios::trunc);
    outFile << "struct _ZAPIProcAddress\n{\n";
        if (!outFile) {
        // Handle error opening file
        DEBUG_MESSAGE("Failed to open output.txt for writing\n");
        return;
    }

    static auto s_zapiloader = reinterpret_cast<void*>(0x00670D58);
    static auto ZAPILoader__DecodePackage = reinterpret_cast<unsigned int (__thiscall*)(void*, char*, unsigned int*, char*)>(0x00516F8F);
    static auto s_aAPIInfo = reinterpret_cast<char*>(0x006685A0);

    // ZAPILoader::InitDecoderInfo
    *reinterpret_cast<unsigned __int64*>(reinterpret_cast<uintptr_t>(s_zapiloader) + 0xA0) = 0;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(s_zapiloader) + 0xB0) = 0;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(s_zapiloader) + 0xB4) = 1;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(s_zapiloader) + 0xB8) = 1;

    unsigned int uPibot = 0;
    char sBuffer[256];
    while (uPibot < 0xE7D) {
        memset(sBuffer, 0, sizeof(sBuffer));
        unsigned int nIdx = ZAPILoader__DecodePackage(s_zapiloader, s_aAPIInfo, &uPibot, sBuffer);
        DEBUG_MESSAGE("%s", sBuffer);
        if (auto search = sZAPI.find(std::string(sBuffer)); search != sZAPI.end()) {
            outFile << "  " << search->second << "\n";
        } else {
            outFile << "  " << sBuffer << "\n";
        }
    }
    outFile << "};";
    outFile.close();


    // CChatHelper::TryChat
    Patch1(0x004AA7EF, 0xEB); // bypass chat cooldown
    Patch1(0x004AA74A, 0xEB); // bypass chat repeat

    // CVecCtrl::IsFalling
    PatchRetZero(0x0093A140); // double jump while falling

    // CDropPool::TryPickUpDropByPet
    PatchJmp(CDropPool__TryPickUpDropByPet_jmp, reinterpret_cast<uintptr_t>(&CDropPool__TryPickUpDropByPet_hook)); // increase pet pickup range

    ATTACH_HOOK(CCurseProcess__ProcessString, CCurseProcess__ProcessString_hook); // disable profanity filter
    ATTACH_HOOK(CInputSystem__DetectJoystick, CInputSystem__DetectJoystick_hook); // fix stutter
    ATTACH_HOOK(CVecCtrl__SetImpactNext, CVecCtrl__SetImpactNext_hook); // vertical double jump
    ATTACH_HOOK(CUserLocal__Jump, CUserLocal__Jump_hook); // double jump with jump key
    ATTACH_HOOK(get_weapon_attack_speed, get_weapon_attack_speed_hook); // append attack speed value to weapon speed string

#ifdef _DEBUG
    // Add ID to map name, item description, skill description, quest info
    ATTACH_HOOK(CItemInfo__GetMapString, CItemInfo__GetMapString_hook);
    ATTACH_HOOK(CItemInfo__GetItemDesc, CItemInfo__GetItemDesc_hook);
    ATTACH_HOOK(CSkillInfo__LoadSkill, CSkillInfo__LoadSkill_hook);
    PatchJmp(CUIQuestInfoDetail__Draw_jmp, reinterpret_cast<uintptr_t>(&CUIQuestInfoDetail__Draw_hook)); // replace "Low Level Quest"
#endif
}