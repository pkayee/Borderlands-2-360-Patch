#include "TitleFactory.h"

namespace core {
    void TitleFactory::registerTitle(uint32_t titleId, Creator creator) {
        registry()[titleId] = std::move(creator);
    }

    std::unique_ptr<Title> TitleFactory::create(uint32_t titleId, uint32_t titleVersion, platform::IPlatform &platform, platform::IInput &input) {
        auto &reg = registry();
        auto it = reg.find(titleId);
        if (it == reg.end()) {
            return nullptr;
        }
        return it->second(platform, input);
    }

    std::unordered_map<uint32_t, TitleFactory::Creator> &TitleFactory::registry() {
        static std::unordered_map<uint32_t, Creator> instance;
        return instance;
    }
}
