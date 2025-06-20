#include "pch.h"
#include "hook.h"
#include "ztl/zalloc.h"
#include "wzlib/pcom.h"
#include "wzlib/sound.h"
#include "wvs/config.h"
#include "wvs/dialog.h"
#include "wvs/ctrlwnd.h"
#include "wvs/wvscontext.h"
#include "wvs/wndman.h"
#include "wvs/soundman.h"
#include "wvs/field.h"

#include <strsafe.h>
#include <cstdint>


class CUISysOpt : public CDialog {
public:
    MEMBER_AT(CONFIG_SYSOPT, 0x150, m_sysOptCur)
};

static ZRef<CCtrlComboBox> g_cbResolution;
static ZRef<CCtrlCheckBox> g_cbGlobalFocus;
static int32_t g_bGlobalFocus;


static auto CUISysOpt__OnCreate = reinterpret_cast<void(__thiscall*)(CUISysOpt*, void*)>(0x00978010);

void __fastcall CUISysOpt__OnCreate_hook(CUISysOpt* pThis, void* _EDX, void* pData) {
    CUISysOpt__OnCreate(pThis, pData);

    CCtrlComboBox::CREATEPARAM paramComboBox;
    paramComboBox.nBackColor = 0xFFEEEEEE;
    paramComboBox.nBackFocusedColor = 0xFFA5A198;
    paramComboBox.nBorderColor = 0xFF999999;

    g_cbResolution = new CCtrlComboBox();
    g_cbResolution->CreateCtrl(pThis, 2000, 0, 65, 58, 100, 18, &paramComboBox);
    const char* asResolution[] = {
        "800 x 600",
        "1024 x 768",
        "1366 x 768",
        "1600 x 900",
        "1920 x 1080",
    };
    size_t nResolution = sizeof(asResolution) / sizeof(asResolution[0]);
    for (size_t i = 0; i < nResolution; ++i) {
        g_cbResolution->AddItem(asResolution[i], i);
    }
    g_cbResolution->SetSelect(pThis->m_sysOptCur.bSysOpt_LargeScreen);

    CCtrlCheckBox::CREATEPARAM paramCheckBox;
    paramCheckBox.nBackColor = 0;
    paramCheckBox.nWidth = 150;
    paramCheckBox.nHeight = 15;

    g_cbGlobalFocus = new CCtrlCheckBox();
    g_cbGlobalFocus->CreateCtrl(pThis, 2001, 165, 60, &paramCheckBox);
    g_cbGlobalFocus->SetChecked(g_bGlobalFocus);
}

static auto CUISysOpt__GetSysOptFromCtrl = reinterpret_cast<void(__thiscall*)(CUISysOpt*)>(0x009692A0);

void __fastcall CUISysOpt__GetSysOptFromCtrl_hook(CUISysOpt* pThis, void* _EDX) {
    CUISysOpt__GetSysOptFromCtrl(pThis);
    pThis->m_sysOptCur.bSysOpt_LargeScreen = g_cbResolution->m_nSelect;
    g_bGlobalFocus = g_cbGlobalFocus->m_bChecked;
}


static auto CConfig__LoadGlobal = reinterpret_cast<void(__thiscall*)(CConfig*)>(0x004B51B0);

void __fastcall CConfig__LoadGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__LoadGlobal(pThis);
    int32_t soScreenMode = pThis->GetOpt_Int(CConfig::GLOBAL_OPT, "soScreenMode", 0, 0, (4 << 1) | 1);
    pThis->m_sysOpt.bSysOpt_LargeScreen = (soScreenMode >> 1);
    pThis->m_sysOpt.bSysOpt_WindowedMode = soScreenMode != 0 ? soScreenMode & 1 : 1;
    g_bGlobalFocus = pThis->GetOpt_Int(CConfig::GLOBAL_OPT, "soGlobalFocus", 0, 0, 1);
}

static auto CConfig__SaveGlobal = reinterpret_cast<void(__thiscall*)(CConfig*)>(0x004B3BE0);

void __fastcall CConfig__SaveGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__SaveGlobal(pThis);
    int32_t soScreenMode = (pThis->m_sysOpt.bSysOpt_LargeScreen << 1);
    if (pThis->m_sysOpt.bSysOpt_WindowedMode) {
        soScreenMode |= 1;
    }
    pThis->SetOpt_Int(CConfig::GLOBAL_OPT, "soScreenMode", soScreenMode);
    pThis->SetOpt_Int(CConfig::GLOBAL_OPT, "soGlobalFocus", g_bGlobalFocus);
}

static auto CConfig__ApplySysOpt = reinterpret_cast<void(__thiscall*)(CConfig*, CONFIG_SYSOPT*, int32_t)>(0x004B2300);

void __fastcall CConfig__ApplySysOpt_hook(CConfig* pThis, void* _EDX, CONFIG_SYSOPT* pSysOpt, int32_t bApplyVideo) {
    CConfig__ApplySysOpt(pThis, pSysOpt, bApplyVideo);
    if (pSysOpt && !bApplyVideo) {
        return;
    }
    IWzSoundPtr pSound;
    PcCreateObject<IWzSoundPtr>(L"Sound_DX8", pSound, nullptr);
    int32_t bOldGlobalFocus;
    CHECK_HR(pSound->get_globalFocus(&bOldGlobalFocus));
    if (bOldGlobalFocus == g_bGlobalFocus) {
        return;
    }
    CHECK_HR(pSound->put_globalFocus(g_bGlobalFocus));
    if (!bApplyVideo) {
        return;
    }
    auto pField = get_field();
    if (pField) {
        CSoundMan::GetInstance()->PlayBGM(CSoundMan::GetInstance()->m_sBGMPath, 1, 600, 600, 1);
    }
}


static auto CWvsContext__SetScreenResolution = reinterpret_cast<void(__thiscall*)(CWvsContext*, bool, bool)>(0x009CD0C0);

void __fastcall CWvsContext__SetScreenResolution_hook(CWvsContext* pThis, void* _EDX, bool bLargeScreen, bool bSave) {
    int32_t nResolution = CConfig::GetInstance()->m_sysOpt.bSysOpt_LargeScreen;
    int32_t nScreenWidth = 800;
    int32_t nScreenHeight = 600;
    int32_t nAdjustCenterY = 0;
    if (bLargeScreen) {
        switch (nResolution) {
        case 1:
            nScreenWidth = 1024;
            nScreenHeight = 768;
            nAdjustCenterY = 84;
            break;
        case 2:
            nScreenWidth = 1366;
            nScreenHeight = 768;
            nAdjustCenterY = 84;
            break;
        case 3:
            nScreenWidth = 1600;
            nScreenHeight = 900;
            nAdjustCenterY = 84;
            break;
        case 4:
            nScreenWidth = 1920;
            nScreenHeight = 1080;
            nAdjustCenterY = 84;
            break;
        }
    }
    if (SUCCEEDED(get_gr()->put_screenResolution(nScreenWidth, nScreenHeight))) {
        CHECK_HR(get_gr()->raw_AdjustCenter(0, -nAdjustCenterY));
        pThis->m_nScreenWidth = nScreenWidth;
        pThis->m_nScreenHeight = nScreenHeight;
        pThis->m_nAdjustCenterY = nAdjustCenterY;
        pThis->m_bIsLargeScreen = bLargeScreen;
        if (CWndMan::IsInstantiated()) {
            // CWndMan::OnEventChangeScreenResolution(TSingleton<CWndMan>::GetInstance())
            reinterpret_cast<void(__thiscall*)(CWndMan*)>(0x009B3150)(CWndMan::GetInstance());
        }
        if (bSave) {
            auto pField = get_field();
            if (pField) {
                // CMapLoadable::OnEventChangeScreenResolution(pField);
                reinterpret_cast<void(__thiscall*)(CMapLoadable*)>(0x0061F550)(pField);
            }
            CConfig::GetInstance()->m_sysOpt.bSysOpt_LargeScreen = nResolution;
            CConfig__SaveGlobal_hook(CConfig::GetInstance(), _EDX);
        }
    }
}


static auto CDialog__CreateDlg = reinterpret_cast<void(__thiscall*)(CDialog*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void*, CWnd::UIOrigin)>(0x004FE6D0);

void __fastcall CDialog__CreateDlg_hook(CDialog* pThis, void* _EDX, int32_t l, int32_t t, int32_t w, int32_t h, int32_t z, int32_t bScreenCoord, void* pData, CWnd::UIOrigin origin) {
    l = (CWvsContext::GetInstance()->m_nScreenWidth - w) / 2;
    t = (CWvsContext::GetInstance()->m_nScreenHeight - h) / 2 - 30;
    CDialog__CreateDlg(pThis, l, t, w, h, z, bScreenCoord, pData, origin);
}


void AttachSystemOptionMod() {
    ATTACH_HOOK(CUISysOpt__OnCreate, CUISysOpt__OnCreate_hook);
    ATTACH_HOOK(CUISysOpt__GetSysOptFromCtrl, CUISysOpt__GetSysOptFromCtrl_hook);
    ATTACH_HOOK(CConfig__LoadGlobal, CConfig__LoadGlobal_hook);
    ATTACH_HOOK(CConfig__SaveGlobal, CConfig__SaveGlobal_hook);
    ATTACH_HOOK(CConfig__ApplySysOpt, CConfig__ApplySysOpt_hook);
    ATTACH_HOOK(CWvsContext__SetScreenResolution, CWvsContext__SetScreenResolution_hook);

    // CWorldMapDlg::CreateWorldMapDlg - center world map
    PatchCall(0x009BEA22, reinterpret_cast<uintptr_t>(&CDialog__CreateDlg_hook));

    // CWvsApp::CreateWndManager - patch cursor boundary
    Patch4(0x009C20DF + 1, 1920);
    Patch4(0x009C20DA + 1, 1080);

    // CUISysOpt::OnCreate - hide m_pCBScreen1024
    Patch4(0x0097826E + 1, 65);
}