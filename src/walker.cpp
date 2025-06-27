#include "pch.h"
#include "hook.h"
#include "debug.h"
#include <windows.h>
#include <pathcch.h>
#include <cstdio>
#include <ctime>
#include <StackWalker.h>

#define EH_EXCEPTION_NUMBER 0xE06D7363


class StackTraceWalker : public StackWalker {
public:
    FILE* m_file;

    StackTraceWalker(FILE* file) : m_file(file) {
    }

protected:
    virtual void OnOutput(LPCSTR szText) {
        fprintf_s(m_file, szText);
    }
};


bool TryOpenFile(FILE** pFile, const wchar_t* sFileName) {
    for (size_t i = 0; i < 10; ++i) {
        if (!_wfopen_s(pFile, sFileName, L"a")) {
            return true;
        }
        Sleep(100);
    }
    return false;
}


void LogStackTrace(PCONTEXT pContextRecord) {
    wchar_t sPath[MAX_PATH];
    GetModuleFileNameW(nullptr, sPath, MAX_PATH);
    PathCchRemoveFileSpec(sPath, MAX_PATH);
    PathCchAppend(sPath, MAX_PATH, L"log");
    CreateDirectoryW(sPath, nullptr);

    time_t now = time(nullptr);
    struct tm localtime;
    localtime_s(&localtime, &now);
    wchar_t sFileName[MAX_PATH];
    wcsftime(sFileName, MAX_PATH, L"%Y-%m-%d.txt", &localtime);
    PathCchAppend(sPath, MAX_PATH, sFileName);

    FILE* file;
    if (!TryOpenFile(&file, sPath)) {
        DebugMessage("Failed to open file %ls", sPath);
        return;
    }

    char sTimeInfo[256];
    strftime(sTimeInfo, MAX_PATH, "%Y-%m-%d %H:%M:%S", &localtime);
    fprintf_s(file, "================================================================================\n");
    fprintf_s(file, "Stack Trace : %s\n", sTimeInfo);
    fprintf_s(file, "================================================================================\n");

    StackTraceWalker walker(file);
    walker.SetSymPath("MapleStory.pdb");
    walker.ShowCallstack(GetCurrentThread(), pContextRecord);
    fclose(file);
}


LONG NTAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
    DWORD dwExceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
    DWORD dwExceptionAddress = reinterpret_cast<DWORD>(pExceptionInfo->ExceptionRecord->ExceptionAddress);

    switch (dwExceptionCode) {
    case STATUS_PRIVILEGED_INSTRUCTION:
    case DBG_PRINTEXCEPTION_C:
    case DBG_PRINTEXCEPTION_WIDE_C:
        break;
    case STATUS_ACCESS_VIOLATION:
        DebugMessage("Status Access Violation : 0x%08X : 0x%08X, flags : 0x%08X", dwExceptionCode, dwExceptionAddress, pExceptionInfo->ExceptionRecord->ExceptionFlags);
        LogStackTrace(pExceptionInfo->ContextRecord);
        break;
    case EH_EXCEPTION_NUMBER:
        break;
    default:
        DebugMessage("Exception Code 0x%08X : 0x%08X", dwExceptionCode, dwExceptionAddress);
        break;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


void AttachExceptionHandler() {
    AddVectoredExceptionHandler(1, VectoredExceptionHandler);
}