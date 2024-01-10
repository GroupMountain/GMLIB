#include "GMLIB/Server/ActorAPI.h"
#include "GMLIB/Server/CompoundTagAPI.h"
#include "Global.h"

namespace GMLIB::ActorAPI {

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Actor* ac) { return ac->save(); }

GMLIB_API bool setNbt(Actor* ac, CompoundTag* nbt) { return ac->load(*nbt); }

GMLIB_API void setHealth(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::HEALTH)->setCurrentValue(value);
}

GMLIB_API void setMaxHealth(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::HEALTH)->setMaxValue(value);
}

GMLIB_API void setAttackDamage(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setCurrentValue(value);
}

GMLIB_API void setFollowRange(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setCurrentValue(value);
}

GMLIB_API void setMaxFollowRange(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setMaxValue(value);
}

GMLIB_API void setJumpStrength(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setCurrentValue(value);
}

GMLIB_API void setKnockbackResistance(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)->setCurrentValue(value);
}

GMLIB_API void setLavaMovementSpeed(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)->setCurrentValue(value);
}

GMLIB_API void setMovementSpeed(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)->setCurrentValue(value);
}

GMLIB_API void setUnderwaterMovementSpeed(Actor* actor, int value) {
    actor->getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->setCurrentValue(value);
}

} // namespace GMLIB::ActorAPI
