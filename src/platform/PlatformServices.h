#pragma once
#include <platform/ISystem.h>
#include <platform/IInput.h>

namespace platform {
    struct PlatformServices {
        ISystem &system;
        IInput &input;

        PlatformServices(ISystem &s, IInput &i) : system(s), input(i) {}
    };
}