#include "Global.h"
#include <GMLIB/Event/Entity/ProjectileCreateEvent.h>
#include <mc/entity/components/ProjectileComponent.h>

namespace GMLIB::Event::EntityEvent {

optional_ref<Actor> ProjectileCreateBeforeEvent::getShooter() const { return mShooter; }

optional_ref<Actor> ProjectileCreateAfterEvent::getShooter() const { return mShooter; }

LL_TYPE_INSTANCE_HOOK(
    ProjectileCreateEventHook1,
    ll::memory::HookPriority::Normal,
    ProjectileComponent,
    "?shoot@ProjectileComponent@@QEAAXAEAVActor@@AEBVVec3@@MM1PEAV2@@Z",
    void,
    class Actor&      projectile,
    class Vec3 const& direction,
    float             power,
    float             offset,
    class Vec3 const& baseSpeed,
    class Actor*      target
) {
    if (!target) {
        return origin(projectile, direction, power, offset, baseSpeed, target);
    }
    auto owner       = projectile.getOwner();
    auto beforeEvent = ProjectileCreateBeforeEvent(projectile, owner);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        projectile.remove();
        return;
    }
    origin(projectile, direction, power, offset, baseSpeed, target);
    auto afterEvent = ProjectileCreateAfterEvent(projectile, owner);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

LL_TYPE_INSTANCE_HOOK(
    ProjectileCreateEventHook2,
    ll::memory::HookPriority::Normal,
    ProjectileComponent,
    "?shoot@ProjectileComponent@@QEAAXAEAVActor@@0@Z",
    void,
    class Actor& projectile,
    class Actor& target
) {
    auto owner       = projectile.getOwner();
    auto beforeEvent = ProjectileCreateBeforeEvent(projectile, owner);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        projectile.remove();
        return;
    }
    origin(projectile, target);
    auto afterEvent = ProjectileCreateAfterEvent(projectile, owner);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ProjectileCreateBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, ProjectileCreateBeforeEvent> {
    ll::memory::HookRegistrar<ProjectileCreateEventHook1, ProjectileCreateEventHook2> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ProjectileCreateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ProjectileCreateAfterEventEmitter : public ll::event::Emitter<emitterFactory2, ProjectileCreateAfterEvent> {
    ll::memory::HookRegistrar<ProjectileCreateEventHook1, ProjectileCreateEventHook2> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ProjectileCreateAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent
