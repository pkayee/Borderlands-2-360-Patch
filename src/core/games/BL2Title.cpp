#include "BL2Title.h"

namespace games {
    BL2Title* BL2Title::s_instance = nullptr;
    hooks::IHookService* BL2Title::s_activeHookService = nullptr;
    DWORD BL2Title::s_activeRefreshHandle = 0;
    // when this is ran the title is registered in TitleFactory
    bool BL2Title::s_registered = BL2Title::registerSelf(kBL2TitleId);

    void BL2Title::onOpen() {
        s_instance = this;
        initMenu();
        initRender();
        m_platform.system.notify(L"BL2 PATCH LOADED");

        m_refreshHandle = m_hook.install(0x82e9d718, (void*)&RefreshHook);
        s_activeHookService = &m_hook;
        s_activeRefreshHandle = m_refreshHandle;
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
