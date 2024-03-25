#include "Global.h"
#include <GMLIB/Event/Entity/ProjectileCreateEvent.h>
#include <mc/entity/components/ProjectileComponent.h>

/*
LL_AUTO_TYPE_INSTANCE_HOOK(
    EntityChangeDimensionEventHook,
    ll::memory::HookPriority::Normal,
    Level,
    &Level::entityChangeDimension,
    void,
    class Actor&              entity,
    DimensionType             toId,
    std::optional<class Vec3> entityPos
) {
    logger.warn("Before {} | {}", entity.getTypeName(), toId.id);
    // origin(entity, toId, entityPos);
    // logger.warn("After {} | {}", entity.getTypeName(), toId.id);
}


LL_AUTO_TYPE_INSTANCE_HOOK(
    ProjectileCreateEventHook1,
    ll::memory::HookPriority::Normal,
    ProjectileComponent,
    "?shoot@ProjectileComponent@@QEAAXAEAVActor@@AEBVVec3@@MM1PEAV2@@Z",
    void,
    class Actor&      owner,
    class Vec3 const& dir,
    float             pow,
    float             uncertainty,
    class Vec3 const& baseSpeed,
    class Actor*      target
) {
    if (!target) {
        return origin(owner, dir, pow, uncertainty, baseSpeed, target);
    }
    logger.warn("Shoot1 Before {}", owner.getTypeName());
    owner.remove();
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ProjectileCreateEventHook2,
    ll::memory::HookPriority::Normal,
    ProjectileComponent,
    "?shoot@ProjectileComponent@@QEAAXAEAVActor@@0@Z",
    void,
    class Actor& owner,
    class Actor& shooter
) {
    logger.warn(
        "Shoot2 Before {} | {}",
        &owner ? owner.getTypeName() : "null",
        &shooter ? shooter.getTypeName() : "null" // first
    );
    owner.remove();
}
*/
