#include "Global.h"
#include <GMLIB/Mod/CustomItem/JsonItem.h>
#include <mc/deps/cereal/ReflectionCtx.h>
#include <mc/deps/core/Path.h>
#include <mc/deps/json/Value.h>
#include <mc/resources/ResourcePackManager.h>
#include <mc/world/item/components/ItemVersion.h>
#include <mc/world/item/registry/ItemRegistry.h>
#include <mc/world/level/storage/Experiments.h>

namespace GMLIB::Mod {

std::vector<std::string> mAllItemJson;

void JsonItem::loadJsonItem(std::string json) { mAllItemJson.push_back(json); }

void JsonItem::loadJsonItem(nlohmann::json json) {
    auto value = json.dump();
    mAllItemJson.push_back(value);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    LoadJSonItemHook,
    ll::memory::HookPriority::Normal,
    ItemRegistry,
    "?_loadItemData@ItemRegistry@@AEAAXAEAVResourcePackManager@@V?$function@$$A6AXAEAV?$WeakPtr@VItem@@@@AEAVValue@"
    "Json@@AEBVSemVersion@@_NAEBVExperiments@@@Z@std@@AEBVExperiments@@W4ItemVersion@@@Z",
    void,
    class ResourcePackManager& rpm,
    std::function<
        void(class WeakPtr<class Item>&, class Json::Value&, class SemVersion const&, bool, class Experiments const&)>
                             call,
    class Experiments const& exp,
    ::ItemVersion            ver
) {
    origin(rpm, call, exp, ver);
    for (auto& json : mAllItemJson) {
        _parseItemDefinition(
            json,
            true,
            call,
            exp,
            ver,
            PackType::Behavior,
            Core::Path::EMPTY,
            cereal::ReflectionCtx::global()
        );
    }
}

WeakPtr<class Item> JsonItem::loadJsonItem(
    std::string        typeId,
    std::string        json,
    ItemRegistry&      registry,
    RegistryCall       call,
    Experiments const& experiments,
    ::ItemVersion      version
) {
    registry._parseItemDefinition(
        json,
        true,
        call,
        experiments,
        version,
        PackType::Behavior,
        Core::Path::EMPTY,
        cereal::ReflectionCtx::global()
    );
    return registry.lookupByName(HashedString(typeId));
}

WeakPtr<class Item> JsonItem::loadJsonItem(
    std::string        typeId,
    nlohmann::json     json,
    ItemRegistry&      registry,
    RegistryCall       call,
    Experiments const& experiments,
    ::ItemVersion      version
) {
    registry._parseItemDefinition(
        json.dump(),
        true,
        call,
        experiments,
        version,
        PackType::Behavior,
        Core::Path::EMPTY,
        cereal::ReflectionCtx::global()
    );
    return registry.lookupByName(HashedString(typeId));
}

} // namespace GMLIB::Mod