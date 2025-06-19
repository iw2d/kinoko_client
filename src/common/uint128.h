#pragma once
#include <cstdint>

class UINT128 {
private:
    uint32_t m_data[4];

public:
    UINT128() {
        setValue(0);
    }
    uint32_t getBitNumber(uint32_t bit) {
        if (bit <= 0x7F) {
            return (m_data[bit >> 5] >> (31 - (bit & 0x1F))) & 1;
        } else {
            return 0;
        }
    }
    UINT128& setBitNumber(uint32_t bit, uint32_t value) {
        uint32_t mask = 1 << (31 - (bit & 0x1F));
        m_data[bit >> 5] |= mask;
        if (!value) {
            m_data[bit >> 5] ^= mask;
        }
        return *this;
    }
    UINT128& setValue(uint32_t value) {
        m_data[0] = 0;
        m_data[1] = 0;
        m_data[2] = 0;
        m_data[3] = value;
        return *this;
    }
    UINT128& setValue(const UINT128& value) {
        *this = value;
        return *this;
    }
};
static_assert(sizeof(UINT128) == 0x10);