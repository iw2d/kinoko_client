#pragma once
#include <windows.h>
#include <winsock2.h>
#include <cstdint>


class ZInetAddr : public sockaddr_in {
public:
    operator const struct sockaddr *() const {
        return (const struct sockaddr*)this;
    }
    operator const struct sockaddr_in *() const {
        return (const struct sockaddr_in*)this;
    }
    ZInetAddr() {
        _Init(0, 0);
    }
    ZInetAddr& operator=(const ZInetAddr& r) {
        if (this != &r) {
            *static_cast<sockaddr_in*>(this) = static_cast<const sockaddr_in&>(r);
        }
        return *this;
    }

private:
    void _Init(uint32_t uPort, uint32_t uAddr) {
        memset(this, 0, sizeof(ZInetAddr));
        sin_addr.S_un.S_addr = htonl(uAddr);
        sin_port = htons(uPort);
        sin_family = AF_INET;
    }
};

static_assert(sizeof(ZInetAddr) == sizeof(sockaddr_in));


class ZSocketBase {
private:
    SOCKET _m_hSocket;

public:
    ZSocketBase() {
        _m_hSocket = INVALID_SOCKET;
    }
    ~ZSocketBase() {
        if (_m_hSocket != INVALID_SOCKET) {
            closesocket(_m_hSocket);
            _m_hSocket = INVALID_SOCKET;
        }
    }
    operator SOCKET() const {
        return _m_hSocket;
    }
};

static_assert(sizeof(ZSocketBase) == 0x4);


class ZSocketBuffer;
