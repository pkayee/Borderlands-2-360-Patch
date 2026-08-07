#pragma once
#include <cstdint>
#include <xtl.h>
#include <platform/IPlatform.h>
#include <memory>
#include "Title.h"
#include "platform/IInput.h"

namespace core {
    class TitleManager {
    public:
        TitleManager(platform::IPlatform &platform, platform::IInput &input);
        void init();
        void run();
        void stop() { m_running = false; }
    private:
        void initNewTitle(uint32_t newTitleId);
        bool m_running;
        uint32_t m_currentTitleId;
        std::string m_currentTitleVersion;
        HANDLE m_handle;
        std::unique_ptr<Title> m_currentTitle;
        platform::IPlatform &m_platform;
        platform::IInput &m_input;
    };
}
