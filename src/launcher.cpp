#include "pch.h"
#include "config.h"
#include "debug.h"


int WINAPI PatchExecutionLevel() {
    if (!CopyFileA("MapleStory.exe", "MapleStory.exe.backup", false)) {
        ErrorMessage("Error creating backup for MapleStory.exe");
        return 1;
    }
    HANDLE hFile = CreateFileA("MapleStory.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        ErrorMessage("Error opening MapleStory.exe [%d]", GetLastError());
        return 1;
    }
    LPCSTR sOriginal = "level=\"requireAdministrator\"";
    LPCSTR sPatch    = "level=\"asInvoker\"           ";
    DWORD dwPatchSize = strlen(sPatch);
    LARGE_INTEGER liOffset;
    liOffset.QuadPart = 0x005594FD;
    if (!SetFilePointerEx(hFile, liOffset, NULL, FILE_BEGIN) ||
            !WriteFile(hFile, sPatch, dwPatchSize, NULL, NULL)) {
        ErrorMessage("Error patching MapleStory.exe [%d]", GetLastError());
        CloseHandle(hFile);
        return 1;
    }
    CloseHandle(hFile);
    ErrorMessage("Patching execution level complete");
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Parse arguments
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv == nullptr) {
        ErrorMessage("Error parsing command line arguments \"%s\"", lpCmdLine);
        return 1;
    }
    for (int i = 0; i < argc; ++i) {
        if (!wcscmp(argv[i], L"--patch-execution-level")) {
            return PatchExecutionLevel();
        }
    }
    LocalFree(argv);

    // Start process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    if (!DetourCreateProcessWithDllExA("MapleStory.exe", lpCmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi, CONFIG_DLL_NAME, NULL)) {
        ErrorMessage("Could not start MapleStory.exe [%d]", GetLastError());
        return 1;
    }
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwExitCode;
    if (!GetExitCodeProcess(pi.hProcess, &dwExitCode)) {
        ErrorMessage("GetExitCodeProcess failed [%d]", GetLastError());
        return 1;
    }
    return dwExitCode;
}