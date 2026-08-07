#pragma once

namespace platform {
    class IInput {
        virtual void poll() = 0;
    };
}