#include "Global.h"
#include <GMLIB/Event/Entity/SpawnWanderingTraderEvent.h>
#include <mc/world/actor/ai/village/WanderingTraderScheduler.h>

namespace GMLIB::Event::EntityEvent {

BlockPos const SpawnWanderingTraderBeforeEvent::getPos() const { return mPos; }
BlockSource&   SpawnWanderingTraderBeforeEvent::getRegion() const { return mRegion; }

BlockPos const SpawnWanderingTraderAfterEvent::getPos() const { return mPos; }
BlockSource&   SpawnWanderingTraderAfterEvent::getRegion() const { return mRegion; }

LL_TYPE_INSTANCE_HOOK(
    SpawnWanderingTraderEventHook,
    ll::memory::HookPriority::Normal,
    WanderingTraderScheduler,
    &WanderingTraderScheduler::_spawnWanderingTraderAtPos,
    void,
    BlockPos const&    pos,
    class BlockSource& region
) {
    auto beforeEvent = SpawnWanderingTraderBeforeEvent(pos, region);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(pos, region);
    auto afterEvent = SpawnWanderingTraderAfterEvent(pos, region);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class SpawnWanderingTraderBeforeEventEmitter
: public ll::event::Emitter<emitterFactory1, SpawnWanderingTraderBeforeEvent> {
    ll::memory::HookRegistrar<SpawnWanderingTraderEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<SpawnWanderingTraderBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class SpawnWanderingTraderAfterEventEmitter
: public ll::event::Emitter<emitterFactory2, SpawnWanderingTraderAfterEvent> {
    ll::memory::HookRegistrar<SpawnWanderingTraderEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<SpawnWanderingTraderAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent