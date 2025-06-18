#include "config.h"
#include "debug.h"
#include <windows.h>
#include <detours.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
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
    return 0;
}