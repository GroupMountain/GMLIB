#include "Global.h"
#include <GMLIB/Event/Registries/JsonItemInitEvent.h>
#include <mc/deps/cereal/ReflectionCtx.h>
#include <mc/deps/core/Path.h>
#include <mc/deps/json/Value.h>
#include <mc/resources/ResourcePackManager.h>
#include <mc/world/item/components/ItemVersion.h>
#include <mc/world/item/registry/ItemRegistry.h>
#include <mc/world/level/storage/Experiments.h>

namespace GMLIB::Event::Registries {

ItemRegistry&      JsonItemInitEvent::getRegistry() const { return mRegistry; }
RegistryCall&      JsonItemInitEvent::getRegistryCall() const { return mCall; }
Experiments const& JsonItemInitEvent::getExperiments() const { return mExperiments; }
::ItemVersion&     JsonItemInitEvent::getItemVersion() const { return mVersion; }

LL_TYPE_INSTANCE_HOOK(
    JsonItemInitEventHook,
    HookPriority::Normal,
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
    auto event = JsonItemInitEvent(*this, call, exp, ver);
    ll::event::EventBus::getInstance().publish(event);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class JsonItemInitEventEmitter : public ll::event::Emitter<emitterFactory, JsonItemInitEvent> {
    ll::memory::HookRegistrar<JsonItemInitEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<JsonItemInitEventEmitter>();
}

} // namespace GMLIB::Event::Registries