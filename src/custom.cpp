#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


void OnRegisterMigrationResult(CInPacket* iPacket) {
    DEBUG_MESSAGE("OnRegisterMigrationResult");
    if (iPacket->Decode1()) {
        ErrorMessage("Failed to register migration.");
        return;
    }
    // Get channel host and port
    unsigned int host = iPacket->Decode4();
    unsigned short port = iPacket->Decode2();

    // Get client key and set in CWvsContext
    unsigned char aClientKey[8];
    iPacket->DecodeBuffer(aClientKey, 8);
    memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(CWvsContext::GetInstance()) + 0x20A8), aClientKey, 8);

    // Connect to channel host/port
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = host;
    // CWvsContext::IssueConnect(CWvsContext::GetInstance(), addr);
    reinterpret_cast<void (__thiscall*)(CWvsContext*, sockaddr_in*)>(0x009E0300)(CWvsContext::GetInstance(), &addr);
}


static auto CClientSocket__ProcessPacket = reinterpret_cast<void (__thiscall*)(CClientSocket*, CInPacket*)>(0x004B00F0);

void __fastcall CClientSocket__ProcessPacket_hook(CClientSocket* pThis, void* _EDX, CInPacket* iPacket) {
    unsigned int uOffset = iPacket->m_uOffset();
    unsigned short op = iPacket->Decode2();
    switch (op) {
        case 1000:
            OnRegisterMigrationResult(iPacket);
            return;
    }
    iPacket->m_uOffset() = uOffset;
    CClientSocket__ProcessPacket(pThis, iPacket);
}


void AttachCustomPacket() {
    ATTACH_HOOK(CClientSocket__ProcessPacket, CClientSocket__ProcessPacket_hook);
}