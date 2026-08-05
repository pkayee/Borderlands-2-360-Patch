#pragma once

#include "RegisteredTitle.h"

constexpr uint32_t kBL2TitleId = 0x0000;

namespace core {
    class BL2Title : public RegisteredTitle<BL2Title> {
    public:
        BL2Title(platform::IPlatform &platform, platform::IInput &input) : RegisteredTitle(platform, input) {}

        void onClose() override;
        void onOpen() override;

    protected:
        void initMenu() override;
        void initRender() override;

    private:
        static bool s_registered;
    };
}