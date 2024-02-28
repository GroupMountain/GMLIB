#include "Global.h"
#include <GMLIB/Mod/CustomDamageCause.h>
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/NetworkPacketAPI.h>
#include <GMLIB/Server/PlayerAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <mc/locale/I18n.h>
#include <mc/locale/Localization.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/MinecraftPackets.h>
#include <mc/network/packet/AddActorPacket.h>
#include <mc/network/packet/BossEventPacket.h>
#include <mc/network/packet/ScorePacketInfo.h>
#include <mc/network/packet/SetDisplayObjectivePacket.h>
#include <mc/network/packet/SetScorePacket.h>
#include <mc/network/packet/ToastRequestPacket.h>
#include <mc/network/packet/UpdatePlayerGameTypePacket.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>
#include <mc/world/effect/MobEffect.h>
#include <mc/world/effect/MobEffectInstance.h>
#include <mc/world/level/storage/DBStorage.h>

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

std::string GMLIB_Player::getServerIdFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

std::unique_ptr<CompoundTag> GMLIB_Player::getOfflineNbt(std::string& serverId) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverId, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverId, DBHelpers::Category::Player);
}

bool GMLIB_Player::setOfflineNbt(std::string& serverId, CompoundTag& nbt) {
    try {
        if (serverId.empty()) {
            return false;
        }
        GMLIB::Global<DBStorage>->saveData(serverId, nbt.toBinaryNbt(), DBHelpers::Category::Player);
        return true;
    } catch (...) {
        return false;
    }
}

std::unique_ptr<CompoundTag> GMLIB_Player::getPlayerNbt(std::string& serverId) {
    if (serverId.empty()) {
        return nullptr;
    }
    auto player = (GMLIB_Player*)ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId);
    if (player) {
        return player->getNbt();
    } else {
        return getOfflineNbt(serverId);
    }
}

std::unique_ptr<CompoundTag> GMLIB_Player::getPlayerNbt(mce::UUID const& uuid) {
    auto serverId = getServerIdFromUuid(uuid);
    return getPlayerNbt(serverId);
}

std::unique_ptr<CompoundTag> GMLIB_Player::getNbt() {
    auto nbt = std::make_unique<CompoundTag>();
    save(*nbt);
    return std::move(nbt);
}

bool GMLIB_Player::setPlayerNbt(std::string& serverId, CompoundTag& nbt) {
    if (serverId.empty()) {
        return false;
    }
    auto player = ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId);
    if (player) {
        auto res = player->load(nbt);
        player->refreshInventory();
        return res;
    }
    return setOfflineNbt(serverId, nbt);
}

bool GMLIB_Player::setPlayerNbt(mce::UUID const& uuid, CompoundTag& nbt) {
    auto serverId = getServerIdFromUuid(uuid);
    return setPlayerNbt(serverId, nbt);
}

bool GMLIB_Player::setNbt(CompoundTag& nbt) { return load(nbt); }

void writeNbtTags(CompoundTag& originNbt, CompoundTag& dataNbt, const std::vector<std::string>& tags) {
    for (auto tag : tags) {
        if (dataNbt.get(tag)) {
            originNbt.put(tag, dataNbt.get(tag)->copy());
        }
    }
}

bool GMLIB_Player::setPlayerNbtTags(std::string& serverId, CompoundTag& nbt, const std::vector<std::string>& tags) {
    if (serverId.empty()) {
        return false;
    }
    auto player = (GMLIB_Player*)ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId);
    if (player) {
        return player->setNbtTags(nbt, tags);
    }
    auto data = *getOfflineNbt(serverId);
    writeNbtTags(data, nbt, tags);
    return setOfflineNbt(serverId, data);
}

bool GMLIB_Player::setPlayerNbtTags(mce::UUID const& uuid, CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto serverId = getServerIdFromUuid(uuid);
    return setPlayerNbtTags(serverId, nbt, tags);
}

bool GMLIB_Player::setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto data = *getNbt();
    writeNbtTags(data, nbt, tags);
    auto res = load(data);
    refreshInventory();
    return res;
}

bool GMLIB_Player::deletePlayerNbt(std::string& serverId) {
    if (serverId.empty()) {
        return false;
    }
    auto pl = ll::service::getLevel()->getPlayerFromServerId(serverId);
    if (pl) {
        ll::service::getLevel()->getLevelStorage().deleteData(serverId, DBHelpers::Category::Player);
        return true;
    }
    if (GMLIB::Global<DBStorage>->hasKey(serverId, DBHelpers::Category::Player)) {
        GMLIB::Global<DBStorage>->deleteData(serverId, DBHelpers::Category::Player);
        return true;
    }
    return false;
}

bool GMLIB_Player::deletePlayerNbt(mce::UUID const& uuid) {
    auto pl = ll::service::getLevel()->getPlayer(uuid);
    if (pl) {
        auto serverId = pl->getServerId();
        return deletePlayerNbt(serverId);
    }
    auto serverId = getServerIdFromUuid(uuid);
    return deletePlayerNbt(serverId);
}

std::optional<std::pair<Vec3, int>> GMLIB_Player::getPlayerPosition(std::string& serverId) {
    auto pl = ll::service::getLevel()->getPlayerFromServerId(serverId);
    if (pl) {
        return {
            {pl->getPosition(), pl->getDimensionId()}
        };
    }
    auto nbt = getPlayerNbt(serverId);
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

std::optional<std::pair<Vec3, int>> GMLIB_Player::getPlayerPosition(mce::UUID& uuid) {
    auto pl = ll::service::getLevel()->getPlayer(uuid);
    if (pl) {
        return {
            {pl->getPosition(), pl->getDimensionId()}
        };
    }
    auto nbt = getPlayerNbt(uuid);
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

bool GMLIB_Player::setPlayerPosition(std::string& serverId, Vec3 pos, DimensionType dimId) {
    auto pl = ll::service::getLevel()->getPlayerFromServerId(serverId);
    if (pl) {
        pl->teleport(pos, dimId);
        return true;
    }
    auto nbt = getPlayerNbt(serverId);
    if (nbt) {
        auto tag                        = nbt->getList("Pos");
        tag->at(0)->as<FloatTag>().data = pos.x;
        tag->at(1)->as<FloatTag>().data = pos.y;
        tag->at(2)->as<FloatTag>().data = pos.z;
        nbt->putInt("DimensionId", dimId.id);
        return setPlayerNbt(serverId, *nbt);
    }
    return false;
}

bool GMLIB_Player::setPlayerPosition(mce::UUID& uuid, Vec3 pos, DimensionType dimId) {
    auto pl = ll::service::getLevel()->getPlayer(uuid);
    if (pl) {
        pl->teleport(pos, dimId);
        return true;
    }
    auto nbt = getPlayerNbt(uuid);
    if (nbt) {
        auto tag                        = nbt->getList("Pos");
        tag->at(0)->as<FloatTag>().data = pos.x;
        tag->at(1)->as<FloatTag>().data = pos.y;
        tag->at(2)->as<FloatTag>().data = pos.z;
        nbt->putInt("DimensionId", dimId.id);
        return setPlayerNbt(uuid, *nbt);
    }
    return false;
}

void GMLIB_Player::setClientSidebar(
    const std::string                               title,
    const std::vector<std::pair<std::string, int>>& data,
    ObjectiveSortOrder                              sortOrder
) {
    SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder))
        .sendTo(*this);

    std::vector<ScorePacketInfo> info;
    for (auto& key : data) {
        auto                idValue   = GMLIB_Actor::getNextActorUniqueID();
        const ScoreboardId& id        = ScoreboardId(idValue);
        auto                text      = key.first;
        auto                scoreInfo = ScorePacketInfo();
        scoreInfo.mScoreboardId       = id;
        scoreInfo.mObjectiveName      = "GMLIB_SIDEBAR_API";
        scoreInfo.mIdentityType       = IdentityDefinition::Type::FakePlayer;
        scoreInfo.mScoreValue         = key.second;
        scoreInfo.mFakePlayerName     = text;
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
    bs1.writeVec2(Vec2{0, 0});
    bs1.writeFloat(0.0f);
    bs1.writeFloat(0.0f);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    GMLIB::Server::NetworkPacket<(int)MinecraftPacketIds::AddActor> pkt1(bs1.getAndReleaseData());
    pkt1.sendTo(*this);
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

int64_t GMLIB_Player::setClientBossbar(std::string name, float percentage, ::BossBarColor color, int overlay) {
    auto bossbarId = GMLIB_Actor::getNextActorUniqueID();
    setClientBossbar(bossbarId, name, percentage, color, overlay);
    return bossbarId;
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

void GMLIB_Player::setClientWeather(WeatherType weather) { return GMLIB_Level::setClientWeather(weather, this); }

void GMLIB_Player::sendToast(std::string_view title, std::string_view message) {
    auto pkt = ToastRequestPacket(std::string(title), std::string(message));
    pkt.sendTo(*this);
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

std::optional<int> GMLIB_Player::getScore(std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->getPlayerScore(objective, this);
}

std::optional<int> GMLIB_Player::setScore(std::string objective, int value, PlayerScoreSetFunction action) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->setPlayerScore(objective, this, value, action);
}

bool GMLIB_Player::resetScore(std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerScore(objective, this);
}

bool GMLIB_Player::resetScore() {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerAllScores(this);
}

std::optional<int> GMLIB_Player::getPlayerScore(std::string& serverId, std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->getPlayerScore(objective, serverId);
}

std::optional<int> GMLIB_Player::getPlayerScore(mce::UUID& uuid, std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->getPlayerScore(objective, uuid);
}

std::optional<int>
GMLIB_Player::setPlayerScore(std::string& serverId, std::string objective, int value, PlayerScoreSetFunction action) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->setPlayerScore(objective, serverId, value, action);
}

std::optional<int>
GMLIB_Player::setPlayerScore(mce::UUID& uuid, std::string objective, int value, PlayerScoreSetFunction action) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->setPlayerScore(objective, uuid, value, action);
}

bool GMLIB_Player::resetPlayerScore(std::string& serverId, std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerScore(objective, serverId);
}

bool GMLIB_Player::resetPlayerScore(mce::UUID& uuid, std::string objective) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerScore(objective, uuid);
}

bool GMLIB_Player::resetPlayerScore(std::string& serverId) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerAllScores(serverId);
}

bool GMLIB_Player::resetPlayerScore(mce::UUID& uuid) {
    auto scoreboard = GMLIB_Scoreboard::getServerScoreboard();
    return scoreboard->resetPlayerAllScores(uuid);
}

ItemStack* GMLIB_Player::getMainHandSlot() {
    return (ItemStack*)&getEquippedSlot(Puv::Legacy::EquipmentSlot::Mainhand);
}

void GMLIB_Player::setMainHandSlot(ItemStack& itemStack) {
    return setEquippedSlot(Puv::Legacy::EquipmentSlot::Mainhand, itemStack);
}

ItemStack* GMLIB_Player::getOffHandSlot() { return (ItemStack*)&getEquippedSlot(Puv::Legacy::EquipmentSlot::Offhand); }

void GMLIB_Player::setOffHandSlot(ItemStack& itemStack) {
    return setEquippedSlot(Puv::Legacy::EquipmentSlot::Offhand, itemStack);
}

GMLIB_Actor* GMLIB_Player::shootProjectile(std::string typeName, float speed, float offset) {
    return GMLIB_Spawner::spawnProjectile((GMLIB_Actor*)this, typeName, speed, offset);
}

void GMLIB_Player::setFreezing(float percentage) { getEntityData().set<float>(0x78, percentage); }

void GMLIB_Player::hurtPlayer(float damage, std::string causeName, Actor* source) {
    auto cause = GMLIB::Mod::DamageCause::getCauseFromName(causeName);
    this->hurtByCause(damage, cause, source);
}

InventoryTransactionManager* GMLIB_Player::getInventoryTransactionManager() {
    return ll::memory::dAccess<InventoryTransactionManager*>(this, 3816); // IDA: ClearCommand::execute()  Line 392
}

FullPlayerInventoryWrapper GMLIB_Player::getFullPlayerInventoryWrapper() {
    return FullPlayerInventoryWrapper(
        getSupplies(),
        getArmorContainer(),
        getHandContainer(),
        getInventoryTransactionManager(),
        this
    );
}

int GMLIB_Player::clearAllItems() { return getFullPlayerInventoryWrapper().clearAllItems(); }

std::string_view GMLIB_Player::getIP() {
    auto ipAndPort = getNetworkIdentifier().getIPAndPort();
    auto pos       = ipAndPort.find(":");
    return ipAndPort.substr(0, pos);
}

ushort GMLIB_Player::getPort() {
    auto ipAndPort = getNetworkIdentifier().getIPAndPort();
    auto pos       = ipAndPort.find(":");
    auto strPort   = ipAndPort.substr(pos + 1);
    return std::stoi(strPort);
}

int GMLIB_Player::getAvgPing() {
    if (isSimulatedPlayer()) {
        return -1;
    }
    return getNetworkStatus()->mAveragePing;
}

int GMLIB_Player::getLastPing() {
    if (isSimulatedPlayer()) {
        return -1;
    }
    return getNetworkStatus()->mCurrentPing;
}

std::string_view GMLIB_Player::getLanguageCode() {
    if (isSimulatedPlayer()) {
        return I18n::getCurrentLanguage()->getFullLanguageCode();
    }
    auto map = ll::service::getServerNetworkHandler()
                   ->fetchConnectionRequest(getNetworkIdentifier())
                   .mRawToken.get()
                   ->mDataInfo.value_.map_;
    for (auto& iter : *map) {
        std::string s(iter.first.c_str());
        if (s.find("LanguageCode") != std::string::npos) {
            auto langCode = iter.second.value_.string_;
            return langCode;
        }
    }
    return "unknown";
}

void GMLIB_Player::updateClientBlock(
    BlockPos const&               pos,
    uint                          runtimeId,
    BlockUpdateFlag               flag,
    UpdateBlockPacket::BlockLayer layer
) {
    UpdateBlockPacket(pos, (uint)layer, runtimeId, (uchar)flag).sendTo(*this);
}

bool GMLIB_Player::updateClientBlock(
    BlockPos const&               pos,
    Block*                        block,
    BlockUpdateFlag               flag,
    UpdateBlockPacket::BlockLayer layer
) {
    if (block) {
        auto runtimeId = block->getRuntimeId();
        updateClientBlock(pos, runtimeId, flag, layer);
        return true;
    }
    return false;
}

bool GMLIB_Player::updateClientBlock(
    BlockPos const&               pos,
    std::string_view              blockName,
    ushort                        auxValue,
    BlockUpdateFlag               flag,
    UpdateBlockPacket::BlockLayer layer
) {
    auto block = Block::tryGetFromRegistry(blockName, auxValue);
    if (block.has_value()) {
        auto runtimeId = block->getRuntimeId();
        updateClientBlock(pos, runtimeId, flag, layer);
        return true;
    }
    return false;
}

bool GMLIB_Player::updateClientBlock(
    BlockPos const&               pos,
    std::string_view              blockName,
    Block::BlockStatesType        blockState,
    BlockUpdateFlag               flag,
    UpdateBlockPacket::BlockLayer layer
) {
    auto block = Block::tryGetFromRegistry(blockName, blockState);
    if (block.has_value()) {
        auto runtimeId = block->getRuntimeId();
        updateClientBlock(pos, runtimeId, flag, layer);
        return true;
    }
    return false;
}