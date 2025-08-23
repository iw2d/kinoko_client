#include "pch.h"
#include "hook.h"
#include "debug.h"
#include "config.h"
#include "wvs/wvsapp.h"
#include "wvs/util.h"
#include <windows.h>
#include <strsafe.h>
#include <cstdlib>


void CWvsApp::InitializeResMan() {
    try {
        IWzResManPtr& rm = get_rm();
        PcCreateObject<IWzResManPtr>(L"ResMan", rm, nullptr);
        rm->SetResManParam(static_cast<RESMAN_PARAM>(RESMAN_PARAM::RC_AUTO_REPARSE | RESMAN_PARAM::RC_AUTO_SERIALIZE), -1, -1);

        IWzNameSpacePtr& root = get_root();
        PcCreateObject<IWzNameSpacePtr>(L"NameSpace", root, nullptr);
        PcSetRootNameSpace(root);

#ifdef CONFIG_IMAGE_LOADING
        IWzFileSystemPtr fs;
        PcCreateObject<IWzFileSystemPtr>(L"NameSpace#FileSystem", fs, nullptr);
        char sStartPath[MAX_PATH];
        GetModuleFileNameA(nullptr, sStartPath, MAX_PATH);
        Dir_BackSlashToSlash(sStartPath);
        Dir_upDir(sStartPath);
        strcat_s(sStartPath, MAX_PATH, "/Data");
        fs->Init(Ztl_bstr_t(sStartPath));
        root->Mount(L"/", fs, 0);
#else
        IWzFileSystemPtr fs;
        PcCreateObject<IWzFileSystemPtr>(L"NameSpace#FileSystem", fs, nullptr);
        char sStartPath[MAX_PATH];
        GetModuleFileNameA(nullptr, sStartPath, MAX_PATH);
        Dir_BackSlashToSlash(sStartPath);
        Dir_upDir(sStartPath);
        fs->Init(Ztl_bstr_t(sStartPath));

        IWzPackagePtr pPackage;
        PcCreateObject<IWzPackagePtr>(L"NameSpace#Package", pPackage, nullptr);
        IWzSeekableArchivePtr pArchive = fs->item[L"Base.wz"].GetUnknown();
        pPackage->Init(L"95", L"", pArchive);
        root->Mount(L"/", pPackage, 0);

        const wchar_t* asNameOrder[] = {L"Character", L"Mob", L"Skill", L"Reactor", L"Npc", L"UI", L"Quest", L"Item", L"Effect", L"String", L"Etc", L"Morph", L"TamingMob", L"Sound", L"Map"};
        for (size_t i = 0; i < 15; ++i) {
            ZXString<wchar_t> sPackageName;
            sPackageName.Format(L"%s.wz", asNameOrder[i]);
            IWzPackagePtr pSubPackage;
            PcCreateObject<IWzPackagePtr>(L"NameSpace#Package", pSubPackage, nullptr);
            IWzSeekableArchivePtr pSubArchive = fs->item[Ztl_bstr_t(sPackageName)].GetUnknown();
            get_sub(i) = root->item[asNameOrder[i]].GetUnknown();
            pSubPackage->Init(L"95", asNameOrder[i], pSubArchive);

            IWzNameSpacePtr pSubNameSpace;
            PcCreateObject<IWzNameSpacePtr>(L"NameSpace", pSubNameSpace, nullptr);
            pSubNameSpace->Mount(L"/", pSubPackage, 0);
            get_sub(i)->Mount(L"/", pSubNameSpace, 1);
        }
#endif
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