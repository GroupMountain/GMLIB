#include "Global.h"
#include <GMLIB/Event/Entity/ActorChangeDImensionEvent.h>

namespace GMLIB::Event::EntityEvent {

DimensionType const ActorChangeDimensionBeforeEvent::getFromDimensionId() const { return mFromDimensionType; }
DimensionType&      ActorChangeDimensionBeforeEvent::getToDimensionId() const { return mToDimensionType; }

DimensionType const ActorChangeDimensionAfterEvent::getFromDimensionId() const { return mFromDimensionType; }
DimensionType&      ActorChangeDimensionAfterEvent::getToDimensionId() const { return mToDimensionType; }

LL_TYPE_INSTANCE_HOOK(
    ActorChangeDimensionEventHook,
    ll::memory::HookPriority::Normal,
    Level,
    &Level::entityChangeDimension,
    void,
    class Actor&              entity,
    DimensionType             toId,
    std::optional<class Vec3> entityPos
) {
    auto formId      = entity.getDimensionId();
    auto beforeEvent = ActorChangeDimensionBeforeEvent(entity, formId, toId);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(entity, toId, entityPos);
    auto afterEvent = ActorChangeDimensionAfterEvent(entity, formId, toId);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ActorChangeDimensionBeforeEventEmitter
: public ll::event::Emitter<emitterFactory1, ActorChangeDimensionBeforeEvent> {
    ll::memory::HookRegistrar<ActorChangeDimensionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ActorChangeDimensionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ActorChangeDimensionAfterEventEmitter
: public ll::event::Emitter<emitterFactory2, ActorChangeDimensionAfterEvent> {
    ll::memory::HookRegistrar<ActorChangeDimensionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ActorChangeDimensionAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent