#pragma once
#include <title/RegisteredTitle.h>
#include <cstdint>

namespace games {
    const uint32_t kDashboardId = 0xFFFE07D1;
    class DashboardTitle : public core::RegisteredTitle<DashboardTitle> {
    public:
        DashboardTitle(platform::PlatformServices &platform, hooks::IHookService &hook) : RegisteredTitle(platform, hook) {}
        void onOpen() { m_platform.system.notify(L"Plugin is working!"); }
        void onClose();
    private:
        void initMenu();
        void initRender();
        static bool s_registered;
    };
};