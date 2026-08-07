#include "TitleFactory.h"
#include <iostream>
#include <memory>

#include "utils.h"

namespace core {
    void TitleFactory::registerTitle(uint32_t titleId, Creator creator) {
        std::cerr << "\n" << "Title:" << titleId << " Registered";
        registry()[titleId] = creator;
    }

    Title* TitleFactory::create(uint32_t titleId, std::string& titleVersion, platform::IPlatform &platform, platform::IInput &input) {
        std::map<uint32_t, Creator>::iterator it = registry().find(titleId);

        if (it == registry().end() || it->second == NULL) {
            std::cerr << "\n" "Could not find title in registry!";
            return NULL;
        }

        if (!it->second) {
            std::cerr << titleId << "found but creator function is null";
        }

        std::cerr << "\n" << "Title:" << titleId <<" found in registry";
        return it->second(platform, input);
    }

    std::map<uint32_t, TitleFactory::Creator> &TitleFactory::registry() {
        static std::map<uint32_t, Creator> instance;
        return instance;
    }
}
