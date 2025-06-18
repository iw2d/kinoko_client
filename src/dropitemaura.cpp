#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


class CAnimationDisplayer : public TSingleton<CAnimationDisplayer, 0x00C68750> {
public:
    void RegisterOneTimeAnimation(IWzGr2DLayerPtr pLayer, int tDelayBeforeStart, IWzGr2DLayerPtr pFlipOrigin) {
        reinterpret_cast<void (__thiscall*)(CAnimationDisplayer*, IWzGr2DLayerPtr, int, IWzGr2DLayerPtr)>(0x00444410)(this, pLayer, tDelayBeforeStart, pFlipOrigin);
    }

    inline static auto LoadLayer = reinterpret_cast<void (__cdecl*)(
        IWzGr2DLayerPtr* result,
        IWzPropertyPtr pProp,
        int bFlip,
        IWzVector2DPtr pOrigin,
        int rx,
        int ry,
        IWzGr2DLayerPtr pOverlay,
        int z,
        int alpha,
        int magLevel
    )>(0x0044A8F0);
};


struct DROP {
    MEMBER_AT(IWzGr2DLayerPtr, 0x38, pLayer)
    MEMBER_AT(IWzGr2DLayerPtr, 0x90, pLayerFront)
    MEMBER_AT(IWzGr2DLayerPtr, 0x94, pLayerBack)
};

static auto DROP__ctor = reinterpret_cast<void (__thiscall*)(DROP*)>(0x00511620);

void __fastcall DROP__ctor_hook(DROP* pThis, void* _EDX) {
    DROP__ctor(pThis);
    pThis->pLayerFront().Detach();
    pThis->pLayerBack().Detach();
}

static auto DROP__dtor = reinterpret_cast<void (__thiscall*)(DROP*)>(0x00510D40);

void __fastcall DROP__dtor_hook(DROP* pThis, void* _EDX) {
    DROP__dtor(pThis);
    pThis->pLayerFront().~IWzGr2DLayerPtr();
    pThis->pLayerBack().~IWzGr2DLayerPtr();
}


static const wchar_t* g_asAuraType[4] = {
    L"Rare",
    L"Epic",
    L"Unique",
    L"Legendary"
};
static IWzPropertyPtr g_pEffect = nullptr;
static IWzPropertyPtr g_pFront[4] = {};
static IWzPropertyPtr g_pBack[4] = {};
static bool g_bDropItemAuraLoaded = false;

void LoadDropItemAura() {
    if (g_bDropItemAuraLoaded) {
        return;
    }
    g_bDropItemAuraLoaded = true;

    Ztl_variant_t vEffectProperty;
    CHECK_HRESULT(get_rm()->raw_GetObject(L"Effect/BasicEff.img/dropItemEffect", Ztl_variant_t(), Ztl_variant_t(), &vEffectProperty));
    g_pEffect = vEffectProperty.GetUnknown(false, false);

    for (size_t i = 0; i < 4; ++i) {
        wchar_t sUOL[256];
        swprintf_s(sUOL, 256, L"Effect/BasicEff.img/dropItemAura/%ls", g_asAuraType[i]);

        Ztl_variant_t vAuraProperty;
        CHECK_HRESULT(get_rm()->raw_GetObject(sUOL, Ztl_variant_t(), Ztl_variant_t(), &vAuraProperty));
        IWzPropertyPtr pAuraProperty(vAuraProperty.GetUnknown(false, false));

        Ztl_variant_t vFrontProperty;
        Ztl_variant_t vBackProperty;
        CHECK_HRESULT(pAuraProperty->get_item(L"front", &vFrontProperty));
        CHECK_HRESULT(pAuraProperty->get_item(L"back", &vBackProperty));
        g_pFront[i] = vFrontProperty.GetUnknown(false, false);
        g_pBack[i] = vBackProperty.GetUnknown(false, false);
    }
}


static auto CDropPool__OnDropEnterField_jmp = 0x00517331;
static auto CDropPool__OnDropEnterField_ret = 0x00517336;

unsigned char __stdcall CDropPool__OnDropEnterField_helper(CInPacket* iPacket, DROP* pDrop) {
    int nIndex;
    unsigned char nItemGrade = iPacket->Decode1();
    switch (nItemGrade) {
        case 1:
            nIndex = 0;
            break;
        case 2:
            nIndex = 1;
            break;
        case 3:
            nIndex = 2;
            break;
        case 4:
            nIndex = 3; // note: no legendary grade in v95
            break;
        default:
            nIndex = -1;
            break;
    }
    if (nIndex >= 0) {
        LoadDropItemAura();
        Ztl_variant_t vOrigin;
        CHECK_HRESULT(pDrop->pLayer()->get_origin(&vOrigin));
        IWzVector2DPtr pOrigin(vOrigin.GetUnknown(false, false));
        IWzGr2DLayerPtr pOverlay;
        CAnimationDisplayer::LoadLayer(std::addressof(pDrop->pLayerFront()), g_pFront[nIndex], 0, pOrigin, 0, 0, pOverlay, 1, 0, 0);
        int nZ;
        CHECK_HRESULT(pDrop->pLayer()->get_z(&nZ));
        CAnimationDisplayer::LoadLayer(std::addressof(pDrop->pLayerBack()), g_pBack[nIndex], 0, pOrigin, 0, 0, pOverlay, nZ - 1, 0, 0);
    }
    return iPacket->Decode1(); // overwritten Decode1
}

void __declspec(naked) CDropPool__OnDropEnterField_hook() {
    __asm {
        push    esi     ; DROP*
        push    ecx     ; CInPacket*
        call    CDropPool__OnDropEnterField_helper
        jmp     [ CDropPool__OnDropEnterField_ret ]
    };
}


static auto CDropPool__Update_jmp = 0x005162D8; // case 3: (DropState_Dropped)
static auto CDropPool__Update_ret = 0x005162DE;

void __stdcall CDropPool__Update_helper(DROP* pDrop) {
    if (!pDrop->pLayerFront() || !pDrop->pLayerBack()) {
        return;
    }
    int nAnimationState;
    pDrop->pLayerFront()->get_animationState(&nAnimationState);
    if (nAnimationState) {
        return;
    }
    IWzVector2DPtr pAlphaFront;
    CHECK_HRESULT(pDrop->pLayerFront()->get_alpha(&pAlphaFront));
    CHECK_HRESULT(pAlphaFront->raw_RelMove(255, 255, Ztl_variant_t(), Ztl_variant_t()));
    CHECK_HRESULT(pDrop->pLayerFront()->raw_Animate(GA_REPEAT, Ztl_variant_t(), Ztl_variant_t()));
    IWzVector2DPtr pAlphaBack;
    CHECK_HRESULT(pDrop->pLayerBack()->get_alpha(&pAlphaBack));
    CHECK_HRESULT(pAlphaBack->raw_RelMove(255, 255, Ztl_variant_t(), Ztl_variant_t()));
    CHECK_HRESULT(pDrop->pLayerBack()->raw_Animate(GA_REPEAT, Ztl_variant_t(), Ztl_variant_t()));

    Ztl_variant_t vOrigin;
    CHECK_HRESULT(pDrop->pLayer()->get_origin(&vOrigin));
    IWzVector2DPtr pOrigin(vOrigin.GetUnknown(false, false));
    int nZ;
    CHECK_HRESULT(pDrop->pLayer()->get_z(&nZ));
    IWzGr2DLayerPtr pEffectLayer;
    CAnimationDisplayer::LoadLayer(std::addressof(pEffectLayer), g_pEffect, 0, pOrigin, 0, 0, pDrop->pLayer(), nZ - 2, 255, 0);
    if (pEffectLayer) {
        CHECK_HRESULT(pEffectLayer->raw_Animate(GA_NORMAL, Ztl_variant_t(), Ztl_variant_t()));
        CAnimationDisplayer::GetInstance()->RegisterOneTimeAnimation(pEffectLayer, 0, IWzGr2DLayerPtr());
    }
}

void __declspec(naked) CDropPool__Update_hook() {
    __asm {
        push    esi     ; DROP*
        call    CDropPool__Update_helper
        fldpi
        jmp     [ CDropPool__Update_ret ]
    };
}


void AttachDropItemAuraMod() {
    ATTACH_HOOK(DROP__ctor, DROP__ctor_hook);
    ATTACH_HOOK(DROP__dtor, DROP__dtor_hook);

    Patch4(0x0050F9B1 + 1, 0xA0 + 0x8); // ZRecyclableAvBuffer<ZRefCountedDummy<DROP>,16,DROP>::raw_new - uBlockSize + 0x8
    PatchJmp(CDropPool__OnDropEnterField_jmp, reinterpret_cast<uintptr_t>(&CDropPool__OnDropEnterField_hook));
    PatchJmp(CDropPool__Update_jmp, reinterpret_cast<uintptr_t>(&CDropPool__Update_hook));
}