#include "Global.h"
#include <GMLIB/Mod/CustomDamageCause.h>
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <corecrt_math_defines.h>
#include <mc/math/Vec2.h>
#include <mc/util/Random.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>
#include <mc/world/effect/MobEffect.h>
#include <mc/world/effect/MobEffectInstance.h>

bool GMLIB_Actor::isPlayer() const { return hasCategory(ActorCategory::Player); }

bool GMLIB_Actor::isItemActor() const { return hasCategory(ActorCategory::Item); }

bool GMLIB_Actor::isMob() const { return hasCategory(ActorCategory::Mob); }

std::unique_ptr<CompoundTag> GMLIB_Actor::getNbt() {
    auto nbt = std::make_unique<CompoundTag>();
    save(*nbt);
    return std::move(nbt);
}

bool GMLIB_Actor::setNbt(CompoundTag& nbt) { return load(nbt); }

void GMLIB_Actor::setHealth(int value) { return getMutableAttribute(SharedAttributes::HEALTH)->setCurrentValue(value); }

void GMLIB_Actor::setMaxHealth(int value) { return getMutableAttribute(SharedAttributes::HEALTH)->setMaxValue(value); }

void GMLIB_Actor::setAttackDamage(int value) {
    return getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setCurrentValue(value);
}

void GMLIB_Actor::setFollowRange(int value) {
    return getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setCurrentValue(value);
}

void GMLIB_Actor::setMaxFollowRange(int value) {
    return getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setMaxValue(value);
}

void GMLIB_Actor::setJumpStrength(int value) {
    return getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setCurrentValue(value);
}

void GMLIB_Actor::setKnockbackResistance(int value) {
    return getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)->setCurrentValue(value);
}

void GMLIB_Actor::setLavaMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Actor::setMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Actor::setUnderwaterMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Actor::addEffect(
    MobEffect::EffectType effectType,
    int                   duration,
    int                   amplifier,
    bool                  showParticles,
    bool                  ambient,
    bool                  showAnimation
) {
    auto effect = MobEffectInstance((uint)effectType, duration, amplifier, ambient, showParticles, showAnimation);
    return addEffect(effect);
}

void GMLIB_Actor::removeEffect(MobEffect::EffectType effectType) { return removeEffect((int)effectType); }

std::vector<MobEffectInstance> GMLIB_Actor::getAllEffects() {
    std::vector<MobEffectInstance> result = {};
    for (int i = 0; i <= 30; i++) {
        auto effect = getEffect(i);
        if (effect) {
            result.emplace_back(*effect);
        }
    }
    return result;
}

bool GMLIB_Actor::setProjectile(Actor* projectile, float speed, float offset) {
    if (this && projectile) {
        try {
            projectile->setOwner(getOrCreateUniqueID());
            projectile->teleport(getPosition(), getDimensionId());
            if (speed > 0) {
                auto berot = getRotation();
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
                projectile->setVelocity({dx, -dy, dz});
            }
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool GMLIB_Actor::throwEntity(Actor* projectile, float speed, float offset) {
    if (projectile->hasCategory(ActorCategory::Player)) {
        return false;
    }
    if (speed > 0) {
        return setProjectile(projectile, speed, offset);
    }
    return false;
}

ItemStack* GMLIB_Actor::getMainHandSlot() { return (ItemStack*)&getEquippedSlot(Puv::Legacy::EquipmentSlot::Mainhand); }

void GMLIB_Actor::setMainHandSlot(ItemStack& itemStack) {
    return setEquippedSlot(Puv::Legacy::EquipmentSlot::Mainhand, itemStack);
}

ItemStack* GMLIB_Actor::getOffHandSlot() { return (ItemStack*)&getEquippedSlot(Puv::Legacy::EquipmentSlot::Offhand); }

void GMLIB_Actor::setOffHandSlot(ItemStack& itemStack) {
    return setEquippedSlot(Puv::Legacy::EquipmentSlot::Offhand, itemStack);
}

GMLIB_Actor* GMLIB_Actor::shootProjectile(std::string typeName, float speed, float offset) {
    return GMLIB_Spawner::spawnProjectile((GMLIB_Actor*)this, typeName, speed, offset);
}

int64_t GMLIB_Actor::getNextActorUniqueID() {
    auto res = Random::getThreadLocal().nextLong();
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(res)) != nullptr) {
        res = Random::getThreadLocal().nextLong();
    }
    return res;
}

void GMLIB_Actor::hurtEntity(float damage, std::string causeName, Actor* source) {
    auto cause = GMLIB::Mod::DamageCause::getCauseFromName(causeName);
    this->hurtByCause(damage, cause, source);
}

Biome* GMLIB_Actor::getBiome() {
    auto& bs = getDimensionBlockSourceConst();
    return const_cast<Biome*>(&bs.getConstBiome(getFeetBlockPos()));
}