#include "Global.h"
#include <GMLIB/Event/Entity/DeathMessageEvent.h>
#include <mc/world/actor/ActorDamageByActorSource.h>
#include <mc/world/actor/ActorDamageByBlockSource.h>
#include <mc/world/actor/ActorDamageByChildActorSource.h>

namespace GMLIB::Event::EntityEvent {

using DEATH_MESSAGE = std::pair<std::string, std::vector<std::string>>;

ActorDamageSource& DeathMessageBeforeEvent::getDamageSource() const { return mDamageSource; }

ActorDamageSource& DeathMessageAfterEvent::getDamageSource() const { return mDamageSource; }
DEATH_MESSAGE&     DeathMessageAfterEvent::getDeathMessage() const { return mDeathMessage; }

LL_TYPE_INSTANCE_HOOK(
    DeathMessageEvent1,
    HookPriority::Normal,
    ActorDamageSource,
    "?getDeathMessage@ActorDamageSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?"
    "$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_"
    "traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto beforeEvent = DeathMessageBeforeEvent(*a2, *this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return DEATH_MESSAGE{};
    }
    auto result     = origin(a1, a2);
    auto afterEvent = DeathMessageAfterEvent(*a2, *this, result);
    ll::event::EventBus::getInstance().publish(afterEvent);
    result = afterEvent.getDeathMessage();
    return result;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageEvent2,
    HookPriority::Normal,
    ActorDamageByActorSource,
    "?getDeathMessage@ActorDamageByActorSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@"
    "std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$"
    "char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto beforeEvent = DeathMessageBeforeEvent(*a2, *this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return DEATH_MESSAGE{};
    }
    auto result     = origin(a1, a2);
    auto afterEvent = DeathMessageAfterEvent(*a2, *this, result);
    ll::event::EventBus::getInstance().publish(afterEvent);
    result = afterEvent.getDeathMessage();
    return result;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageEvent3,
    HookPriority::Normal,
    ActorDamageByBlockSource,
    "?getDeathMessage@ActorDamageByBlockSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@"
    "std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$"
    "char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto beforeEvent = DeathMessageBeforeEvent(*a2, *this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return DEATH_MESSAGE{};
    }
    auto result     = origin(a1, a2);
    auto afterEvent = DeathMessageAfterEvent(*a2, *this, result);
    ll::event::EventBus::getInstance().publish(afterEvent);
    result = afterEvent.getDeathMessage();
    return result;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageEvent4,
    HookPriority::Normal,
    ActorDamageByChildActorSource,
    "?getDeathMessage@ActorDamageByChildActorSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@"
    "D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?"
    "$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto beforeEvent = DeathMessageBeforeEvent(*a2, *this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return DEATH_MESSAGE{};
    }
    auto result     = origin(a1, a2);
    auto afterEvent = DeathMessageAfterEvent(*a2, *this, result);
    ll::event::EventBus::getInstance().publish(afterEvent);
    result = afterEvent.getDeathMessage();
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class DeathMessageBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, DeathMessageBeforeEvent> {
    ll::memory::HookRegistrar<DeathMessageEvent1, DeathMessageEvent2, DeathMessageEvent3, DeathMessageEvent4> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<DeathMessageBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class DeathMessageAfterEventEmitter : public ll::event::Emitter<emitterFactory2, DeathMessageAfterEvent> {
    ll::memory::HookRegistrar<DeathMessageEvent1, DeathMessageEvent2, DeathMessageEvent3, DeathMessageEvent4> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<DeathMessageAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent