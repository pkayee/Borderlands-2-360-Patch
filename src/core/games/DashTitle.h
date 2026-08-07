#pragma once
#include <title/RegisteredTitle.h>
#include <cstdint>

namespace games {
    const uint32_t kDashboardId = 0xFFFE07D1;
    class DashboardTitle : public core::RegisteredTitle<DashboardTitle> {
    public:
        DashboardTitle(platform::IPlatform &platform, platform::IInput &input) : RegisteredTitle(platform, input) {}
        void onOpen() { m_platform.notify(L"Plugin is working!"); }
        void onClose();
    private:
        void initMenu();
        void initRender();
        static bool s_registered;
    };
};