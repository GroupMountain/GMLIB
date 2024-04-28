#include "Global.h"
#include <GMLIB/Mod/Addons/CustomDamageCause.h>
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <corecrt_math_defines.h>
#include <mc/math/Vec2.h>
#include <mc/server/commands/ActorCommandOrigin.h>
#include <mc/server/commands/CommandContext.h>
#include <mc/server/commands/MinecraftCommands.h>
#include <mc/util/Random.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>
#include <mc/world/effect/MobEffect.h>
#include <mc/world/effect/MobEffectInstance.h>
#include <mc/world/level/dimension/VanillaDimensions.h>
#include <mc/world/level/storage/DBStorage.h>

std::unordered_map<int64, std::string> GMLIB_Actor::getActorIdsKeyMap() {
    std::unordered_map<int64, std::string> result;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "actorprefix",
        DBHelpers::Category::Actor,
        [&result](std::string_view key_left, std::string_view data) {
            auto keyId   = "actorprefix" + std::string(key_left);
            auto keyData = CompoundTag::fromBinaryNbt(data);
            if (keyData && keyData->contains("UniqueID")) {
                auto intUniqueId    = keyData->getInt64("UniqueID");
                result[intUniqueId] = keyId;
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
                auto strAuid  = keyData->getInt64("UniqueID");
                auto uniqueId = ActorUniqueID(strAuid);
                result.push_back(uniqueId);
            } else {
                GMLIB::Global<DBStorage>->deleteData(keyId, DBHelpers::Category::Actor);
            }
        }
    );
    return result;
}

std::unique_ptr<CompoundTag> getUnloadedActorNbt(std::string const& actorKey) {
    if (actorKey.empty()) {
        return nullptr;
    }
    if (!GMLIB::Global<DBStorage>->hasKey(actorKey, DBHelpers::Category::Actor)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(actorKey, DBHelpers::Category::Actor);
}

bool setUnloadedActorNbt(std::string const& actorKey, CompoundTag& nbt) {
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

ActorUniqueID GMLIB_Actor::getActorUniqueID(std::string const& actorKey) {
    auto nbt = getUnloadedActorNbt(actorKey);
    if (nbt && nbt->contains("UniqueID")) {
        auto uniqueId = nbt->getInt64("UniqueID");
        return ActorUniqueID(uniqueId);
    }
    return ActorUniqueID::INVALID_ID;
}

std::string getActorKeyFromUniqueId(ActorUniqueID const& uniqueId) {
    auto map = GMLIB_Actor::getActorIdsKeyMap();
    if (map.count(uniqueId.id)) {
        return map[uniqueId.id];
    }
    return "";
}

std::string GMLIB_Actor::getActorTypeName(ActorUniqueID const& uniqueId) {
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        return en->getTypeName();
    }
    auto actorKey = getActorKeyFromUniqueId(uniqueId);
    return getActorTypeName(actorKey);
}

std::string GMLIB_Actor::getActorTypeName(std::string const& actorKey) {
    auto nbt = getUnloadedActorNbt(actorKey);
    if (nbt && nbt->contains("identifier")) {
        auto type = nbt->getString("identifier");
        return type;
    }
    return "unknown";
}

std::optional<std::pair<Vec3, DimensionType>> GMLIB_Actor::getActorPosition(ActorUniqueID const& uniqueId) {
    if (auto ac = ll::service::getLevel()->fetchEntity(uniqueId)) {
        return {
            {ac->getPosition(), ac->getDimensionId()}
        };
    }
    auto nbt = getActorNbt(uniqueId);
    if (nbt) {
        auto  tag   = nbt->getList("Pos");
        float x     = tag->at(0)->as<FloatTag>().data;
        float y     = tag->at(1)->as<FloatTag>().data;
        float z     = tag->at(2)->as<FloatTag>().data;
        int   dimId = nbt->getInt("DimensionId");
        return {
            {{x, y, z}, dimId}
        };
    }
    return {};
}

std::optional<std::pair<Vec3, DimensionType>> GMLIB_Actor::getActorPosition(std::string const& actorKey) {
    auto uniqueId = getActorUniqueID(actorKey);
    return getActorPosition(uniqueId);
}

bool GMLIB_Actor::setActorPosition(ActorUniqueID const& uniqueId, Vec3 const& pos, DimensionType dimId) {
    if (auto ac = ll::service::getLevel()->fetchEntity(uniqueId)) {
        ac->teleport(pos, dimId);
        return true;
    }
    auto nbt = getActorNbt(uniqueId);
    if (nbt) {
        auto tag                        = nbt->getList("Pos");
        tag->at(0)->as<FloatTag>().data = pos.x;
        tag->at(1)->as<FloatTag>().data = pos.y;
        tag->at(2)->as<FloatTag>().data = pos.z;
        nbt->putInt("DimensionId", dimId.id);
        return setActorNbt(uniqueId, *nbt);
    }
    return false;
}

bool GMLIB_Actor::setActorPosition(std::string const& actorKey, Vec3 const& pos, DimensionType dimId) {
    auto uniqueId = getActorUniqueID(actorKey);
    return setActorPosition(uniqueId, pos, dimId);
}

std::unique_ptr<CompoundTag> GMLIB_Actor::getActorNbt(ActorUniqueID const& uniqueId) {
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->getNbt();
    }
    auto actorKey = getActorKeyFromUniqueId(uniqueId);
    return getActorNbt(actorKey);
}

std::unique_ptr<CompoundTag> GMLIB_Actor::getActorNbt(std::string const& actorKey) {
    auto uniqueId = getActorUniqueID(actorKey);
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->getNbt();
    }
    return getUnloadedActorNbt(actorKey);
}

bool GMLIB_Actor::setActorNbt(ActorUniqueID const& uniqueId, CompoundTag& nbt) {
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbt(nbt);
    }
    auto actorKey = getActorKeyFromUniqueId(uniqueId);
    return setActorNbt(actorKey, nbt);
}

bool GMLIB_Actor::setActorNbt(std::string const& actorKey, CompoundTag& nbt) {
    auto uniqueId = getActorUniqueID(actorKey);
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbt(nbt);
    }
    return setUnloadedActorNbt(actorKey, nbt);
}

bool GMLIB_Actor::setActorNbtTags(
    ActorUniqueID const&            uniqueId,
    CompoundTag&                    nbt,
    std::vector<std::string> const& tags
) {
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        auto actor = (GMLIB_Actor*)en;
        return actor->setNbtTags(nbt, tags);
    }
    auto actorKey = getActorKeyFromUniqueId(uniqueId);
    return setActorNbtTags(actorKey, nbt, tags);
}

bool GMLIB_Actor::setActorNbtTags(
    std::string const&                    actorKey,
    CompoundTag&                          nbt,
    std::vector<std::string> const& tags
) {
    if (actorKey.empty()) {
        return false;
    }
    auto uniqueId = getActorUniqueID(actorKey);
    if (auto actor = (GMLIB_Actor*)ll::service::bedrock::getLevel()->fetchEntity(uniqueId)) {
        return actor->setNbtTags(nbt, tags);
    }
    auto data = *getUnloadedActorNbt(actorKey);
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    return setUnloadedActorNbt(actorKey, data);
}

bool GMLIB_Actor::deleteActor(ActorUniqueID const& uniqueId) {
    if (auto en = ll::service::getLevel()->fetchEntity(uniqueId)) {
        en->remove();
        return true;
    }
    auto actorKey = getActorKeyFromUniqueId(uniqueId);
    return deleteActor(actorKey);
}

bool GMLIB_Actor::deleteActor(std::string const& actorKey) {
    if (actorKey.empty()) {
        return false;
    }
    auto uniqueId = getActorUniqueID(actorKey);
    GMLIB_Scoreboard::getInstance()->resetScore(uniqueId);
    if (auto actor = (GMLIB_Actor*)ll::service::bedrock::getLevel()->fetchEntity(uniqueId)) {
        return false;
    }
    if (GMLIB::Global<DBStorage>->hasKey(actorKey, DBHelpers::Category::Actor)) {
        GMLIB::Global<DBStorage>->deleteData(actorKey, DBHelpers::Category::Actor);
        return true;
    }
    return false;
}

std::optional<int> GMLIB_Actor::getActorScore(ActorUniqueID const& uniqueId, std::string const& objective) {
    return GMLIB_Scoreboard::getInstance()->getScore(objective, uniqueId);
}

std::optional<int> GMLIB_Actor::getActorScore(std::string const& actorKey, std::string const& objective) {
    auto uniqueId = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->getScore(objective, uniqueId);
}

std::optional<int> GMLIB_Actor::setActorScore(
    ActorUniqueID const&   uniqueId,
    std::string const&     objective,
    int                    value,
    PlayerScoreSetFunction action
) {
    return GMLIB_Scoreboard::getInstance()->setScore(objective, uniqueId, value, action);
}

std::optional<int> GMLIB_Actor::setActorScore(
    std::string const&     actorKey,
    std::string const&     objective,
    int                    value,
    PlayerScoreSetFunction action
) {
    auto uniqueId = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->setScore(objective, uniqueId, value, action);
}

bool GMLIB_Actor::resetActorScore(ActorUniqueID const& uniqueId, std::string const& objective) {
    return GMLIB_Scoreboard::getInstance()->resetScore(objective, uniqueId);
}

bool GMLIB_Actor::resetActorScore(std::string const& actorKey, std::string const& objective) {
    auto uniqueId = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->resetScore(objective, uniqueId);
}

bool GMLIB_Actor::resetActorScore(ActorUniqueID const& uniqueId) {
    return GMLIB_Scoreboard::getInstance()->resetScore(uniqueId);
}

bool GMLIB_Actor::resetActorScore(std::string const& actorKey) {
    auto uniqueId = getActorUniqueID(actorKey);
    return GMLIB_Scoreboard::getInstance()->resetScore(uniqueId);
}

bool GMLIB_Actor::isPlayer() const { return hasCategory(ActorCategory::Player); }

bool GMLIB_Actor::isItemActor() const { return hasCategory(ActorCategory::Item); }

bool GMLIB_Actor::isMob() const { return hasCategory(ActorCategory::Mob); }

std::unique_ptr<CompoundTag> GMLIB_Actor::getNbt() {
    auto nbt = std::make_unique<CompoundTag>();
    save(*nbt);
    return std::move(nbt);
}

bool GMLIB_Actor::setNbt(CompoundTag& nbt) { return load(nbt, *GMLIB_CompoundTag::getDataLoadHelper()); }

bool GMLIB_Actor::setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto data = *getNbt();
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    auto res = load(data, *GMLIB_CompoundTag::getDataLoadHelper());
    refresh();
    return res;
}

std::optional<int> GMLIB_Actor::getScore(std::string const& objective) {
    return GMLIB_Scoreboard::getInstance()->getScore(objective, this);
}

std::optional<int> GMLIB_Actor::setScore(std::string const& objective, int value, PlayerScoreSetFunction action) {
    return GMLIB_Scoreboard::getInstance()->setScore(objective, this, value, action);
}

bool GMLIB_Actor::resetScore(std::string const& objective) {
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

GMLIB_Actor* GMLIB_Actor::shootProjectile(std::string const& typeName, float speed, float offset) {
    return GMLIB_Spawner::spawnProjectile((GMLIB_Actor*)this, typeName, speed, offset);
}

int64_t GMLIB_Actor::getNextActorUniqueID() {
    auto res = Random::getThreadLocal().nextLong();
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(res)) != nullptr) {
        res = Random::getThreadLocal().nextLong();
    }
    return res;
}

void GMLIB_Actor::hurtEntity(float damage, std::string const& causeName, Actor* source) {
    auto cause = GMLIB::Mod::DamageCause::getCauseFromName(causeName);
    this->hurtByCause(damage, cause, source);
}

Biome* GMLIB_Actor::getBiome() {
    auto& bs = getDimensionBlockSourceConst();
    return const_cast<Biome*>(&bs.getConstBiome(BlockPos(getPosition())));
}

MCRESULT GMLIB_Actor::executeCommand(std::string_view command) {
    CommandContext context =
        CommandContext(std::string(command), std::make_unique<ActorCommandOrigin>(ActorCommandOrigin(*this)));
    return ll::service::getMinecraft()->getCommands().executeCommand(context);
}

std::string GMLIB_Actor::getDimensionTypeName() { return getDimension().mName; }

std::string GMLIB_Actor::getDimensionName() { return VanillaDimensions::toString(getDimensionId()); }