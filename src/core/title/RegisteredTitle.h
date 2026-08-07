#pragma once

#include "Title.h"
#include "TitleFactory.h"
#include <iostream>

// note function registerSelf is ran before even main as the derived classes in core/games
// define bool ExampleTitle::s_registered = ExampleTitle::registerSelf(kExampleTitleId);

// what ive done with TitleFactory is quite confusion might refactor

namespace core {
    template<typename Derived>
    class RegisteredTitle : public Title {
    public:
        RegisteredTitle(platform::IPlatform &platform, platform::IInput &input)
        : Title(platform, input) {}

        static bool registerSelf(uint32_t titleId) {
            std::cout << "\n" << "registerSelf called with TID: " << titleId;

            // pass memory of our create function which then is used by creator to return our new object of our
            // derived class
            TitleFactory::registerTitle(titleId, &RegisteredTitle::create);
            return true;
        }
    private:
        static Title* create(platform::IPlatform &platform, platform::IInput &input) {
            return new Derived(platform, input);
        }
    };
}