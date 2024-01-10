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

} // namespace GMLIB::ActorAPI