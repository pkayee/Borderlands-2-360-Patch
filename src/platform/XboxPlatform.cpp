//
// Created by pixel on 8/5/2026.
//

#include "XboxPlatform.h"

namespace platform {
    void XboxPlatform::reboot() {

    }

    void XboxPlatform::notify(const std::wstring &message) {

    }

    uint32_t XboxPlatform::getCurrentTitleId() {
        return XamGetCurrentTitleId();
    }
}
