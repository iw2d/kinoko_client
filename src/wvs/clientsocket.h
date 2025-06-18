#pragma once
#include "ztl/tsingleton.h"
#include "ztl/zlock.h"
#include "ztl/zcoll.h"
#include "ztl/zsock.h"

#include <windows.h>
#include <cstdint>


class CClientSocket : public TSingleton<CClientSocket, 0x00C64064> {
public:
    struct CONNECTCONTEXT {
        ZList<ZInetAddr> lAddr;
        ZInetAddr* posList;
        int32_t bLogin;
    };
    static_assert(sizeof(CONNECTCONTEXT) == 0x1C);

    MEMBER_AT(HWND, 0x4, m_hWnd)
    MEMBER_AT(ZSocketBase, 0x8, m_sock)
    MEMBER_AT(CONNECTCONTEXT, 0xC, m_ctxConnect)
    MEMBER_AT(ZList<ZRef<ZSocketBuffer>>, 0x50, m_lpSendBuff)
    MEMBER_AT(ZFatalSection, 0x7C, m_lockSend)
    MEMBER_AT(uint32_t, 0x84, m_uSeqSnd)

    __forceinline void SendPacket(const COutPacket& oPacket) {
        ZSynchronizedHelper<ZFatalSection> sync(&m_lockSend());
        if (m_sock() && m_sock() != INVALID_SOCKET && !m_ctxConnect().lAddr.GetCount()) {
            // COutPacket::MakeBufferList(oPacket, &m_lpSendBuff, 95, &m_uSeqSnd, 1, m_uSeqSnd);
            reinterpret_cast<void(__thiscall*)(COutPacket*, ZList<ZRef<ZSocketBuffer>>*, uint16_t, uint32_t*, int32_t, uint32_t)>(0x0068D100)(const_cast<COutPacket*>(&oPacket), &m_lpSendBuff(), 95, &m_uSeqSnd(), 1, m_uSeqSnd());
            // m_uSeqSnd = CIGCipher::innoHash((unsigned __int8 *)&m_uSeqSnd, 4, 0);
            m_uSeqSnd() = reinterpret_cast<unsigned int(__cdecl*)(unsigned char*, int, unsigned int*)>(0x00A1BF30)(reinterpret_cast<unsigned char*>(&m_uSeqSnd()), 4, nullptr);
            // CClientSocket::Flush(this);
            reinterpret_cast<void(__thiscall*)(CClientSocket*)>(0x004AF6A0)(this);
        }
    }
};