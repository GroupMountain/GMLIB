#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/PlayerAPI.h>

void forEachUuid(bool includeOfflineSignedId, std::function<void(std::string_view const& uuid)> callback) {
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "player_",
        DBHelpers::Category::Player,
        [&callback, includeOfflineSignedId](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto  tag   = CompoundTag::fromBinaryNbt(data);
                auto& msaId = tag->getString("MsaId");
                if (!msaId.empty()) {
                    if (msaId == key_left) {
                        callback(msaId);
                    }
                    return;
                }
                if (!includeOfflineSignedId) {
                    return;
                }
                auto& selfSignedId = tag->getString("SelfSignedId");
                if (!selfSignedId.empty()) {
                    if (selfSignedId == key_left) {
                        callback(selfSignedId);
                    }
                    return;
                }
            }
        }
    );
}

std::vector<std::string> GMLIB_Player::getAllUuids(bool includeOfflineSignedId) {
    std::vector<std::string> uuids;
    forEachUuid(includeOfflineSignedId, [&uuids](std::string_view uuid) { uuids.push_back(std::string(uuid)); });
    return uuids;
}

std::unique_ptr<CompoundTag> GMLIB_Player::getUuidDBTag(mce::UUID const& uuid) {
    auto& dbStorage = *GMLIB::Global<DBStorage>;
    auto  playerKey = "player_" + uuid.asString();
    if (dbStorage.hasKey(playerKey, DBHelpers::Category::Player)) {
        return dbStorage.getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

std::string GMLIB_Player::getServeridFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

std::unique_ptr<CompoundTag> GMLIB_Player::getOfflineNbt(std::string& serverid) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverid, DBHelpers::Category::Player);
}

bool GMLIB_Player::setOfflineNbt(std::string& serverid, CompoundTag* nbt) {
    try {
        auto& data = *nbt;
        if (serverid.empty()) {
            return false;
        }
        GMLIB::Global<DBStorage>->saveData(serverid, data.toBinaryNbt(), DBHelpers::Category::Player);
        return true;
    } catch (...) {
        return false;
    }
}

std::unique_ptr<CompoundTag> GMLIB_Player::getPlayerNbt(mce::UUID uuid) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto nbt = player->save();
        return nbt;
    } else {
        auto serverid = getServeridFromUuid(uuid);
        return getOfflineNbt(serverid);
    }
}

std::unique_ptr<CompoundTag> GMLIB_Player::getNbt() { return save(); }

bool GMLIB_Player::setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        return player->load(*nbt);
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    return setOfflineNbt(serverid, nbt);
}

bool GMLIB_Player::setNbt(CompoundTag* nbt) { return load(*nbt); }

void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags) {
    for (auto tag : tags) {
        if (dataNbt->get(tag)) {
            originNbt->put(tag, dataNbt->get(tag)->copy());
        }
    }
}

bool GMLIB_Player::setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto data = player->save();
        setNbtTags(data.get(), nbt, tags);
        return player->load(*nbt);
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    auto data = getOfflineNbt(serverid);
    setNbtTags(data.get(), nbt, tags);
    return setOfflineNbt(serverid, data.get());
}

bool GMLIB_Player::deletePlayerNbt(std::string& serverid) {
    if (serverid.empty()) {
        return false;
    }
    if (GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        GMLIB::Global<DBStorage>->deleteData(serverid, DBHelpers::Category::Player);
        return true;
    }
    return false;
}

bool GMLIB_Player::deletePlayerNbt(mce::UUID& uuid) {
    auto serverid = getServeridFromUuid(uuid);
    return deletePlayerNbt(serverid);
}

void GMLIB_Player::setClientSidebar(
    const std::string&                              title,
    const std::vector<std::pair<std::string, int>>& data,
    ObjectiveSortOrder                              sortOrder
) {
    SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder))
        .sendTo(*this);

    std::vector<ScorePacketInfo> info;
    for (auto& key : data) {
        auto         idValue = ll::service::getLevel()->getRandom().nextLong();
        ScoreboardId id      = ScoreboardId(idValue);
        auto         text    = key.first;
        auto         scoreInfo =
            ScorePacketInfo(&id, "GMLIB_SIDEBAR_API", IdentityDefinition::Type::FakePlayer, key.second, text);
        info.emplace_back(scoreInfo);
    }
    auto pkt        = (SetScorePacket*)(MinecraftPackets::createPacket(MinecraftPacketIds::SetScore).get());
    pkt->mType      = (ScorePacketType)0;
    pkt->mScoreInfo = info;
    pkt->sendTo(*this);

    SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder))
        .sendTo(*this);
}

void GMLIB_Player::removeClientSidebar() {
    SetDisplayObjectivePacket("sidebar", "", "", "dummy", ObjectiveSortOrder::Ascending).sendTo(*this);
}

void GMLIB_Player::setHealth(int value) {
    return getMutableAttribute(SharedAttributes::HEALTH)->setCurrentValue(value);
}

void GMLIB_Player::setMaxHealth(int value) { return getMutableAttribute(SharedAttributes::HEALTH)->setMaxValue(value); }

void GMLIB_Player::setAbsorption(int value) {
    return getMutableAttribute(SharedAttributes::ABSORPTION)->setCurrentValue(value);
}

void GMLIB_Player::setMaxAbsorption(int value) {
    return getMutableAttribute(SharedAttributes::ABSORPTION)->setMaxValue(value);
}

void GMLIB_Player::setAttackDamage(int value) {
    return getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setCurrentValue(value);
}

void GMLIB_Player::setJumpStrength(int value) {
    return getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setCurrentValue(value);
}

void GMLIB_Player::setKnockbackResistance(int value) {
    return getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)->setCurrentValue(value);
}

void GMLIB_Player::setLavaMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Player::setLuck(int value) { return getMutableAttribute(SharedAttributes::LUCK)->setCurrentValue(value); }

void GMLIB_Player::setMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Player::setUnderwaterMovementSpeed(int value) {
    return getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->setCurrentValue(value);
}

void GMLIB_Player::setClientGamemode(GameType gamemode) {
    UpdatePlayerGameTypePacket(gamemode, getOrCreateUniqueID()).sendTo(*this);
}

void GMLIB_Player::setClientBossbar(
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
) {
    // AddActorPacket
    GMLIB_BinaryStream bs1;
    bs1.writeVarInt64(bossbarId);
    bs1.writeUnsignedVarInt64(bossbarId);
    bs1.writeString("player");
    bs1.writeVec3(Vec3{getPosition().x, -66.0f, getPosition().z});
    bs1.writeVec3(Vec3{0, 0, 0});
    bs1.writeVec3(Vec3{0, 0, 0});
    bs1.writeFloat(0.0f);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    auto pkt1 = MinecraftPackets::createPacket(MinecraftPacketIds::AddActor);
    pkt1->read(bs1);
    pkt1->sendTo(*this);
    // BossEventPacket
    GMLIB_BinaryStream bs2;
    bs2.mBuffer->reserve(8 + name.size());
    bs2.writeVarInt64(bossbarId);
    bs2.writeUnsignedVarInt(0);
    bs2.writeString(name);
    bs2.writeFloat(percentage);
    bs2.writeUnsignedShort(1);
    bs2.writeUnsignedVarInt((int)color);
    bs2.writeUnsignedVarInt(overlay);
    auto pkt2 = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    pkt2->read(bs2);
    pkt2->sendTo(*this);
}

void GMLIB_Player::removeClientBossbar(int64_t bossbarId) {
    GMLIB_BinaryStream bs;
    bs.writeVarInt64(bossbarId);
    bs.writeUnsignedVarInt((int)2);
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    pkt->read(bs);
    pkt->sendTo(*this);
}

void GMLIB_Player::updateClientBossbar(
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
) {
    removeClientBossbar(bossbarId);
    setClientBossbar(bossbarId, name, percentage, color, overlay);
}

void GMLIB_Player::addEffect(
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

void GMLIB_Player::removeEffect(MobEffect::EffectType effectType) { return removeEffect((int)effectType); }

std::vector<MobEffectInstance> GMLIB_Player::getAllEffects() {
    std::vector<MobEffectInstance> result = {};
    for (int i = 0; i <= 30; i++) {
        auto effect = getEffect(i);
        if (effect) {
            result.emplace_back(*effect);
        }
    }
    return result;
}
