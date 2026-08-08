#pragma once
#include <cstdint>
#include <xtl.h>
#include <hooks/IHookService.h>
#include <platform/PlatformServices.h>
#include <memory>
#include "Title.h"

namespace core {
    class TitleManager {
    public:
        TitleManager(platform::ISystem &system, platform::IInput &input, hooks::IHookService &hook);

        void init();
        void run();
        void stop() { m_running = false; }
    private:
        void initNewTitle(uint32_t newTitleId);
        bool m_running;
        uint32_t m_currentTitleId;
        std::string m_currentTitleVersion;
        HANDLE m_handle;
        static Title* m_currentTitle;
        hooks::IHookService &m_hook;
        platform::PlatformServices m_platform;

    };
}
