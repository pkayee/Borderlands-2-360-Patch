#pragma once
#include "cstdint"
#include <platform/IPlatform.h>
#include <platform/IInput.h>

namespace core {
    class Title {
    public:
        Title(platform::IPlatform &platform, platform::IInput &input) : m_platform(platform), m_input(input) {}

        virtual ~Title();

        Title(const Title &) = delete;
        Title& operator=(const Title &) = delete;

        virtual void onOpen() = 0;
        virtual void onClose() = 0;

        virtual void update();
        virtual void render();

        bool requestOpen();
        bool requestClose();
        bool requestReboot();

    protected:
        platform::IPlatform &m_platform;
        platform::IInput &m_input;
    private:
        uint32_t m_titleId;
        uint32_t m_titleVersion;
        virtual void initMenu() = 0;
        virtual void initRender() = 0;
    };
}



