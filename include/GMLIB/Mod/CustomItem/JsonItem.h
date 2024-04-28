#pragma once
#include "GMLIB/Macros.h"
#include "mc/world/item/components/ItemVersion.h"
#include "mc/world/item/registry/ItemRegistry.h"
#include "mc/world/level/storage/Experiments.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

using RegistryCall = std::function<
    void(class WeakPtr<class Item>&, class Json::Value&, class SemVersion const&, bool, class Experiments const&)>;

class JsonItem {
public:
    GMLIB_API static void loadJsonItem(std::string const& json);

    GMLIB_API static void loadJsonItem(nlohmann::json const& json);

    GMLIB_API static WeakPtr<class Item> loadJsonItem(
        std::string const& typeId,
        std::string const& json,
        ItemRegistry&      registry,
        RegistryCall       call,
        Experiments const& experiments,
        ::ItemVersion      version
    );

    GMLIB_API static WeakPtr<class Item> loadJsonItem(
        std::string const&    typeId,
        nlohmann::json const& json,
        ItemRegistry&         registry,
        RegistryCall          call,
        Experiments const&    experiments,
        ::ItemVersion         version
    );
};

} // namespace GMLIB::Mod