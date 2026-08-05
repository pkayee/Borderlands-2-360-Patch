#pragma once
#include "IPlatform.h"
#include "xtl.h"

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
    class XboxPlatform : public IPlatform {
    public:
        ~XboxPlatform() override;
        void reboot() override;
        void notify(const std::wstring &message) override;
        uint32_t getCurrentTitleId() override { return XamGetCurrentTitleId(); }
        bool isDevkit() override;
    };
}
