#pragma once

#include <title/RegisteredTitle.h>
#include <cstdint>

namespace games {
    const unsigned int kBL2TitleId = 0x5454087C;
    class BL2Title : public core::RegisteredTitle<BL2Title> {
    public:
        BL2Title(platform::PlatformServices &platform) : RegisteredTitle(platform) {}

        void onClose();
        void onOpen();

    protected:
        void initMenu();
        void initRender();

    private:
        static bool s_registered;
    };
};