#pragma once
#include "ztl/tsingleton.h"
#include "wzlib/shape2d.h"
#include "wzlib/gr2d.h"
#include <memory>


class CAnimationDisplayer : public TSingleton<CAnimationDisplayer, 0x00C68750> {
public:
    void RegisterOneTimeAnimation(IWzGr2DLayerPtr pLayer, int32_t tDelayBeforeStart, IWzGr2DLayerPtr pFlipOrigin) {
        reinterpret_cast<void(__thiscall*)(CAnimationDisplayer*, IWzGr2DLayerPtr, int32_t, IWzGr2DLayerPtr)>(0x00444410)(this, pLayer, tDelayBeforeStart, pFlipOrigin);
    }
    static IWzGr2DLayerPtr LoadLayer(const wchar_t* sLayerUOL, int32_t bFlip, IWzVector2DPtr pOrigin, int32_t rx, int32_t ry, IWzGr2DLayerPtr pOverlay, int32_t z, int32_t alpha, int32_t magLevel) {
        IWzGr2DLayerPtr result;
        reinterpret_cast<IWzGr2DLayerPtr*(__cdecl*)(IWzGr2DLayerPtr*, const wchar_t*, int32_t, IWzVector2DPtr, int32_t, int32_t, IWzGr2DLayerPtr, int32_t, int32_t, int32_t)>(0x00451B10)(std::addressof(result), sLayerUOL, bFlip, pOrigin, rx, ry, pOverlay, z, alpha, magLevel);
        return result;
    }
};