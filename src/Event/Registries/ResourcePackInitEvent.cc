#include "Global.h"
#include <GMLIB/Event/Registries/ResourcePackInitEvent.h>

namespace GMLIB::Event::Registries {

ResourcePackRepository& ResourcePackInitEvent::getResourcePackRepository() const { return mRepo; }

LL_TYPE_INSTANCE_HOOK(
    ResourcePackInitEventHook,
    ll::memory::HookPriority::Normal,
    ResourcePackRepository,
    "?_initialize@ResourcePackRepository@@AEAAXXZ",
    void
) {
    auto event = ResourcePackInitEvent(*this);
    ll::event::EventBus::getInstance().publish(event);
    return origin();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class ResourcePackInitEventEmitter : public ll::event::Emitter<emitterFactory, ResourcePackInitEvent> {
    ll::memory::HookRegistrar<ResourcePackInitEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<ResourcePackInitEventEmitter>();
}

} // namespace GMLIB::Event::Registries