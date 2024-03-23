#include "Global.h"
#include <GMLIB/Mod/Addons/CustomDamageCause.h>
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <corecrt_math_defines.h>
#include <mc/math/Vec2.h>
#include <mc/util/Random.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>
#include <mc/world/effect/MobEffect.h>
#include <mc/world/effect/MobEffectInstance.h>
#include <mc/world/level/storage/DBStorage.h>

std::unordered_map<ActorUniqueID, std::string> GMLIB_Actor::getActorIdsKeyMap() {
    std::unordered_map<ActorUniqueID, std::string> result;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "actorprefix",
        DBHelpers::Category::Actor,
        [&result](std::string_view key_left, std::string_view data) {
            auto keyId   = "actorprefix" + std::string(key_left);
            auto keyData = CompoundTag::fromBinaryNbt(data);
            if (keyData && keyData->contains("UniqueID")) {
                auto strAuid = keyData->getInt64("UniqueID");
                auto auid    = ActorUniqueID(strAuid);
                result[auid] = keyId;
            } else {
                GMLIB::Global<DBStorage>->deleteData(keyId, DBHelpers::Category::Actor);
            }
        }
    );
    return result;
}

std::vector<ActorUniqueID> GMLIB_Actor::getAllEntities() {
    std::vector<ActorUniqueID> result;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "actorprefix",
        DBHelpers::Category::Actor,
        [&result](std::string_view key_left, std::string_view data) {
            auto keyId   = "actorprefix" + std::string(key_left);
            auto keyData = CompoundTag::fromBinaryNbt(data);
            if (keyData && keyData->contains("UniqueID")) {
                auto strAuid = keyData->getInt64("UniqueID");
                auto auid    = ActorUniqueID(strAuid);
                result.push_back(auid);
            } else {
                GMLIB::Global<DBStorage>->deleteData(keyId, DBHelpers::Category::Actor);
            }
        }
    );
    return result;
}

std::unique_ptr<CompoundTag> getUnloadedActorNbt(std::string& actorKey) {
    if (actorKey.empty()) {
        return nullptr;
    }
    if (!GMLIB::Global<DBStorage>->hasKey(actorKey, DBHelpers::Category::Actor)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(actorKey, DBHelpers::Category::Actor);
}

bool setUnloadedActorNbt(std::string& actorKey, CompoundTag& nbt) {
    try {
        if (actorKey.empty()) {
            return false;
        }
        GMLIB::Global<DBStorage>->saveData(actorKey, nbt.toBinaryNbt(), DBHelpers::Category::Actor);
        return true;
    } catch (...) {
        return false;
    }
}

ActorUniqueID GMLIB_Actor::getActorUniqueID(std::string& actorKey) {
    auto nbt = getUnloadedActorNbt(actorKey);
    if (nbt && nbt->contains("UniqueID")) {
        auto auid = nbt->getInt64("UniqueID");
        return ActorUniqueID(auid);
    }
    return ActorUniqueID::INVALID_ID;
}

std::string getActorKeyFromUniqueId(ActorUniqueID& auid) {
    auto map = GMLIB_Actor::getActorIdsKeyMap();
    if (map.count(auid)) {
        return map[auid];
    }
    return "";
}

std::string GMLIB_Actor::getActorTypeName(ActorUniqueID& auid) {
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        return en->getTypeName();
    }
    auto actorKey = getActorKeyFromUniqueId(auid);
    return getActorTypeName(actorKey);
}

std::string GMLIB_Actor::getActorTypeName(std::string& actorKey) {
    auto nbt = getUnloadedActorNbt(actorKey);
    if (nbt && nbt->contains("identifier")) {
        auto type = nbt->getString("identifier");
        return type;
    }
    return "unknown";
}

std::unique_ptr<CompoundTag> GMLIB_Actor::getActorNbt(ActorUniqueID& auid) {
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->getNbt();
    }
    auto actorKey = getActorKeyFromUniqueId(auid);
    return getActorNbt(actorKey);
}

std::unique_ptr<CompoundTag> GMLIB_Actor::getActorNbt(std::string& actorKey) {
    auto auid = getActorUniqueID(actorKey);
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->getNbt();
    }
    return getUnloadedActorNbt(actorKey);
}

bool GMLIB_Actor::setActorNbt(ActorUniqueID& auid, CompoundTag& nbt) {
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbt(nbt);
    }
    auto actorKey = getActorKeyFromUniqueId(auid);
    return setActorNbt(actorKey, nbt);
}

bool GMLIB_Actor::setActorNbt(std::string& actorKey, CompoundTag& nbt) {
    auto auid = getActorUniqueID(actorKey);
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbt(nbt);
    }
    return setUnloadedActorNbt(actorKey, nbt);
}

bool GMLIB_Actor::setActorNbtTags(ActorUniqueID& auid, CompoundTag& nbt, const std::vector<std::string>& tags) {
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbtTags(nbt, tags);
    }
    auto actorKey = getActorKeyFromUniqueId(auid);
    return setActorNbtTags(actorKey, nbt, tags);
}

bool GMLIB_Actor::setActorNbtTags(std::string& actorKey, CompoundTag& nbt, const std::vector<std::string>& tags) {
    if (actorKey.empty()) {
        return false;
    }
    auto auid = getActorUniqueID(actorKey);
    if (auto actor = (GMLIB_Actor*)ll::service::bedrock::getLevel()->fetchEntity(auid)) {
        return actor->setNbtTags(nbt, tags);
    }
    auto data = *getUnloadedActorNbt(actorKey);
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    return setUnloadedActorNbt(actorKey, data);
}

bool GMLIB_Actor::deleteActor(ActorUniqueID& auid) {
    if (auto en = ll::service::getLevel()->fetchEntity(auid)) {
        en->remove();
        return true;
    }
    auto actorKey = getActorKeyFromUniqueId(auid);
    return deleteActor(actorKey);
}

bool GMLIB_Actor::deleteActor(std::string& actorKey) {
    if (actorKey.empty()) {
        return false;
    }
    auto auid = getActorUniqueID(actorKey);
    GMLIB_Scoreboard::getInstance()->resetScore(auid);
    if (auto actor = (GMLIB_Actor*)ll::service::bedrock::getLevel()->fetchEntity(auid)) {
        return false;
    }
    if (GMLIB::Global<DBStorage>->hasKey(actorKey, DBHelpers::Category::Actor)) {
        GMLIB::Global<DBStorage>->deleteData(actorKey, DBHelpers::Category::Actor);
        return true;
    }
    return false;
}

std::optional<int> GMLIB_Actor::getActorScore(ActorUniqueID& auid, std::string objective) {
    return GMLIB_Scoreboard::getInstance()->getScore(objective, auid);
}

std::optional<int> GMLIB_Actor::getActorScore(std::string& actorKey, std::string objective) {
    auto auid = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->getScore(objective, auid);
}

std::optional<int>
GMLIB_Actor::setActorScore(ActorUniqueID& auid, std::string objective, int value, PlayerScoreSetFunction action) {
    return GMLIB_Scoreboard::getInstance()->setScore(objective, auid, value, action);
}

std::optional<int>
GMLIB_Actor::setActorScore(std::string& actorKey, std::string objective, int value, PlayerScoreSetFunction action) {
    auto auid = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->setScore(objective, auid, value, action);
}

bool GMLIB_Actor::resetActorScore(ActorUniqueID& auid, std::string objective) {
    return GMLIB_Scoreboard::getInstance()->resetScore(objective, auid);
}

bool GMLIB_Actor::resetActorScore(std::string& actorKey, std::string objective) {
    auto auid = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->resetScore(objective, auid);
}

bool GMLIB_Actor::resetActorScore(ActorUniqueID& auid) { return GMLIB_Scoreboard::getInstance()->resetScore(auid); }

bool GMLIB_Actor::resetActorScore(std::string& actorKey) {
    auto auid = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->resetScore(auid);
}

bool GMLIB_Actor::isPlayer() const { return hasCategory(ActorCategory::Player); }

bool GMLIB_Actor::isItemActor() const { return hasCategory(ActorCategory::Item); }

bool GMLIB_Actor::isMob() const { return hasCategory(ActorCategory::Mob); }

std::unique_ptr<CompoundTag> GMLIB_Actor::getNbt() {
    auto nbt = std::make_unique<CompoundTag>();
    save(*nbt);
    return std::move(nbt);
}

bool GMLIB_Actor::setNbt(CompoundTag& nbt) { return load(nbt); }

bool GMLIB_Actor::setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto data = *getNbt();
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    auto res = load(data);
    refresh();
    return res;
}

std::optional<int> GMLIB_Actor::getScore(std::string objective) {
    return GMLIB_Scoreboard::getInstance()->getScore(objective, this);
}

std::optional<int> GMLIB_Actor::setScore(std::string objective, int value, PlayerScoreSetFunction action) {
    return GMLIB_Scoreboard::getInstance()->setScore(objective, this, value, action);
}

bool GMLIB_Actor::resetScore(std::string objective) {
    return GMLIB_Scoreboard::getInstance()->resetScore(objective, this);
}

bool GMLIB_Actor::resetScore() { return GMLIB_Scoreboard::getInstance()->resetScore(this); }

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