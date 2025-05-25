#include "pch.h"
#include "debug.h"


void DebugMessage(const char* pszFormat, ...) {
    char pszDest[1024];
    size_t cbDest = sizeof(pszDest);
    va_list argList;
    va_start(argList, pszFormat);
    StringCbVPrintfA(pszDest, cbDest, pszFormat, argList);
    OutputDebugStringA(pszDest);
    va_end(argList);
}

void DebugMessageW(const wchar_t* pszFormat, ...) {
    wchar_t pszDest[1024];
    size_t cbDest = sizeof(pszDest);
    va_list argList;
    va_start(argList, pszFormat);
    StringCbVPrintfW(pszDest, cbDest, pszFormat, argList);
    OutputDebugStringW(pszDest);
    va_end(argList);
}

void ErrorMessage(const char* pszFormat, ...) {
    char pszDest[1024];
    size_t cbDest = 1024 * sizeof(char);
    va_list argList;
    va_start(argList, pszFormat);
    StringCbVPrintfA(pszDest, cbDest, pszFormat, argList);
    MessageBox(nullptr, pszDest, "Error", MB_ICONERROR);
    va_end(argList);
}