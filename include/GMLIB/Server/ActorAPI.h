#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/effect/MobEffect.h"

class GMLIB_Actor : public Actor {
public:
    using Actor::addEffect;
    using Actor::getAllEffects;
    using Actor::removeEffect;

public:
    GMLIB_API static int64_t getNextActorUniqueID();

public:
    GMLIB_API bool isPlayer() const;

    GMLIB_API bool isItemActor() const;

    GMLIB_API bool isMob() const;

    GMLIB_API std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API void addEffect(
        MobEffect::EffectType effectType,
        int                   duration      = 600,
        int                   amplifier     = 0,
        bool                  showParticles = true,
        bool                  ambient       = false,
        bool                  showAnimation = false
    );

    GMLIB_API void removeEffect(MobEffect::EffectType effectType);

    GMLIB_API std::vector<MobEffectInstance> getAllEffects();

    GMLIB_API bool setProjectile(Actor* projectile, float speed = 2, float offset = 3);

    GMLIB_API GMLIB_Actor* shootProjectile(std::string typeName, float speed = 2, float offset = 3);

    GMLIB_API bool throwEntity(Actor* projectile, float speed = 2, float offset = 3);

    GMLIB_API void setHealth(int value);

    GMLIB_API void setMaxHealth(int value);

    GMLIB_API void setAttackDamage(int value);

    GMLIB_API void setFollowRange(int value);

    GMLIB_API void setMaxFollowRange(int value);

    GMLIB_API void setJumpStrength(int value);

    GMLIB_API void setKnockbackResistance(int value);

    GMLIB_API void setLavaMovementSpeed(int value);

    GMLIB_API void setMovementSpeed(int value);

    GMLIB_API void setUnderwaterMovementSpeed(int value);

    GMLIB_API ItemStack* getMainHandSlot();

    GMLIB_API void setMainHandSlot(ItemStack& itemStack);

    GMLIB_API ItemStack* getOffHandSlot();

    GMLIB_API void setOffHandSlot(ItemStack& itemStack);
};