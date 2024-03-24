#include "Global.h"
#include <GMLIB/Event/Entity/MobHurtEvent.h>

namespace GMLIB::Event::EntityEvent {

ActorDamageSource& MobHurtAfterEvent::getSource() const { return mSource; }
float&             MobHurtAfterEvent::getDamage() const { return mDamage; }

LL_TYPE_INSTANCE_HOOK(
    MobHurtEventHook,
    HookPriority::Normal,
    Mob,
    "?getDamageAfterResistanceEffect@Mob@@QEBAMAEBVActorDamageSource@@M@Z",
    float,
    ActorDamageSource& source,
    float              damage
) {
    auto result     = origin(source, damage);
    auto afterEvent = MobHurtAfterEvent(*this, source, result);
    ll::event::EventBus::getInstance().publish(afterEvent);
    return result;
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class MobHurtAfterEventEmitter : public ll::event::Emitter<emitterFactory, MobHurtAfterEvent> {
    ll::memory::HookRegistrar<MobHurtEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<MobHurtAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent