#pragma once

#include <title/RegisteredTitle.h>
#include <cstdint>

namespace games {
    const unsigned int kBL2TitleId = 0x5454087C;
    class BL2Title : public core::RegisteredTitle<BL2Title> {
    public:
        BL2Title(platform::PlatformServices &platform, hooks::IHookService &hook) : RegisteredTitle(platform, hook) {}

        void onClose();
        void onOpen();

    protected:
        void initMenu();
        void initRender();

    private:
        static BL2Title* s_instance;
        static bool s_registered;
        DWORD m_refreshHandle;
        static void RefreshHook(void *context, void *stack, void *result);
        static hooks::IHookService* s_activeHookService;
        static DWORD s_activeRefreshHandle;
        static DWORD WINAPI InstallHookThread(LPVOID lpParam);
    };
};