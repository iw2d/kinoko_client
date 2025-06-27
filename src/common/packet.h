#pragma once
#include "ztl/zalloc.h"
#include "ztl/zcoll.h"
#include "ztl/zstr.h"
#include <cstdint>


class CInPacket {
protected:
    int32_t m_bLoopback;
    int32_t m_nState;
    ZArray<uint8_t> m_aRecvBuff;
    uint16_t m_uLength;
    uint16_t m_uRawSeq;
    uint16_t m_uDataLen;
    uint32_t m_uOffset;
};

static_assert(sizeof(CInPacket) == 0x18);


class COutPacket {
protected:
    int32_t m_bLoopback;
    ZArray<uint8_t> m_aSendBuff;
    uint32_t m_uOffset;
    int32_t m_bIsEncryptedByShanda;

public:
    explicit COutPacket(int32_t nType) : m_aSendBuff(0x100) {
        Init(nType, 0, 0);
    }
    void Encode1(uint8_t n) {
        EncodeBuffer(&n, 1);
    }
    void Encode2(uint16_t n) {
        EncodeBuffer(&n, 2);
    }
    void Encode4(uint32_t n) {
        EncodeBuffer(&n, 4);
    }
    void EncodeStr(ZXString<char> s) {
        int32_t n = s.GetLength();
        Encode2(n);
        EncodeBuffer(s, n);
    }
    void EncodeBuffer(const void* p, uint32_t uSize) {
        EnlargeBuffer(uSize);
        memcpy(&m_aSendBuff[m_uOffset], p, uSize);
        m_uOffset += uSize;
    }
    void Init(int32_t nType, int32_t bLoopback, int32_t bTypeHeader1Byte) {
        m_bLoopback = bLoopback;
        m_uOffset = 0;
        if (nType != 0x7FFFFFFF) {
            if (bTypeHeader1Byte) {
                Encode1(nType);
            } else {
                Encode2(nType);
            }
        }
        m_bIsEncryptedByShanda = 0;
    }

protected:
    void EnlargeBuffer(uint32_t uSize) {
        uint32_t uCur = m_aSendBuff.GetCount();
        uint32_t uReq = m_uOffset + uSize;
        if (uCur < uReq) {
            do {
                uCur *= 2;
            } while (uCur < uReq);
            m_aSendBuff.Realloc(uCur, 0);
        }
    }
};

static_assert(sizeof(COutPacket) == 0x10);