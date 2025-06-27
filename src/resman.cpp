#include "pch.h"
#include "hook.h"
#include "debug.h"
#include "wzlib/resman.h"
#include "wzlib/namespace.h"
#include "wzlib/package.h"
#include "wvs/wvsapp.h"
#include "wvs/util.h"
#include <windows.h>
#include <strsafe.h>
#include <cstdlib>


void CWvsApp::InitializeResMan() {
    try {
        IWzResManPtr& rm = get_rm();
        PcCreateObject<IWzResManPtr>(L"ResMan", rm, nullptr);
        CHECK_HR(rm->raw_SetResManParam(RESMAN_PARAM::RC_AUTO_REPARSE | RESMAN_PARAM::RC_AUTO_SERIALIZE, -1, -1));

        IWzNameSpacePtr& root = get_root();
        PcCreateObject<IWzNameSpacePtr>(L"NameSpace", root, nullptr);
        PcSetRootNameSpace(root);

        IWzFileSystemPtr fs;
        PcCreateObject<IWzFileSystemPtr>(L"NameSpace#FileSystem", fs, nullptr);
        char sStartPath[MAX_PATH];
        GetModuleFileNameA(nullptr, sStartPath, MAX_PATH);
        Dir_BackSlashToSlash(sStartPath);
        Dir_upDir(sStartPath);
        CHECK_HR(fs->raw_Init(static_cast<wchar_t*>(Ztl_bstr_t(sStartPath))));

        IWzPackagePtr pPackage;
        PcCreateObject<IWzPackagePtr>(L"NameSpace#Package", pPackage, nullptr);
        Ztl_variant_t vBaseWz;
        CHECK_HR(fs->get_item(L"Base.wz", &vBaseWz));
        IWzSeekableArchivePtr pArchive = vBaseWz.GetUnknown(false, false);
        CHECK_HR(pPackage->raw_Init(L"95", L"", pArchive));
        CHECK_HR(root->raw_Mount(L"/", pPackage, 0));

        const wchar_t* asNameOrder[] = {L"Character", L"Mob", L"Skill", L"Reactor", L"Npc", L"UI", L"Quest", L"Item", L"Effect", L"String", L"Etc", L"Morph", L"TamingMob", L"Sound", L"Map"};
        for (size_t i = 0; i < 15; ++i) {
            ZXString<wchar_t> sPackageName;
            sPackageName.Format(L"%s.wz", asNameOrder[i]);
            IWzPackagePtr pSubPackage;
            PcCreateObject<IWzPackagePtr>(L"NameSpace#Package", pSubPackage, nullptr);
            Ztl_variant_t vPackageWz;
            CHECK_HR(fs->get_item(sPackageName, &vPackageWz));

            IWzSeekableArchivePtr pSubArchive = vPackageWz.GetUnknown(false, false);
            Ztl_variant_t vSubNameSpace;
            ZXString<wchar_t> sNameSpace(asNameOrder[i]);
            CHECK_HR(root->get_item(sNameSpace, &vSubNameSpace));
            get_sub(i) = vSubNameSpace.GetUnknown(false, false);
            CHECK_HR(pSubPackage->raw_Init(L"95", sNameSpace, pSubArchive));

            IWzNameSpacePtr pSubNameSpace;
            PcCreateObject<IWzNameSpacePtr>(L"NameSpace", pSubNameSpace, nullptr);
            CHECK_HR(pSubNameSpace->raw_Mount(L"/", pSubPackage, 0));
            CHECK_HR(get_sub(i)->raw_Mount(L"/", pSubNameSpace, 1));
        }
    } catch (const _com_error& e) {
        HRESULT hr = e.Error();
        ZException exception(hr);
        if (hr == 0x80070005) {
            hr = 0x22000005; // EC_INVALID_GAME_DATA_VERSION
        } else if (hr == 0x80070057) {
            hr = 0x22000003; // EC_NOT_ENOUGH_MEMORY
        } else {
            hr = 0x22000004; // EC_NO_DATA_PACAKGE
        }
        // CTerminateException::CTerminateException(&exception, hr);
        reinterpret_cast<void(__thiscall*)(void*, HRESULT)>(0x00401D50)(&exception, hr);
        throw exception;
    }
}


void CWvsApp::Dir_BackSlashToSlash(char* sDir) {
    size_t uLen = strlen(sDir);
    for (size_t i = 0; i < uLen; ++i) {
        if (sDir[i] == '\\') {
            sDir[i] = '/';
        }
    }
}

void CWvsApp::Dir_SlashToBackSlash(char* sDir) {
    size_t uLen = strlen(sDir);
    for (size_t i = 0; i < uLen; ++i) {
        if (sDir[i] == '/') {
            sDir[i] = '\\';
        }
    }
}

void CWvsApp::Dir_upDir(char* sDir) {
    size_t uLen = strlen(sDir);
    if (uLen > 0 && sDir[uLen - 1] == '/') {
        sDir[uLen - 1] = 0;
    }
    for (size_t i = strlen(sDir) - 1; i > 0; --i) {
        if (sDir[i] == '/') {
            sDir[i] = 0;
            return;
        }
    }
}