#pragma once
#include "cstdint"
#include "string"

namespace platform {
    class IPlatform {
    public:
        virtual ~IPlatform() = default;
        virtual void reboot() = 0;
        virtual void notify(const std::wstring &message) = 0;
        virtual bool isDevkit() = 0;
        virtual uint32_t getFirmwareVersion() = 0;
        virtual uint32_t getCurrentTitleId() = 0;
        virtual uint32_t getTitleVersion() = 0;
    };
}
