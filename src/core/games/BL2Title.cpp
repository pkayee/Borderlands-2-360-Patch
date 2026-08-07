#include "BL2Title.h"

namespace games {
    bool BL2Title::s_registered = BL2Title::registerSelf(kBL2TitleId);

    void BL2Title::onOpen() {
        m_platform.notify(L"BL2 PATCH LOADED");
    }

    void BL2Title::onClose() {

    }

    void BL2Title::initMenu() {

    }

    void BL2Title::initRender() {

    }
}
