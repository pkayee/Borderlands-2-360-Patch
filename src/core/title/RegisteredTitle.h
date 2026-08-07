#pragma once

#include "Title.h"
#include "TitleFactory.h"


namespace core {
    template<typename Derived>
    class RegisteredTitle : public Title {
    public:
        RegisteredTitle(platform::IPlatform &platform, platform::IInput &input)
        : Title(platform, input) {}

        static bool registerSelf(uint32_t titleId) {
            TitleFactory::registerTitle(titleId, &RegisteredTitle::create);
            return true;
        }
    private:
        static Title* create(platform::IPlatform &platform, platform::IInput &input) {
            return new Derived(platform, input);
        }
    };
}