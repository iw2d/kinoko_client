#pragma once
#include <cstdint>


struct CRTTI {
    const CRTTI* m_pPrev;

    CRTTI(const CRTTI* pPrev) : m_pPrev(pPrev) {
    }
    int32_t IsKindOf(const CRTTI* pRTTI) const {
        CRTTI result = {this};
        while (result.m_pPrev != pRTTI) {
            result.m_pPrev = result.m_pPrev->m_pPrev;
            if (!result.m_pPrev) {
                return 0;
            }
        }
        return 1;
    }
};