#include "pch.h"
#include "client.h"
#include "hook.h"
#include "config.h"

#define REPLACE_STRING(INDEX, NEW_STRING) \
    do { \
        static char sEncoded[GetLength(NEW_STRING) + 2]; \
        EncodeString(INDEX, NEW_STRING, sEncoded); \
    } while (0)


class StringPool {
public:
    class Key {
    public:
        ZArray<unsigned char> m_aKey;
    };
    static_assert(sizeof(Key) == 0x4);
};

typedef void (__thiscall* StringPool__Key__ctor_t)(StringPool::Key*, const unsigned char*, unsigned int, unsigned int);
static auto StringPool__Key__ctor = reinterpret_cast<StringPool__Key__ctor_t>(0x00746470);

static auto StringPool__ms_aKey = reinterpret_cast<const unsigned char*>(0x00B98830);
static auto StringPool__ms_aString = reinterpret_cast<const char**>(0x00C5A878);

constexpr size_t GetLength(const char* s) {
    size_t n = 0;
    while (s[n]) {
        ++n;
    }
    return n;
}

void EncodeString(int nIdx, const char* sSource, char* sDestination) {
    StringPool::Key keygen;
    StringPool__Key__ctor(&keygen, StringPool__ms_aKey, 0x10, 0);
    size_t n = strlen(sSource);
    for (size_t i = 0; i < n; ++i) {
        unsigned char key = keygen.m_aKey.a[i % 0x10];
        sDestination[i + 1] = sSource[i] ^ key;
        if (sSource[i] == key) {
            sDestination[i + 1] = key;
        }
    }
    sDestination[0] = 0;
    sDestination[n + 1] = 0;
    StringPool__ms_aString[nIdx] = sDestination;
}


void AttachStringPoolMod() {
    REPLACE_STRING(2585, CONFIG_REGISTRY_KEY);
    REPLACE_STRING(2948, CONFIG_VERSION_STRING);
}