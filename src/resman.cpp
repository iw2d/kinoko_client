#include "pch.h"
#include "client.h"
#include "hook.h"
#include "debug.h"


void InitializeResMan(const IWzResManPtr& rm) {
    DEBUG_MESSAGE("InitializeResMan");
    try {
        PcCreateObject<IWzResManPtr>(L"ResMan", const_cast<IWzResManPtr*>(&rm), nullptr);
        CHECK_HRESULT(rm->raw_SetResManParam(RESMAN_PARAM::RC_AUTO_REPARSE | RESMAN_PARAM::RC_AUTO_SERIALIZE, -1, -1));

        DEBUG_MESSAGE("InitializeResMan - Creating NameSpace");
        const IWzNameSpacePtr& root = get_root();
        PcCreateObject<IWzNameSpacePtr>(L"NameSpace", const_cast<IWzNameSpacePtr*>(&root), nullptr);
        PcSetRootNameSpace(root);

        DEBUG_MESSAGE("InitializeResMan - Creating FileSystem");
        const IWzFileSystemPtr fs;
        PcCreateObject<IWzFileSystemPtr>(L"NameSpace#FileSystem", const_cast<IWzFileSystemPtr*>(&fs), nullptr);
        char sStartPath[260];
        GetModuleFileNameA(NULL, sStartPath, 260);
        CWvsApp::Dir_BackSlashToSlash(sStartPath);
        CWvsApp::Dir_upDir(sStartPath);
        CHECK_HRESULT(fs->raw_Init(Ztl_bstr_t(sStartPath).m_Data->m_wstr));

        DEBUG_MESSAGE("InitializeResMan - Base.wz");
        const IWzPackagePtr package;
        PcCreateObject<IWzPackagePtr>(L"Namespace#Package", const_cast<IWzPackagePtr*>(&package), nullptr);
        Ztl_variant_t vBaseWz;
        CHECK_HRESULT(fs->get_item(L"Base.wz", &vBaseWz));
        const IWzSeekableArchivePtr archive(vBaseWz.GetUnknown(false, false));
        CHECK_HRESULT(package->raw_Init(L"95", L"", archive));
        CHECK_HRESULT(root->raw_Mount(L"/", package, 0));

        const wchar_t* asNameOrder[] = {
            L"Character",
            L"Mob",
            L"Skill",
            L"Reactor",
            L"Npc",
            L"UI",
            L"Quest",
            L"Item",
            L"Effect",
            L"String",
            L"Etc",
            L"Morph",
            L"TamingMob",
            L"Sound",
            L"Map"
        };
        size_t nNameOrder = sizeof(asNameOrder) / sizeof(asNameOrder[0]);
        for (size_t i = 0; i < nNameOrder; ++i) {
            wchar_t sPackageName[50];
            swprintf(sPackageName, 50, L"%s.wz", asNameOrder[i]);
            DEBUG_MESSAGE("InitializeResMan - %ls", sPackageName);
            const IWzPackagePtr subPackage;
            PcCreateObject<IWzPackagePtr>(L"NameSpace#Package", const_cast<IWzPackagePtr*>(&subPackage), nullptr);
            Ztl_variant_t vPackageWz;
            CHECK_HRESULT(fs->get_item(sPackageName, &vPackageWz));

            const IWzSeekableArchivePtr subArchive(vPackageWz.GetUnknown(false, false));
            Ztl_variant_t vSub;
            CHECK_HRESULT(root->get_item(const_cast<wchar_t*>(asNameOrder[i]), &vSub));
            get_sub(i) = vSub.GetUnknown(false, false);
            CHECK_HRESULT(subPackage->raw_Init(L"95", const_cast<wchar_t*>(asNameOrder[i]), subArchive));

            const IWzNameSpacePtr subNameSpace;
            PcCreateObject<IWzNameSpacePtr>(L"NameSpace#Package", const_cast<IWzNameSpacePtr*>(&subNameSpace), nullptr);
            CHECK_HRESULT(subNameSpace->raw_Mount(L"/", subPackage, 0));
            CHECK_HRESULT(get_sub(i)->raw_Mount(L"/", subNameSpace, 1));
        }
    } catch (const _com_error& e) {
        HRESULT hr = e.Error();
        ZException exception(hr);
        if (hr == 0x80070005) {
            hr = 0x22000005; // EC_INVALID_GAME_DATA_VERSION
        } else if (hr == 0x80070057) {
            hr = 0x22000003; // EC_NOT_ENOUGH_MEMORY
        } else {
            hr = 0x22000003; // EC_NO_DATA_PACAKGE
        }
        // CTerminateException::CTerminateException(&exception, hr);
        reinterpret_cast<void (__thiscall*)(void*, HRESULT)>(0x00401D50)(&exception, hr);
        throw exception;
    }
}