#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/item/components/ItemVersion.h"
#include "mc/world/item/registry/ItemRegistry.h"
#include "mc/world/level/storage/Experiments.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

using RegistryCall = std::function<
    void(class WeakPtr<class Item>&, class Json::Value&, class SemVersion const&, bool, class Experiments const&)>;

class JsonItem {
public:
    GMLIB_API static void loadJsonItem(std::string json);

    GMLIB_API static void loadJsonItem(nlohmann::json json);

    GMLIB_API static WeakPtr<class Item> loadJsonItem(
        std::string        typeId,
        std::string        json,
        ItemRegistry&      registry,
        RegistryCall       call,
        Experiments const& experiments,
        ::ItemVersion      version
    );

    GMLIB_API static WeakPtr<class Item> loadJsonItem(
        std::string        typeId,
        nlohmann::json     json,
        ItemRegistry&      registry,
        RegistryCall       call,
        Experiments const& experiments,
        ::ItemVersion      version
    );
};

} // namespace GMLIB::Mod