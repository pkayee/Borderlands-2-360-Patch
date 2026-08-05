#pragma once
#include "cstdint"
#include "memory"
#include "function.h"
#include <platform/IPlatform.h>
#include <platform/IInput.h>

#include "functional"
#include "Title.h"
#include "unordered_map"

namespace core {
    class TitleFactory {
    public:
        using Creator = std::function<std::unique_ptr<Title>(platform::IPlatform&, platform::IInput&)>;
        static void registerTitle(uint32_t titleId, Creator creator);
        static std::unique_ptr<Title>  create(uint32_t titleId, uint32_t titleVersion, platform::IPlatform &platform, platform::IInput &input);

    private:
        static std::unordered_map<uint32_t, Creator>& registry();
    };
}
