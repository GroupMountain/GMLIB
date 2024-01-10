#pragma once
#include "GMLIB/GMLIB.h"
#include "Global.h"

namespace GMLIB::ActorAPI {

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Actor* ac);
GMLIB_API bool                         setNbt(Actor* ac, CompoundTag* nbt);

GMLIB_API void setHealth(Actor* actor, int value);
GMLIB_API void setMaxHealth(Actor* actor, int value);
GMLIB_API void setAttackDamage(Actor* actor, int value);
GMLIB_API void setFollowRange(Actor* actor, int value);
GMLIB_API void setMaxFollowRange(Actor* actor, int value);
GMLIB_API void setJumpStrength(Actor* actor, int value);
GMLIB_API void setKnockbackResistance(Actor* actor, int value);
GMLIB_API void setLavaMovementSpeed(Actor* actor, int value);
GMLIB_API void setMovementSpeed(Actor* actor, int value);
GMLIB_API void setUnderwaterMovementSpeed(Actor* actor, int value);

GMLIB_API void addEffect(
    Actor*                actor,
    MobEffect::EffectType effectType,
    int                   duration      = 600,
    int                   amplifier     = 0,
    bool                  showParticles = true,
    bool                  ambient       = false,
    bool                  showAnimation = false
);
GMLIB_API void removeEffect(Actor* actor, MobEffect::EffectType effectType);
GMLIB_API void removeAllEffects(Actor* actor);

GMLIB_API std::vector<MobEffectInstance> getAllEffects(Actor* actor);


} // namespace GMLIB::ActorAPI