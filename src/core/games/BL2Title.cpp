#include "BL2Title.h"
#include "XexUtils.h"

namespace games {
    BL2Title* BL2Title::s_instance = nullptr;
    hooks::IHookService* BL2Title::s_activeHookService = nullptr;
    DWORD BL2Title::s_activeRefreshHandle = 0;
    // when this is ran the title is registered in TitleFactory
    bool BL2Title::s_registered = BL2Title::registerSelf(kBL2TitleId);

    DWORD WINAPI BL2Title::InstallHookThread(LPVOID lpParam) {
        BL2Title* self = static_cast<BL2Title*>(lpParam);
        DWORD targetAddr = 0x82e9d718;

        std::cerr << "[BL2Title::InstallHookThread] - Waiting for BL2 code to unpack...";

        while (true) {
            __try {
                if (MmIsAddressValid((PVOID)targetAddr)) {
                    DWORD firstInstruction = *(DWORD*)targetAddr;
                    if (firstInstruction != 0x00000000 && firstInstruction != 0xFFFFFFFF) {
                        std::cerr << "\n[BL2Title::InstallHookThread] - Code found, waiting for SNet transition to settle...";
                        Sleep(12000);
                        s_instance->m_platform.system.notify(L"[BL2Title::InstallHookThread]");
                        self->m_refreshHandle = self->m_hook.install(targetAddr, (void*)&RefreshHook);
                        s_activeHookService = &self->m_hook;
                        s_activeRefreshHandle = self->m_refreshHandle;

                        std::cerr << "\n[BL2Title::InstallHookThread()] - installing hook successful at " << std::hex << targetAddr;
                        break;
                    }
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                s_instance->m_platform.system.notify(L"[BL2Title::InstallHookThread] - Failed to install hook exception occured");
            }
            Sleep(200);

        }
        return 0;
    }

    void BL2Title::onOpen() {
        s_instance = this;
        std::cerr << "\n[BL2Title::onOpen()] - BL2 running";
        initMenu();
        initRender();
        m_platform.system.notify(L"BL2 PATCH LOADED");

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InstallHookThread, this, 0, NULL);
    }

    void BL2Title::onClose() {
        if (m_hook.isInstalled(m_refreshHandle))
            m_hook.uninstall(m_refreshHandle);
        s_activeHookService = NULL;
    }

    void BL2Title::initMenu() {

    }

    void BL2Title::initRender() {

    }

    void BL2Title::RefreshHook(void *context, void *stack, void *result) {
        std::cerr << "\n[BL2Title::RefreshHook] - hook fired";
        s_instance->m_platform.system.notify(L"[BL2 PATCH] - Firing refresh hook");


        if (s_activeHookService)
            s_activeHookService->callOriginal(s_activeRefreshHandle, context, stack, result);
    }
}
