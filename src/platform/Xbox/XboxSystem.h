#pragma once
#include "../ISystem.h"
#include "xtl.h"

#include <XexUtils.h>

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

namespace platform {
    class XboxSystem : public ISystem {
    public:
        XboxSystem() {};
        ~XboxSystem() {};

        void reboot() override;
        void notify(const std::wstring &message);

        uint32_t getFirmwareVersion();
        uint32_t getCurrentTitleId() { return XamGetCurrentTitleId(); }
        std::string getTitleUpdateVersion();

        bool isDevkit();
    private:
        XboxSystem(const XboxSystem &);
        XboxSystem &operator=(const XboxSystem &);
    };
}
