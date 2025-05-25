#pragma once

#ifdef _DEBUG
    #define DEBUG_MESSAGE(FORMAT, ...) DebugMessage(FORMAT, __VA_ARGS__)
    #define DEBUG_MESSAGE_W(FORMAT, ...) DebugMessageW(FORMAT, __VA_ARGS__)
#else
    #define DEBUG_MESSAGE(FORMAT, ...)
    #define DEBUG_MESSAGE_W(FORMAT, ...)
#endif


void DebugMessage(const char* pszFormat, ...);

void DebugMessageW(const wchar_t* pszFormat, ...);

void ErrorMessage(const char* pszFormat, ...);