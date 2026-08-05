#include "TitleManager.h"
#include "TitleFactory.h"

#include "xtl.h"
#include "cstdint"

extern "C" {
    uint32_t XamGetCurrentTitleId();

    uint32_t ExCreateThread(
        HANDLE *pHandle,
        uint32_t stackSize,
        uint32_t *pThreadId,
        void *pApiThreadStartup,
        PTHREAD_START_ROUTINE pStartAddress,
        void *pParameter,
        uint32_t creationFlags
    );
}

namespace core {
    TitleManager::TitleManager(HANDLE titleManagerHandle, platform::IPlatform &platform, platform::IInput &input)
        : m_handle(titleManagerHandle), m_platform(platform), m_input(input), m_running(true) {
    }

    void TitleManager::init() {
        // check for kernal version will use platform header to check
    }

    void TitleManager::scanTitleIds() {
        while (m_running) {
            uint32_t newTitleId = m_platform.getCurrentTitleId();
            if (newTitleId != m_currentTitleId) {
                initNewTitle(newTitleId);
            }
        }
    }

    void TitleManager::initNewTitle(uint32_t newTitleId) {
        if (m_currentTitle) {
            m_currentTitle->onClose();
            m_currentTitle.reset();
        }

        m_currentTitleId = m_platform.getCurrentTitleId();
        m_currentTitleVersion = m_platform.getTitleVersion();

        TitleFactory::create(m_currentTitleId, m_currentTitleVersion, m_platform, m_input);

        if (m_currentTitle) {
            m_currentTitle->onOpen();
        }
    }
}


