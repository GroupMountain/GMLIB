#include "Global.h"
#include <GMLIB/Event/entity/MobPickupItemEvent.h>

namespace GMLIB::Event::EntityEvent {

ItemActor const& MobPickupItemBeforeEvent::getItemActor() const { return mItemActor; }

ItemActor const& MobPickupItemAfterEvent::getItemActor() const { return mItemActor; }

LL_TYPE_INSTANCE_HOOK(
    MobPickupItemEventHook,
    ll::memory::HookPriority::Normal,
    PickupItemsGoal,
    "?_pickItemUp@PickupItemsGoal@@AEAAXPEAVItemActor@@@Z",
    void,
    class ItemActor& item
) {
    auto mob         = ll::memory::dAccess<Mob*>(this, 112); // IDA: PickupItemsGoal::PickupItemsGoal()
    auto beforeEvent = MobPickupItemBeforeEvent(*mob, item);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(item);
    auto afterEvent = MobPickupItemAfterEvent(*mob, item);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class MobPickupItemBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, MobPickupItemBeforeEvent> {
    ll::memory::HookRegistrar<MobPickupItemEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<MobPickupItemBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class MobPickupItemAfterEventEmitter : public ll::event::Emitter<emitterFactory2, MobPickupItemAfterEvent> {
    ll::memory::HookRegistrar<MobPickupItemEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<MobPickupItemAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent
