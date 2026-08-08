#pragma once

#include <platform/IInput.h>

namespace platform {
    class XboxInput : public IInput {
        void poll();
    };
}