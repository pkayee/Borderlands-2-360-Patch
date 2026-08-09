#include "BL2Title.h"
#include "XexUtils.h"

namespace games {
    BL2Title* BL2Title::s_instance = nullptr;
    hooks::IHookService* BL2Title::s_activeHookService = nullptr;
    DWORD BL2Title::s_activeRefreshHandle = 0;
    // when this is ran the title is registered in TitleFactory
    bool BL2Title::s_registered = BL2Title::registerSelf(kBL2TitleId);

    DWORD WINAPI BL2Title::InstallHookThread(LPVOID lpParam) {
        std::cerr << "\n[BL2Title::InstallHookThread] - Waiting for BL2 code to unpack...";

        const int maxAttempts = 20;
        const int delayMs = 500;
        bool ready = false;

        for (int attempt = 0; attempt < maxAttempts; attempt++) {
            Sleep(delayMs);

            if (MmIsAddressValid((PVOID)0x82e9d718)) {
                std::cerr << "\n[BL2Title::InstallHookThread] - Address valid on attempt " << attempt
                           << ", waiting extra settle time...";
                Sleep(2000);
                ready = true;
                break;
            }
        }

        if (!ready) {
            std::cerr << "\n[BL2Title::InstallHookThread] - Gave up waiting, hook NOT installed";
            return 1;
        }

        std::cerr << "\n[BL2Title::InstallHookThread] - Proceeding to install hook";
        s_instance->m_refreshHandle = s_instance->m_hooks.install(0x82e9d718, (void*)&RefreshHook);

        if (s_instance->m_hooks.isInstalled(s_instance->m_refreshHandle)) {
            std::cerr << "\n[BL2Title::InstallHookThread] - Hook installed successfully";

            return 0;
        } else {
            std::cerr << "\n[BL2Title::InstallHookThread] - Hook install FAILED";
            return 1;
        }
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
        if (m_hooks.isInstalled(m_refreshHandle))
            m_hooks.uninstall(m_refreshHandle);
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
