#include "Global.h"
#include <GMLIB/Event/Entity/DragonRespawnEvent.h>
#include <mc/world/level/dimension/end/EndDragonFight.h>

namespace GMLIB::Event::EntityEvent {

GMLIB_API ActorUniqueID const DragonRespawnBeforeEvent::getEnderDragon() const { return mUniqueId; }

GMLIB_API optional_ref<EnderDragon> DragonRespawnAfterEvent::getEnderDragon() const { return mDragon; }

LL_TYPE_INSTANCE_HOOK(
    DragonRespawnEventHook,
    ll::memory::HookPriority::Normal,
    EndDragonFight,
    "?_setRespawnStage@EndDragonFight@@AEAAXW4RespawnAnimation@@@Z",
    void,
    ::RespawnAnimation stage
) {
    auto uid         = ll::memory::dAccess<ActorUniqueID>(this, 64);
    auto beforeEvent = DragonRespawnBeforeEvent(uid);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(stage);
    auto dragon = (EnderDragon*)ll::service::getLevel()->fetchEntity(uid);
    if (dragon) {
        auto afterEvent = DragonRespawnAfterEvent(dragon);
        ll::event::EventBus::getInstance().publish(afterEvent);
    }
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class DragonRespawnBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, DragonRespawnBeforeEvent> {
    ll::memory::HookRegistrar<DragonRespawnEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<DragonRespawnBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class DragonRespawnAfterEventEmitter : public ll::event::Emitter<emitterFactory2, DragonRespawnAfterEvent> {
    ll::memory::HookRegistrar<DragonRespawnEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<DragonRespawnAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent