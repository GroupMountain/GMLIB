#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/PlayerAPI.h>

namespace GMLIB::PlayerAPI {

GMLIB_API void forEachUuid(bool includeOfflineSignedId, std::function<void(std::string_view const& uuid)> callback) {
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

GMLIB_API std::vector<std::string> getAllUuids(bool includeOfflineSignedId) {
    std::vector<std::string> uuids;
    forEachUuid(includeOfflineSignedId, [&uuids](std::string_view uuid) { uuids.push_back(std::string(uuid)); });
    return uuids;
}

GMLIB_API std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid) {
    auto& dbStorage = *GMLIB::Global<DBStorage>;
    auto  playerKey = "player_" + uuid.asString();
    if (dbStorage.hasKey(playerKey, DBHelpers::Category::Player)) {
        return dbStorage.getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

GMLIB_API std::string getServeridFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

GMLIB_API std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverid) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverid, DBHelpers::Category::Player);
}

GMLIB_API bool setOfflineNbt(std::string& serverid, CompoundTag* nbt) {
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

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto nbt = player->save();
        return nbt;
    } else {
        auto serverid = getServeridFromUuid(uuid);
        return getOfflineNbt(serverid);
    }
}

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(Player* pl) { return pl->save(); }

GMLIB_API bool setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        return GMLIB::CompoundTagHelper::setNbt(player, nbt);
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    return setOfflineNbt(serverid, nbt);
}

GMLIB_API bool setPlayerNbt(Player* pl, CompoundTag* nbt) { return GMLIB::CompoundTagHelper::setNbt(pl, nbt); }

GMLIB_API void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags) {
    for (auto tag : tags) {
        if (dataNbt->get(tag)) {
            originNbt->put(tag, dataNbt->get(tag)->copy());
        }
    }
}

GMLIB_API bool setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto data = player->save();
        setNbtTags(data.get(), nbt, tags);
        return GMLIB::CompoundTagHelper::setNbt(player, data.get());
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    auto data = getOfflineNbt(serverid);
    setNbtTags(data.get(), nbt, tags);
    return setOfflineNbt(serverid, data.get());
}

GMLIB_API bool deletePlayerNbt(std::string& serverid) {
    if (serverid.empty()) {
        return false;
    }
    if (GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        GMLIB::Global<DBStorage>->deleteData(serverid, DBHelpers::Category::Player);
        return true;
    }
    return false;
}

GMLIB_API void setClientSidebar(
    Player*                                         player,
    const std::string&                              title,
    const std::vector<std::pair<std::string, int>>& data,
    ObjectiveSortOrder                              sortOrder
) {
    SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder))
        .sendTo(*player);

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
    pkt->sendTo(*player);

    SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder))
        .sendTo(*player);
}

GMLIB_API void removeClientSidebar(Player* player) {
    SetDisplayObjectivePacket("sidebar", "", "", "dummy", ObjectiveSortOrder::Ascending).sendTo(*player);
}

GMLIB_API void setHealth(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::HEALTH)->setCurrentValue(value);
}

GMLIB_API void setMaxHealth(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::HEALTH)->setMaxValue(value);
}

GMLIB_API void setAbsorption(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::ABSORPTION)->setCurrentValue(value);
}

GMLIB_API void setMaxAbsorption(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::ABSORPTION)->setMaxValue(value);
}

GMLIB_API void setAttackDamage(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setCurrentValue(value);
}

GMLIB_API void setJumpStrength(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setCurrentValue(value);
}

GMLIB_API void setKnockbackResistance(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)->setCurrentValue(value);
}

GMLIB_API void setLavaMovementSpeed(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)->setCurrentValue(value);
}

GMLIB_API void setLuck(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::LUCK)->setCurrentValue(value);
}

GMLIB_API void setMovementSpeed(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)->setCurrentValue(value);
}

GMLIB_API void setUnderwaterMovementSpeed(Player* player, int value) {
    player->getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->setCurrentValue(value);
}

GMLIB_API void setClientGamemode(Player* player, GameType gamemode) {
    UpdatePlayerGameTypePacket(gamemode, player->getOrCreateUniqueID()).sendTo(*player);
}

GMLIB_API void setClientBossbar(
    Player*        player,
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
) {
    // AddActorPacket
    BinaryStream bs1;
    BinaryStreamHelper::writeVarInt64(bs1, bossbarId);
    BinaryStreamHelper::writeUnsignedVarInt64(bs1, bossbarId);
    BinaryStreamHelper::writeString(bs1, "player");
    BinaryStreamHelper::writeVec3(bs1, Vec3{player->getPosition().x, -66.0f, player->getPosition().z});
    BinaryStreamHelper::writeVec3(bs1, Vec3{0, 0, 0});
    BinaryStreamHelper::writeVec3(bs1, Vec3{0, 0, 0});
    BinaryStreamHelper::writeFloat(bs1, 0.0f);
    BinaryStreamHelper::writeUnsignedVarInt(bs1, 0);
    BinaryStreamHelper::writeUnsignedVarInt(bs1, 0);
    BinaryStreamHelper::writeUnsignedVarInt(bs1, 0);
    BinaryStreamHelper::writeUnsignedVarInt(bs1, 0);
    BinaryStreamHelper::writeUnsignedVarInt(bs1, 0);
    auto pkt1 = MinecraftPackets::createPacket(MinecraftPacketIds::AddActor);
    pkt1->read(bs1);
    pkt1->sendTo(*player);
    // BossEventPacket
    BinaryStream bs2;
    bs2.mBuffer->reserve(8 + name.size());
    BinaryStreamHelper::writeVarInt64(bs2, bossbarId);
    BinaryStreamHelper::writeUnsignedVarInt(bs2, 0);
    BinaryStreamHelper::writeString(bs2, name);
    BinaryStreamHelper::writeFloat(bs2, percentage);
    BinaryStreamHelper::writeUnsignedShort(bs2, 1);
    BinaryStreamHelper::writeUnsignedVarInt(bs2, (int)color);
    BinaryStreamHelper::writeUnsignedVarInt(bs2, overlay);
    auto pkt2 = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    pkt2->read(bs2);
    pkt2->sendTo(*player);
}

GMLIB_API void removeClientBossbar(Player* player, int64_t bossbarId) {
    BinaryStream bs;
    BinaryStreamHelper::writeVarInt64(bs, bossbarId);
    BinaryStreamHelper::writeUnsignedVarInt(bs, (int)2);
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    pkt->read(bs);
    pkt->sendTo(*player);
}

GMLIB_API void updateClientBossbar(
    Player*        player,
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
) {
    removeClientBossbar(player, bossbarId);
    setClientBossbar(player, bossbarId, name, percentage, color, overlay);
}

GMLIB_API void addEffect(
    Player*               player,
    MobEffect::EffectType effectType,
    int                   duration,
    int                   amplifier,
    bool                  showParticles,
    bool                  ambient,
    bool                  showAnimation
) {
    return GMLIB::ActorAPI::addEffect(
        (Actor*)player,
        effectType,
        duration,
        amplifier,
        showParticles,
        ambient,
        showAnimation
    );
}

GMLIB_API void removeEffect(Player* player, MobEffect::EffectType effectType) {
    return player->removeEffect((int)effectType);
}

GMLIB_API void removeAllEffects(Player* player) { return player->removeAllEffects(); }

GMLIB_API std::vector<MobEffectInstance> getAllEffects(Player* player) {
    return GMLIB::ActorAPI::getAllEffects((Actor*)player);
}

} // namespace GMLIB::PlayerAPI