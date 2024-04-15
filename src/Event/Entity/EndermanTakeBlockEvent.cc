#include "Global.h"
#include <GMLIB/Event/Entity/EndermanTakeBlockEvent.h>
#include <mc/world/actor/monster/EnderMan.h>
#include <mc/world/actor/monster/EndermanTakeBlockGoal.h>

namespace GMLIB::Event::EntityEvent {

Block const& EndermanTakeBlockAfterEvent::getBlock() const { return mBlock; }

LL_TYPE_INSTANCE_HOOK(
    EndermanTakeBlockBeforeEventHook,
    ll::memory::HookPriority::Normal,
    EndermanTakeBlockGoal,
    "?canUse@EndermanTakeBlockGoal@@UEAA_NXZ",
    bool
) {
    auto enderman    = ll::memory::dAccess<EnderMan*>(this, 7);
    auto beforeEvent = EndermanTakeBlockBeforeEvent(*enderman);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }
    return origin();
}

LL_TYPE_INSTANCE_HOOK(
    EndermanTakeBlockAfterEventHook,
    ll::memory::HookPriority::Normal,
    EnderMan,
    "?setCarryingBlock@EnderMan@@QEAAXAEBVBlock@@@Z",
    void,
    class Block const& block
) {
    origin(block);
    auto afterEvent = EndermanTakeBlockAfterEvent(*this, block);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class EndermanTakeBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, EndermanTakeBlockBeforeEvent> {
    ll::memory::HookRegistrar<EndermanTakeBlockBeforeEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<EndermanTakeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class EndermanTakeAfterEventEmitter : public ll::event::Emitter<emitterFactory2, EndermanTakeBlockAfterEvent> {
    ll::memory::HookRegistrar<EndermanTakeBlockAfterEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<EndermanTakeAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent