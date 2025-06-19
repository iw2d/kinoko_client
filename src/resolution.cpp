#include "pch.h"
#include "hook.h"
#include "ztl/zalloc.h"
#include "wvs/config.h"
#include "wvs/dialog.h"
#include "wvs/ctrlwnd.h"
#include "wvs/wvscontext.h"
#include "wvs/wndman.h"
#include "wvs/field.h"

#include <strsafe.h>
#include <cstdint>


static auto CConfig__LoadGlobal = reinterpret_cast<void(__thiscall*)(CConfig*)>(0x004B51B0);

void __fastcall CConfig__LoadGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__LoadGlobal(pThis);
    int32_t soScreenMode = pThis->GetOpt_Int(CConfig::GLOBAL_OPT, "soScreenMode", 0, 0, (4 << 1) | 1);
    pThis->m_sysOpt.bSysOpt_LargeScreen = (soScreenMode >> 1);
    pThis->m_sysOpt.bSysOpt_WindowedMode = soScreenMode != 0 ? soScreenMode & 1 : 1;
}

static auto CConfig__SaveGlobal = reinterpret_cast<void(__thiscall*)(CConfig*)>(0x004B3BE0);

void __fastcall CConfig__SaveGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__SaveGlobal(pThis);
    int32_t soScreenMode = (pThis->m_sysOpt.bSysOpt_LargeScreen << 1);
    if (pThis->m_sysOpt.bSysOpt_WindowedMode) {
        soScreenMode |= 1;
    }
    pThis->SetOpt_Int(CConfig::GLOBAL_OPT, "soScreenMode", soScreenMode);
}


class CUISysOpt : public CDialog {
public:
    MEMBER_AT(CONFIG_SYSOPT, 0x150, m_sysOptCur)
};

static ZRef<CCtrlComboBox> g_cbResolution;

static auto CUISysOpt__OnCreate = reinterpret_cast<void(__thiscall*)(CUISysOpt*, void*)>(0x00978010);

void __fastcall CUISysOpt__OnCreate_hook(CUISysOpt* pThis, void* _EDX, void* pData) {
    CUISysOpt__OnCreate(pThis, pData);

    CCtrlComboBox::CREATEPARAM paramComboBox;
    paramComboBox.nBackColor = 0xFFEEEEEE;
    paramComboBox.nBackFocusedColor = 0xFFA5A198;
    paramComboBox.nBorderColor = 0xFF999999;

    g_cbResolution = new CCtrlComboBox();
    g_cbResolution->CreateCtrl(pThis, 2000, 0, 65, 58, 174, 18, &paramComboBox);

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
}

static auto CUISysOpt__GetSysOptFromCtrl = reinterpret_cast<void(__thiscall*)(CUISysOpt*)>(0x009692A0);

void __fastcall CUISysOpt__GetSysOptFromCtrl_hook(CUISysOpt* pThis, void* _EDX) {
    CUISysOpt__GetSysOptFromCtrl(pThis);
    pThis->m_sysOptCur.bSysOpt_LargeScreen = g_cbResolution->m_nSelect;
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
            // field = get_field();
            void* field = reinterpret_cast<void*(__cdecl*)()>(0x00439320)();
            if (field) {
                // CMapLoadable::OnEventChangeScreenResolution(field);
                reinterpret_cast<void(__thiscall*)(void*)>(0x0061F550)(field);
            }
            CConfig::GetInstance()->m_sysOpt.bSysOpt_LargeScreen = nResolution;
            CConfig__SaveGlobal_hook(CConfig::GetInstance(), _EDX);
        }
    }
}


void AttachResolutionMod() {
    ATTACH_HOOK(CConfig__LoadGlobal, CConfig__LoadGlobal_hook);
    ATTACH_HOOK(CConfig__SaveGlobal, CConfig__SaveGlobal_hook);
    ATTACH_HOOK(CUISysOpt__OnCreate, CUISysOpt__OnCreate_hook);
    ATTACH_HOOK(CUISysOpt__GetSysOptFromCtrl, CUISysOpt__GetSysOptFromCtrl_hook);
    ATTACH_HOOK(CWvsContext__SetScreenResolution, CWvsContext__SetScreenResolution_hook);
}