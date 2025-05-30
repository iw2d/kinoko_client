#include <stdio.h>
#include <time.h>
#include <StackWalker.h>

#include "hook.h"
#include "config.h"
#include "debug.h"

#pragma warning(disable : 4996)


class DebugWalker : public StackWalker {
public:
    DebugWalker(LPCSTR szSymPath, FILE* file) {
        StackWalker::StackWalker(StackWalkOptions::OptionsAll, szSymPath);
        this->file = file;
    }
protected:
    virtual void OnOutput(LPCSTR szText) {
        fprintf_s(this->file, szText);
    }
private:
    FILE* file;
};


class ZException {
public:
    HRESULT m_hr;

    ZException(HRESULT hr) {
        this->m_hr = hr;
    }
};


struct EHExceptionRecord {
    unsigned int magicNumber;
    void* pExceptionObject;
    _ThrowInfo* pThrowInfo;
};


VOID WINAPI LogStackTrace() {
    // Open log file
    FILE* file;
    fopen_s(&file, CONFIG_LOG_FILE, "a+");
    // Log time
    time_t rawTime;
    time(&rawTime);
    struct tm * timeInfo = localtime(&rawTime);
    fprintf_s(file, "================================================================================\n");
    fprintf_s(file, "Stack Trace : %s", asctime(timeInfo));
    fprintf_s(file, "================================================================================\n");
    // Initialize StackWalker
    DebugWalker walker("MapleStory.pdb", file);
    walker.ShowCallstack(GetCurrentThread());
    fclose(file);
}


VOID WINAPI LogException(PCONTEXT pContextRecord) {
    // Open log file
    FILE* file;
    fopen_s(&file, CONFIG_LOG_FILE, "a+");
    // Log time
    time_t rawTime;
    time(&rawTime);
    struct tm * timeInfo = localtime(&rawTime);
    fprintf_s(file, "================================================================================\n");
    fprintf_s(file, "Exception : %s", asctime(timeInfo));
    fprintf_s(file, "================================================================================\n");
    // Initialize StackWalker
    DebugWalker walker("MapleStory.pdb", file);
    walker.ShowCallstack(GetCurrentThread(), pContextRecord);
    fclose(file);
}


LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
    DWORD dwExceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
    DWORD dwExceptionAddress = reinterpret_cast<DWORD>(pExceptionInfo->ExceptionRecord->ExceptionAddress);

    if (dwExceptionCode != STATUS_PRIVILEGED_INSTRUCTION && dwExceptionCode != DBG_PRINTEXCEPTION_C) {
        if (dwExceptionCode == STATUS_ACCESS_VIOLATION) {
            DebugMessage("Status Access Violation : 0x%08X - 0x%08X", dwExceptionAddress, pExceptionInfo->ExceptionRecord->ExceptionFlags);
            LogException(pExceptionInfo->ContextRecord);
        } else if (dwExceptionCode == 0xE06D7363) {
            // CallCatchBlock
            EHExceptionRecord* pExcept = reinterpret_cast<EHExceptionRecord*>(pExceptionInfo->ExceptionRecord->ExceptionInformation);
            if (pExcept->magicNumber == 0x19930520 || pExcept->magicNumber == 0x19930521 || pExcept->magicNumber == 0x19930522) {
                ZException* pExceptionObject = reinterpret_cast<ZException*>(pExcept->pExceptionObject);
                DebugMessage("EHException : %s - 0x%08X", pExcept->pThrowInfo->pCatchableTypeArray->arrayOfCatchableTypes[0]->pType->name, pExceptionObject->m_hr);
            }
        } else {
            DebugMessage("Exception Code 0x%08X : 0x%08X", dwExceptionCode, dwExceptionAddress);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}


void AttachExceptionHandler() {
#ifdef _DEBUG
    AddVectoredExceptionHandler(1, VectoredExceptionHandler);
#endif
}