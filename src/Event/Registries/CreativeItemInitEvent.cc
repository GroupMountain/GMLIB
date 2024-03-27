#include "Global.h"
#include <GMLIB/Event/Registries/CreativeItemsInitEvent.h>

namespace GMLIB::Event::Registries {

ItemRegistryRef& CreativeItemInitEvent::getItemRegistryRef() const { return mRef; }

LL_STATIC_HOOK(
    CreativeItemInitEventHook,
    ll::memory::HookPriority::Normal,
    "?addLooseCreativeItems@Item@@SAX_NAEBVBaseGameVersion@@VItemRegistryRef@@@Z",
    void,
    bool                         a1,
    class BaseGameVersion const& a2,
    class ItemRegistryRef&       a3
) {
    auto event = CreativeItemInitEvent(a3);
    ll::event::EventBus::getInstance().publish(event);
    return origin(a1, a2, a3);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class CreativeItemInitEventEmitter : public ll::event::Emitter<emitterFactory, CreativeItemInitEvent> {
    ll::memory::HookRegistrar<CreativeItemInitEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<CreativeItemInitEventEmitter>();
}

} // namespace GMLIB::Event::Registries