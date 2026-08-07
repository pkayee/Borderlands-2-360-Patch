#pragma once
#include "cstdint"
#include "memory"
#include "function.h"
#include <platform/IPlatform.h>
#include <platform/IInput.h>

#include "functional"
#include "Title.h"
#include "map"

namespace core {
    class TitleFactory {
    public:
        // function pointer that returns our newly constructed derived class
        // see RegisteredTitle.h for more context
        typedef Title* (*Creator)(platform::IPlatform&, platform::IInput&);

        static void registerTitle(uint32_t titleId, Creator creator);
        static Title* create(uint32_t titleId, std::string& titleVersion, platform::IPlatform &platform, platform::IInput &input);

    private:
        static std::map<uint32_t, Creator>& registry();
    };
}
