#include "TitleManager.h"
#include "TitleFactory.h"

#include "xtl.h"
#include "cstdint"
#include "iostream"

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
    Title* TitleManager::m_currentTitle = NULL;
    TitleManager::TitleManager(platform::IPlatform &platform, platform::IInput &input)
        : m_platform(platform), m_input(input), m_running(true) {
    }

    void TitleManager::init() {
        // check for kernal version will use platform header to check
    }

    void TitleManager::run() {
        while (m_running) {
            uint32_t newTitleId = m_platform.getCurrentTitleId();
            if (newTitleId != 0 && newTitleId != m_currentTitleId) {
                initNewTitle(newTitleId);
            }
        }
    }

    void TitleManager::initNewTitle(uint32_t newTitleId) {
        if (m_currentTitle) {
            m_currentTitle->onClose();
            delete m_currentTitle;
            m_currentTitle = NULL;
        }
        std::cerr << "\n" << "Initializing title:" << newTitleId << std::endl;

        m_currentTitleId = newTitleId;
        m_currentTitleVersion = m_platform.getTitleUpdateVersion();
        std::cout << "TitleUpdate = " << m_currentTitleVersion;

        m_currentTitle = TitleFactory::create(m_currentTitleId, m_currentTitleVersion, m_platform, m_input);

        if (m_currentTitle) {
            std::cerr << "\n" << "Hooking title on open";
            try {
                m_currentTitle->onOpen();
            }
            catch (const std::exception& e) {
                std::cerr << "\nException in onOpen" << e.what();
            }
            catch (...) {
                std::cerr << "\nUnknown exception caught in onOpen";
            }
        }
        else {
            std::cerr << "\n" << "TitleFactory::create returned NULL for ID "
                  << m_currentTitleId << std::endl;
        }
    }
}


