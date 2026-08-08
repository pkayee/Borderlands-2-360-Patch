#pragma once
#include "cstdint"
#include <platform/PlatformServices.h>

#include "hooks/IHookService.h"


namespace core {
    class Title {
    public:
        Title(platform::PlatformServices &platformServices, hooks::IHookService &hook)
        : m_platform(platformServices), m_hook(hook),
        m_titleId(m_platform.system.getCurrentTitleId()), m_titleVersion(m_platform.system.getTitleUpdateVersion()) {

        }

        virtual ~Title();

        virtual void onOpen() = 0;
        virtual void onClose() = 0;

        virtual void update();
        virtual void render();

        bool requestOpen();
        bool requestClose();
        bool requestReboot();

    protected:
        platform::PlatformServices m_platform;
        hooks::IHookService &m_hook;

    private:
        Title(const Title &);
        Title& operator=(const Title &);
        const uint32_t m_titleId;
        const std::string m_titleVersion;
        virtual void initMenu() = 0;
        virtual void initRender() = 0;
    };
}



