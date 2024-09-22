#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


typedef void (__thiscall* CConfig__LoadGlobal_t)(CConfig*);
static auto CConfig__LoadGlobal = reinterpret_cast<CConfig__LoadGlobal_t>(0x004B51B0);

void __fastcall CConfig__LoadGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__LoadGlobal(pThis);
    // CConfig::GetOpt_Int(this, 0, "soScreenMode", 0, 0, (4 << 1) | 1);
    int soScreenMode = reinterpret_cast<unsigned int (__thiscall*)(CConfig*, int, const char*, int, int, int)>(0x004B25F0)(pThis, 0, "soScreenMode", 0, 0, (4 << 1) | 1);
    pThis->m_sysOpt().bSysOpt_LargeScreen = (soScreenMode >> 1);
    pThis->m_sysOpt().bSysOpt_WindowedMode = soScreenMode != 0 ? soScreenMode & 1 : 1;
}


typedef void (__thiscall* CConfig__SaveGlobal_t)(CConfig*);
static auto CConfig__SaveGlobal = reinterpret_cast<CConfig__SaveGlobal_t>(0x004B3BE0);

void __fastcall CConfig__SaveGlobal_hook(CConfig* pThis, void* _EDX) {
    CConfig__SaveGlobal(pThis);
    int soScreenMode = (pThis->m_sysOpt().bSysOpt_LargeScreen << 1);
    if (pThis->m_sysOpt().bSysOpt_WindowedMode) {
        soScreenMode |= 1;
    }
    // CConfig::SetOpt_Int(this, 0, "soScreenMode", soScreenMode);
    reinterpret_cast<void (__thiscall*)(CConfig*, int, const char*, int)>(0x004B2650)(pThis, 0, "soScreenMode", soScreenMode);
}


class CUISysOpt {
public:
    MEMBER_AT(CONFIG_SYSOPT, 0x150, m_sysOptCur)
};

typedef unsigned int FONT_TYPE;

class CCtrlComboBox {
public:
    struct CREATEPARAM {
        int nBackColor;
        int nBackFocusedColor;
        int nBorderColor;
        int nBoxPosX;
        int nBoxPosY;
        int nBoxWidth;
        int nBoxTextLeftOffset;
        int nDesign;
        int nMaxItem_Shown;
        int nType_ScrollBar;
        unsigned int Id_ScrollBar;
        FONT_TYPE fType;
        FONT_TYPE fTypeFocused;
        FONT_TYPE fTypeSelect;
        FONT_TYPE fTypeSelectFocused;
        int bButtonOnLeft;
        int bAlwayFocusOnText;
        int bDeleteEnable;
        int nItemListPosType;
        Ztl_bstr_t sButtonUOL;
        int nTextOffSet_Y;
    };
    static_assert(sizeof(CREATEPARAM) == 0x54);

    unsigned char padding[0x110];

    MEMBER_AT(int, 0x68, m_nSelect)
};
static_assert(sizeof(CCtrlComboBox) == 0x110);

static ZRef<CCtrlComboBox> g_cbResolution;

typedef void (__thiscall* CUISysOpt__OnCreate_t)(CUISysOpt*, void*);
static auto CUISysOpt__OnCreate = reinterpret_cast<CUISysOpt__OnCreate_t>(0x00978010);

void __fastcall CUISysOpt__OnCreate_hook(CUISysOpt* pThis, void* _EDX, void* pData) {
    DEBUG_MESSAGE("CUISysOpt::OnCreate");
    CUISysOpt__OnCreate(pThis, pData);

    CCtrlComboBox::CREATEPARAM paramComboBox;
    // CCtrlComboBox::CREATEPARAM::CREATEPARAM(&paramComboBox);
    reinterpret_cast<void (__thiscall*)(CCtrlComboBox::CREATEPARAM*)>(0x004894F0)(&paramComboBox);
    paramComboBox.nBackColor = 0xFFEEEEEE;
    paramComboBox.nBackFocusedColor = 0xFFA5A198;
    paramComboBox.nBorderColor = 0xFF999999;

    // ZRef<CCtrlComboBox>::~ZRef<CCtrlComboBox>(&g_cbResolution);
    reinterpret_cast<void (__thiscall*)(ZRef<CCtrlComboBox>*)>(0x0047FE80)(&g_cbResolution);
    // ZAllocEx<ZAllocAnonSelector>::Alloc(&ZAllocEx<ZAllocAnonSelector>::_s_alloc, 0x110u);
    g_cbResolution.p = reinterpret_cast<CCtrlComboBox*>(ZAllocEx<ZAllocAnonSelector>::s_Alloc(0x110));
    // CCtrlComboBox::CCtrlComboBox(g_cbResolution.p);
    reinterpret_cast<void (__thiscall*)(CCtrlComboBox*)>(0x004DA090)(g_cbResolution.p);
    // CCtrlComboBox::CreateCtrl(g_cbResolution.p, this, 2000, 0, 65, 58, 174, 18, &paramComboBox);
    reinterpret_cast<void (__thiscall*)(CCtrlComboBox*, void*, unsigned int, int, int, int, int, int, void*)>(0x004DA330)(g_cbResolution.p, pThis, 2000, 0, 65, 58, 174, 18, &paramComboBox);

    const char* asResolution[] = {
        "800 x 600",
        "1024 x 768",
        "1366 x 768",
        "1600 x 900",
        "1920 x 1080"
    };
    size_t nResolution = sizeof(asResolution) / sizeof(asResolution[0]);
    for (size_t i = 0; i < nResolution; ++i) {
        // CCtrlComboBox::AddItem(g_cbResolution.p, asResolution[i], i);
        reinterpret_cast<void (__thiscall*)(CCtrlComboBox*, char*, unsigned int)>(0x004DE640)(g_cbResolution.p, const_cast<char*>(asResolution[i]), i);
    }

    // CCtrlComboBox::SetSelect(g_cbResolution.p, this->m_sysOptCur.bSysOpt_LargeScreen);
    reinterpret_cast<void (__thiscall*)(CCtrlComboBox*, int)>(0x004D90E0)(g_cbResolution.p, pThis->m_sysOptCur().bSysOpt_LargeScreen);
}


typedef void (__thiscall* CUISysOpt__GetSysOptFromCtrl_t)(CUISysOpt*);
static auto CUISysOpt__GetSysOptFromCtrl = reinterpret_cast<CUISysOpt__GetSysOptFromCtrl_t>(0x009692A0);

void __fastcall CUISysOpt__GetSysOptFromCtrl_hook(CUISysOpt* pThis, void* _EDX) {
    DEBUG_MESSAGE("CUISysOpt::GetSysOptFromCtrl");
    CUISysOpt__GetSysOptFromCtrl(pThis);
    pThis->m_sysOptCur().bSysOpt_LargeScreen = g_cbResolution.p->m_nSelect();
}


typedef void (__thiscall* CWvsContext__SetScreenResolution_t)(CWvsContext*, bool, bool);
static auto CWvsContext__SetScreenResolution = reinterpret_cast<CWvsContext__SetScreenResolution_t>(0x009CD0C0);

void __fastcall CWvsContext__SetScreenResolution_hook(CWvsContext* pThis, void* _EDX, bool bLargeScreen, bool bSave) {
    // Resolve screen size
    int nResolution = CConfig::GetInstance()->m_sysOpt().bSysOpt_LargeScreen;
    int nScreenWidth = 800;
    int nScreenHeight = 600;
    int nAdjustCenterY = 0;
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
    // Update resolution
    if (get_gr()->put_screenResolution(nScreenWidth, nScreenHeight) >= 0) {
        CHECK_HRESULT(get_gr()->raw_AdjustCenter(0, -nAdjustCenterY));
        pThis->m_nScreenWidth() = nScreenWidth;
        pThis->m_nScreenHeight() = nScreenHeight;
        pThis->m_nAdjustCenterY() = nAdjustCenterY;
        pThis->m_bIsLargeScreen() = bLargeScreen;
        if (CWndMan::IsInstantiated()) {
            // CWndMan::OnEventChangeScreenResolution(TSingleton<CWndMan>::GetInstance())
            reinterpret_cast<void (__thiscall*)(CWndMan*)>(0x009B3150)(CWndMan::GetInstance());
        }
        if (bSave) {
            // field = get_field();
            void* field = reinterpret_cast<void* (__cdecl*)()>(0x00439320)();
            if (field) {
                // CMapLoadable::OnEventChangeScreenResolution(field);
                reinterpret_cast<void (__thiscall*)(void*)>(0x0061F550)(field);
            }
            CConfig::GetInstance()->m_sysOpt().bSysOpt_LargeScreen = nResolution;
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