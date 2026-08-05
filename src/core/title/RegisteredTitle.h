#pragma once

#include "Title.h"
#include "TitleFactory.h"


namespace core {
    template<typename Derived>
    class RegisteredTitle : public Title {
    public:
        RegisteredTitle(platform::IPlatform &platform, platform::IInput &input) : Title(platform, input) {}

        static bool registerSelf(uint32_t titleId) {
            TitleFactory::registerTitle(titleId, [](platform::IPlatform &p, platform::IInput &i) {
                return std::unique_ptr<Title>(new Derived(p, i));
            });
            return true;
        }
    };
}