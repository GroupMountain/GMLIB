#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <corecrt_math_defines.h>

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

GMLIB_API void addEffect(
    Actor*                actor,
    MobEffect::EffectType effectType,
    int                   duration,
    int                   amplifier,
    bool                  showParticles,
    bool                  ambient,
    bool                  showAnimation
) {
    auto effect = MobEffectInstance((uint)effectType, duration, amplifier, ambient, showParticles, showAnimation);
    actor->addEffect(effect);
}

GMLIB_API void removeEffect(Actor* actor, MobEffect::EffectType effectType) { actor->removeEffect((int)effectType); }

GMLIB_API std::vector<MobEffectInstance> getAllEffects(Actor* actor) {
    std::vector<MobEffectInstance> result = {};
    for (int i = 0; i <= 30; i++) {
        auto effect = actor->getEffect(i);
        if (effect) {
            result.emplace_back(*effect);
        }
    }
    return result;
}

GMLIB_API void removeAllEffects(Actor* actor) { return actor->removeAllEffects(); }

GMLIB_API bool setProjectile(Actor* owner, Actor* proj, float speed, float offset) {
    if (owner && proj) {
        try {
            proj->setOwner(owner->getOrCreateUniqueID());
            proj->teleport(owner->getPosition(), owner->getDimensionId());
            if (speed > 0) {
                auto berot = owner->getRotation();
                if (offset > 0) { // 偏移
                    // 初始化随机数生成器
                    std::srand(static_cast<unsigned>(std::time(nullptr)));
                    double x_offset = (std::rand() % 200 - 100) / 100.0 * offset; // 在-1到1之间生成随机偏移
                    double y_offset = (std::rand() % 200 - 100) / 100.0 * offset;
                    // 调整角度
                    berot.x += x_offset;
                    berot.y += y_offset;
                }
                auto dx = -speed * sin(berot.y * M_PI / 180) * cos(berot.x * M_PI / 180);
                auto dz = speed * cos(berot.y * M_PI / 180) * cos(berot.x * M_PI / 180);
                auto dy = speed * sin(berot.x * M_PI / 180);
                proj->setVelocity({dx, -dy, dz});
            }
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

GMLIB_API bool throwEntity(Actor* owner, Actor* projectile, float speed, float offset) {
    if (projectile->hasCategory(ActorCategory::Player)) {
        return false;
    }
    if (speed > 0) {
        return setProjectile(owner, projectile, speed, offset);
    }
    return false;
}

} // namespace GMLIB::ActorAPI
