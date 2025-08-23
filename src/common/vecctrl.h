#pragma once
#include "ztl/ztl.h"
#include "common/rtti.h"
#include "common/secure.h"
#include <windows.h>
#include <cstdint>


class CAttrShoe : public ZRefCounted {
public:
    TSecType<double> mass;
    TSecType<double> walkAcc;
    TSecType<double> walkSpeed;
    TSecType<double> walkDrag;
    TSecType<double> walkSlant;
    TSecType<double> walkJump;
    TSecType<double> swimAcc;
    TSecType<double> swimSpeedH;
    TSecType<double> swimSpeedV;
    TSecType<double> flyAcc;
    TSecType<double> flySpeed;

    explicit CAttrShoe(int32_t bUserFlying) {
        mass.SetData(100.0);
        walkAcc.SetData(1.0);
        walkSpeed.SetData(1.0);
        walkDrag.SetData(1.0);
        walkSlant.SetData(0.9);
        walkJump.SetData(1.0);
        swimAcc.SetData(1.0);
        swimSpeedH.SetData(1.0);
        swimSpeedV.SetData(1.0);
        if (bUserFlying) {
            flyAcc.SetData(1.0);
            flySpeed.SetData(1.0);
        } else {
            flyAcc.SetData(0.0);
            flySpeed.SetData(0.0);
        }
    }
    virtual ~CAttrShoe() = default;
};
static_assert(sizeof(CAttrShoe) == 0x90);


class CLadderOrRope {
public:
    uint32_t dwSN;
    int32_t bLadder;
    int32_t bUpperFoothold;
    int32_t x;
    int32_t y1;
    int32_t y2;
    int32_t nPage;
};
static_assert(sizeof(CLadderOrRope) == 0x1C);


class CAttrFoothold : public ZRefCounted {
    TSecType<double> walk;
    TSecType<double> drag;
    TSecType<double> force;
    TSecType<int32_t> forbidfalldown;
    TSecType<int32_t> cantThrough;
};
static_assert(sizeof(CAttrFoothold) == 0x48);


class CStaticFoothold : public ZRefCounted {
    int32_t m_x1;
    int32_t m_y1;
    int32_t m_x2;
    int32_t m_y2;
    int32_t m_xReal1;
    int32_t m_yReal1;
    int32_t m_xReal2;
    int32_t m_yReal2;
    int32_t m_lPage;
    int32_t m_lZMass;
    ZRef<CAttrFoothold> m_pAttrFoothold;
    double m_uvx;
    double m_uvy;
    double m_len;
    uint32_t m_dwSN;
    int32_t m_nState;
    CStaticFoothold* m_posDel;
    union {
        uint32_t m_dwSNPrev;
        CStaticFoothold* m_pfhPrev;
    };
    union {
        uint32_t m_dwSNNext;
        CStaticFoothold* m_pfhNext;
    };
};
static_assert(sizeof(CStaticFoothold) == 0x70);


class CVecCtrl : public ZRefCounted, public IWzVector2D {
    // TODO
};


class IVecCtrlOwner {
public:
    virtual int32_t GetType() const = 0;
    virtual int32_t OnResolveMoveAction(int32_t nInputX, int32_t nInputY, int32_t nCurMoveAction, const CVecCtrl* pvc) = 0;
    virtual int32_t OnLayerZChanged(const CVecCtrl* pvc) = 0;
    virtual const ZRef<CAttrShoe>* GetShoeAttr(const ZRef<CAttrShoe>* result) const = 0;
    virtual const POINT* GetPos(const POINT* result) const = 0;
    virtual const POINT* GetPosPrev(const POINT* result) const = 0;
    virtual int32_t GetZMass() const = 0;
    virtual const CRTTI* GetRTTI() const = 0;
    virtual int32_t IsKindOf(const CRTTI* pRTTI) const = 0;
};