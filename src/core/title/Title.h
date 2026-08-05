#pragma once
#include "cstdint"

namespace core {
    class Title {
    public:
        Title(uint32_t titleId, uint32_t titleVersion);

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
    private:
        uint32_t m_titleId;
        uint32_t m_titleVersion;
        virtual void initMenu() = 0;
        virtual void initRender() = 0;
    };
}



