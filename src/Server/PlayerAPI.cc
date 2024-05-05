#include "Global.h"
#include <GMLIB/Mod/Addons/CustomDamageCause.h>
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/I18nAPI.h>
#include <GMLIB/Server/PlayerAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <mc/deps/core/utility/LootTableUtils.h>
#include <mc/entity/utilities/ActorEquipment.h>
#include <mc/locale/I18n.h>
#include <mc/locale/Localization.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/MinecraftPackets.h>
#include <mc/network/packet/AddActorPacket.h>
#include <mc/network/packet/BossEventPacket.h>
#include <mc/network/packet/RemoveActorPacket.h>
#include <mc/network/packet/ScorePacketInfo.h>
#include <mc/network/packet/SetDisplayObjectivePacket.h>
#include <mc/network/packet/SetScorePacket.h>
#include <mc/network/packet/ToastRequestPacket.h>
#include <mc/network/packet/UpdatePlayerGameTypePacket.h>
#include <mc/server/commands/CommandContext.h>
#include <mc/server/commands/CommandUtils.h>
#include <mc/server/commands/MinecraftCommands.h>
#include <mc/server/commands/PlayerCommandOrigin.h>
#include <mc/world/SimpleContainer.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>
#include <mc/world/effect/MobEffect.h>
#include <mc/world/effect/MobEffectInstance.h>
#include <mc/world/item/ItemInstance.h>
#include <mc/world/level/dimension/VanillaDimensions.h>
#include <mc/world/level/levelgen/WorldGenerator.h>
#include <mc/world/level/levelgen/structure/StructureFeatureTypeNames.h>
#include <mc/world/level/storage/DBStorage.h>

std::vector<std::string> GMLIB_Player::getAllServerIds() {
    std::vector<std::string> serverIds;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "player_server_",
        DBHelpers::Category::Player,
        [&serverIds](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto serverId = "player_server_" + std::string(key_left);
                serverIds.push_back(serverId);
            }
        }
    );
    return serverIds;
}

std::vector<mce::UUID> GMLIB_Player::getAllUuids(bool includeOfflineSignedId) {
    std::vector<mce::UUID> uuids;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "player_",
        DBHelpers::Category::Player,
        [&uuids, includeOfflineSignedId](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto  tag   = CompoundTag::fromBinaryNbt(data);
                auto& msaId = tag->getString("MsaId");
                if (!msaId.empty()) {
                    if (msaId == key_left) {
                        auto uuid = mce::UUID::fromString(msaId);
                        uuids.push_back(uuid);
                    }
                } else if (includeOfflineSignedId) {
                    auto& selfSignedId = tag->getString("SelfSignedId");
                    if (!selfSignedId.empty()) {
                        if (selfSignedId == key_left) {
                            auto uuid = mce::UUID::fromString(selfSignedId);
                            uuids.push_back(uuid);
                        }
                    }
                }
            }
        }
    );
    return uuids;
}

std::unique_ptr<CompoundTag> GMLIB_Player::getUuidDBTag(mce::UUID const& uuid) {
    auto playerKey = "player_" + uuid.asString();
    if (GMLIB::Global<DBStorage>->hasKey(playerKey, DBHelpers::Category::Player)) {
        return GMLIB::Global<DBStorage>->getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

bool GMLIB_Player::deleteUuidDBTag(mce::UUID const& uuid) {
    auto playerKey = "player_" + uuid.asString();
    if (GMLIB::Global<DBStorage>->hasKey(playerKey, DBHelpers::Category::Player)) {
        GMLIB::Global<DBStorage>->deleteData(playerKey, DBHelpers::Category::Player);
        return true;
    }
    return false;
}

std::string GMLIB_Player::getServerIdFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

std::unique_ptr<CompoundTag> GMLIB_Player::getOfflineNbt(std::string const& serverId) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverId, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverId, DBHelpers::Category::Player);
}

bool GMLIB_Player::createNewPlayerTag(mce::UUID const& uuid, std::string const& serverId) {
    auto keyId = "player_" + uuid.asString();
    if (!GMLIB::Global<DBStorage>->hasKey(keyId, DBHelpers::Category::Player)) {
        auto keyData = CompoundTag{
            {"MsaId",        uuid.asString()               },
            {"SelfSignedId", mce::UUID::random().asString()},
            {"ServerId",     serverId                      }
        };
        GMLIB::Global<DBStorage>->saveData(keyId, keyData.toBinaryNbt(), DBHelpers::Category::Player);
        return true;
    }
    return false;
}

bool GMLIB_Player::setOfflineNbt(std::string const& serverId, CompoundTag& nbt) {
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

std::unique_ptr<CompoundTag> GMLIB_Player::getPlayerNbt(std::string const& serverId) {
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

bool GMLIB_Player::setPlayerNbt(std::string const& serverId, CompoundTag& nbt) {
    if (serverId.empty()) {
        return false;
    }
    auto player = ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId);
    if (player) {
        auto res = player->load(nbt, *GMLIB_CompoundTag::getDataLoadHelper());
        player->refreshInventory();
        return res;
    }
    return setOfflineNbt(serverId, nbt);
}

bool GMLIB_Player::setPlayerNbt(mce::UUID const& uuid, CompoundTag& nbt, bool forceCreate) {
    auto serverId = getServerIdFromUuid(uuid);
    if (serverId.empty() && forceCreate) {
        serverId = "player_server_" + mce::UUID::random().asString();
        auto res = createNewPlayerTag(uuid, serverId);
        if (!res) {
            return false;
        }
    }
    return setPlayerNbt(serverId, nbt);
}

bool GMLIB_Player::setNbt(CompoundTag& nbt) { return load(nbt, *GMLIB_CompoundTag::getDataLoadHelper()); }

bool GMLIB_Player::setPlayerNbtTags(
    std::string const&              serverId,
    CompoundTag&                    nbt,
    std::vector<std::string> const& tags
) {
    if (serverId.empty()) {
        return false;
    }
    auto player = (GMLIB_Player*)ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId);
    if (player) {
        return player->setNbtTags(nbt, tags);
    }
    auto data = *getOfflineNbt(serverId);
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    return setOfflineNbt(serverId, data);
}

bool GMLIB_Player::setPlayerNbtTags(mce::UUID const& uuid, CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto serverId = getServerIdFromUuid(uuid);
    return setPlayerNbtTags(serverId, nbt, tags);
}

bool GMLIB_Player::setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags) {
    auto data = *getNbt();
    GMLIB_CompoundTag::writeNbtTags(data, nbt, tags);
    auto res = load(data, *GMLIB_CompoundTag::getDataLoadHelper());
    refreshInventory();
    return res;
}

bool GMLIB_Player::deletePlayerNbt(std::string const& serverId) {
    if (serverId.empty()) {
        return false;
    }
    auto pl = ll::service::getLevel()->getPlayerFromServerId(serverId);
    if (pl) {
        return false;
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
        return false;
    }
    auto serverId = getServerIdFromUuid(uuid);
    return deletePlayerNbt(serverId);
}

bool GMLIB_Player::deletePlayer(mce::UUID const& uuid) {
    auto pl = ll::service::getLevel()->getPlayer(uuid);
    if (pl) {
        return false;
    }
    auto serverId = getServerIdFromUuid(uuid);
    return deletePlayerNbt(serverId) || deleteUuidDBTag(uuid);
}

ActorUniqueID GMLIB_Player::getPlayerUniqueID(std::string const& serverId) {
    if (auto player = ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId)) {
        return player->getOrCreateUniqueID();
    }
    auto nbt = GMLIB_Player::getOfflineNbt(serverId);
    if (nbt && nbt->contains("UniqueID")) {
        auto uniqueId = nbt->getInt64("UniqueID");
        return ActorUniqueID(uniqueId);
    }
    return ActorUniqueID::INVALID_ID;
}

std::unordered_map<int64, std::string> GMLIB_Player::getUniqueIdToServerIdMap() {
    auto                                   serverIds = getAllServerIds();
    std::unordered_map<int64, std::string> result;
    for (auto& serverId : serverIds) {
        auto uniqueId       = getPlayerUniqueID(serverId);
        result[uniqueId.id] = serverId;
    }
    return result;
}

std::unordered_map<int64, mce::UUID> GMLIB_Player::getUniqueIdToUuidMap() {
    auto                                 uuids = getAllUuids(true);
    std::unordered_map<int64, mce::UUID> result;
    for (auto& uuid : uuids) {
        auto uniqueId       = getPlayerUniqueID(uuid);
        result[uniqueId.id] = uuid;
    }
    return result;
}

ActorUniqueID GMLIB_Player::getPlayerUniqueID(mce::UUID const& uuid) {
    auto serverId = getServerIdFromUuid(uuid);
    return getPlayerUniqueID(serverId);
}

std::optional<std::pair<Vec3, int>> GMLIB_Player::getPlayerPosition(std::string const& serverId) {
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

std::optional<std::pair<Vec3, int>> GMLIB_Player::getPlayerPosition(mce::UUID const& uuid) {
    auto serverId = getServerIdFromUuid(uuid);
    return getPlayerPosition(serverId);
}

bool GMLIB_Player::setPlayerPosition(std::string const& serverId, Vec3 const& pos, DimensionType dimId) {
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

bool GMLIB_Player::setPlayerPosition(mce::UUID const& uuid, Vec3 const& pos, DimensionType dimId) {
    auto serverId = getServerIdFromUuid(uuid);
    return setPlayerPosition(serverId, pos, dimId);
}

void GMLIB_Player::setClientSidebar(
    std::string const&                              title,
    std::vector<std::pair<std::string, int>> const& data,
    ObjectiveSortOrder                              sortOrder
) {
    removeClientSidebar();

    auto pkt1 =
        SetDisplayObjectivePacket("sidebar", "GMLIB_SIDEBAR_API", title, "dummy", ObjectiveSortOrder(sortOrder));
    sendPacket(pkt1);

    std::vector<ScorePacketInfo> info;
    for (auto& [key, index] : data) {
        const ScoreboardId& id        = ScoreboardId(index + 1145141919810);
        auto                text      = key;
        auto                scoreInfo = ScorePacketInfo();
        scoreInfo.mScoreboardId       = id;
        scoreInfo.mObjectiveName      = "GMLIB_SIDEBAR_API";
        scoreInfo.mIdentityType       = IdentityDefinition::Type::FakePlayer;
        scoreInfo.mScoreValue         = index;
        scoreInfo.mFakePlayerName     = text;
        info.emplace_back(scoreInfo);
    }
    auto pkt2 = SetScorePacket::change(info);
    sendPacket(pkt2);
}

void GMLIB_Player::removeClientSidebar() {
    auto pkt = SetDisplayObjectivePacket("sidebar", "", "", "dummy", ObjectiveSortOrder::Ascending);
    sendPacket(pkt);
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
    auto pkt =
        UpdatePlayerGameTypePacket(gamemode, getOrCreateUniqueID(), GMLIB_Level::getInstance()->getCurrentTick());
    sendPacket(pkt);
}

void GMLIB_Player::setClientBossbar(
    int64_t            bossbarId,
    std::string const& name,
    float              percentage,
    ::BossBarColor     color,
    ::BossBarOverlay   overlay
) {
    // AddActorPacket
    auto uniqueId = ActorUniqueID(bossbarId);
    if (!ll::service::getLevel()->fetchEntity(uniqueId)) {
        GMLIB_BinaryStream bs1;
        bs1.writePacketHeader(MinecraftPacketIds::AddActor);
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
        bs1.sendTo(*this);
    }
    // BossEventPacket
    auto pkt2           = BossEventPacket();
    pkt2.mBossID        = ActorUniqueID(bossbarId);
    pkt2.mHealthPercent = percentage;
    pkt2.mEventType     = BossEventUpdateType::Add;
    pkt2.mColor         = color;
    pkt2.mOverlay       = overlay;
    pkt2.mName          = name;
    sendPacket(pkt2);
}

int64_t GMLIB_Player::setClientBossbar(
    std::string const& name,
    float              percentage,
    ::BossBarColor     color,
    ::BossBarOverlay   overlay
) {
    auto bossbarId = GMLIB_Actor::getNextActorUniqueID();
    setClientBossbar(bossbarId, name, percentage, color, overlay);
    return bossbarId;
}

void GMLIB_Player::removeClientBossbar(int64_t bossbarId) {
    auto uniqueId  = ActorUniqueID(bossbarId);
    auto pkt       = BossEventPacket();
    pkt.mBossID    = uniqueId;
    pkt.mEventType = BossEventUpdateType::Remove;
    sendPacket(pkt);
    auto pkt2 = RemoveActorPacket(uniqueId);
    sendPacket(pkt2);
}

void GMLIB_Player::updateClientBossbarPercentage(int64_t bossbarId, float percentage) {
    auto pkt           = BossEventPacket();
    pkt.mBossID        = ActorUniqueID(bossbarId);
    pkt.mHealthPercent = percentage;
    pkt.mEventType     = BossEventUpdateType::UpdatePercent;
    sendPacket(pkt);
}

void GMLIB_Player::updateClientBossbarName(int64_t bossbarId, std::string const& name) {
    auto pkt       = BossEventPacket();
    pkt.mBossID    = ActorUniqueID(bossbarId);
    pkt.mName      = name;
    pkt.mEventType = BossEventUpdateType::UpdateName;
    sendPacket(pkt);
}

void GMLIB_Player::updateClientBossbarColor(int64_t bossbarId, ::BossBarColor color) {
    auto pkt       = BossEventPacket();
    pkt.mBossID    = ActorUniqueID(bossbarId);
    pkt.mColor     = color;
    pkt.mEventType = BossEventUpdateType::UpdateStyle;
    sendPacket(pkt);
}

void GMLIB_Player::setClientWeather(WeatherType weather) {
    return GMLIB_Level::getInstance()->setClientWeather(weather, this);
}

void GMLIB_Player::sendToast(std::string_view title, std::string_view message) {
    auto pkt = ToastRequestPacket(std::string(title), std::string(message));
    sendPacket(pkt);
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

void GMLIB_Player::addEffect(
    std::string const& effectType,
    int                duration,
    int                amplifier,
    bool               showParticles,
    bool               ambient,
    bool               showAnimation
) {
    if (auto effectInstance = MobEffect::getByName(effectType)) {
        auto effectId = effectInstance->getId();
        auto effect   = MobEffectInstance(effectId, duration, amplifier, ambient, showParticles, showAnimation);
        return addEffect(effect);
    }
}

void GMLIB_Player::removeEffect(MobEffect::EffectType effectType) { return removeEffect((int)effectType); }

void GMLIB_Player::removeEffect(std::string const& effectType) {
    if (auto effectInstance = MobEffect::getByName(effectType)) {
        auto effectId = effectInstance->getId();
        return removeEffect(effectId);
    }
}

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

std::optional<int> GMLIB_Player::getScore(std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->getPlayerScore(objective, this);
}

std::optional<int> GMLIB_Player::setScore(std::string const& objective, int value, PlayerScoreSetFunction action) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->setPlayerScore(objective, this, value, action);
}

bool GMLIB_Player::resetScore(std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(objective, this);
}

bool GMLIB_Player::resetScore() {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(this);
}

std::optional<int> GMLIB_Player::getPlayerScore(std::string const& serverId, std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->getPlayerScore(objective, serverId);
}

std::optional<int> GMLIB_Player::getPlayerScore(mce::UUID const& uuid, std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->getPlayerScore(objective, uuid);
}

std::optional<int> GMLIB_Player::setPlayerScore(
    std::string const&     serverId,
    std::string const&     objective,
    int                    value,
    PlayerScoreSetFunction action
) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->setPlayerScore(objective, serverId, value, action);
}

std::optional<int> GMLIB_Player::setPlayerScore(
    mce::UUID const&       uuid,
    std::string const&     objective,
    int                    value,
    PlayerScoreSetFunction action
) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->setPlayerScore(objective, uuid, value, action);
}

bool GMLIB_Player::resetPlayerScore(std::string const& serverId, std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(objective, serverId);
}

bool GMLIB_Player::resetPlayerScore(mce::UUID const& uuid, std::string const& objective) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(objective, uuid);
}

bool GMLIB_Player::resetPlayerScore(std::string const& serverId) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(serverId);
}

bool GMLIB_Player::resetPlayerScore(mce::UUID const& uuid) {
    auto scoreboard = GMLIB_Scoreboard::getInstance();
    return scoreboard->resetPlayerScore(uuid);
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

GMLIB_Actor* GMLIB_Player::shootProjectile(std::string const& typeName, float speed, float offset) {
    return GMLIB_Spawner::spawnProjectile((GMLIB_Actor*)this, typeName, speed, offset);
}

void GMLIB_Player::setFreezing(float percentage) { getEntityData().set<float>(0x78, percentage); }

void GMLIB_Player::hurtPlayer(float damage, std::string const& causeName, Actor* source) {
    auto cause = GMLIB::Mod::DamageCause::getCauseFromName(causeName);
    this->hurtByCause(damage, cause, source);
}

FullPlayerInventoryWrapper GMLIB_Player::getFullPlayerInventoryWrapper() { return FullPlayerInventoryWrapper(*this); }

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
        return I18nAPI::getCurrentLanguage()->getFullLanguageCode();
    }
    auto request = getConnectionRequest();
    if (request) {
        return request->mRawToken->mDataInfo["LanguageCode"].asString("unknown");
    }
    return "unknown";
}

void GMLIB_Player::updateClientBlock(
    BlockPos const&               pos,
    uint                          runtimeId,
    BlockUpdateFlag               flag,
    UpdateBlockPacket::BlockLayer layer
) {
    auto pkt = UpdateBlockPacket(pos, (uint)layer, runtimeId, (uchar)flag);
    sendPacket(pkt);
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

Biome* GMLIB_Player::getBiome() {
    auto& bs = getDimensionBlockSourceConst();
    return const_cast<Biome*>(&bs.getConstBiome(BlockPos(getPosition())));
}

void GMLIB_Player::sendTitle(std::string_view text, SetTitlePacket::TitleType type) {
    auto pkt = SetTitlePacket(type, std::string(text));
    sendPacket(pkt);
}

void GMLIB_Player::sendTitle(
    std::string_view          text,
    SetTitlePacket::TitleType type,
    int                       fadeInDuration,
    int                       remainDuration,
    int                       fadeOutDuration
) {
    auto pkt         = SetTitlePacket(type, std::string(text));
    pkt.mFadeInTime  = fadeInDuration;
    pkt.mFadeOutTime = fadeOutDuration;
    pkt.mStayTime    = remainDuration;
    sendPacket(pkt);
}

std::pair<BlockPos, DimensionType> GMLIB_Player::getSpawnPoint() { return {getSpawnPosition(), getSpawnDimension()}; }

void GMLIB_Player::setSpawnPoint(BlockPos pos, DimensionType dimId) { setRespawnPosition(pos, dimId); }

void GMLIB_Player::clearSpawnPoint() { clearRespawnPosition(); }

bool GMLIB_Player::hasSpawnPoint() { return hasRespawnPosition(); }

MCRESULT GMLIB_Player::executeCommand(std::string_view command) {
    CommandContext context =
        CommandContext(std::string(command), std::make_unique<PlayerCommandOrigin>(PlayerCommandOrigin(*this)));
    return ll::service::getMinecraft()->getCommands().executeCommand(context);
}

std::string GMLIB_Player::getDimensionTypeName() { return getDimension().mName; }

std::string GMLIB_Player::getDimensionName() { return VanillaDimensions::toString(getDimensionId()); }

int GMLIB_Player::giveItems(std::vector<ItemStack>& items, bool drop) {
    return Util::LootTableUtils::givePlayer(*this, items, drop);
}

int GMLIB_Player::giveItem(ItemStack const& item, bool drop, bool fixStackSize) {
    std::vector<ItemStack> items;
    if (fixStackSize) {
        auto oldItem   = item.clone();
        auto stackSize = oldItem.getMaxStackSize();
        auto count     = item.mCount;
        while (count > stackSize) {
            auto newItem   = oldItem.clone();
            newItem.mCount = stackSize;
            items.push_back(newItem);
            count -= stackSize;
        }
        oldItem.mCount = count;
        items.push_back(oldItem);
    } else {
        items.push_back(item);
    }
    return giveItems(items, drop);
}

int GMLIB_Player::giveItem(std::string_view name, int count, int data, bool drop, bool invenoryLimit) {
    if (invenoryLimit) {
        int  result = 0;
        auto items  = CommandUtils::createItemStacks(ItemInstance(name, 1, data, nullptr), count, result);
        Util::LootTableUtils::givePlayer(*this, items, drop);
        return result;
    } else {
        auto                   baseItem  = ItemStack{name};
        auto                   stackSize = baseItem.getMaxStackSize();
        std::vector<ItemStack> items;
        while (count > stackSize) {
            auto item = CommandUtils::createItemStack(std::string(name), stackSize, data);
            items.push_back(item);
            count -= stackSize;
        }
        auto item = CommandUtils::createItemStack(std::string(name), count, data);
        items.push_back(item);
        return Util::LootTableUtils::givePlayer(*this, items, drop);
    }
}

int GMLIB_Player::getItemCount(
    ItemStack const&                      item,
    std::function<bool(const ItemStack&)> comparator,
    bool                                  requireExtraData
) {
    int result = 0;

    for (auto slot = 0; slot < getInventory().getContainerSize(); slot++) {
        auto slotItem = getInventory().getItem(slot);
        auto isSame   = false;
        if (requireExtraData) {
            isSame = comparator(slotItem);
        } else {
            isSame = (slotItem.getTypeName() == item.getTypeName());
        }
        if (isSame) {
            result += slotItem.mCount;
        }
    }

    for (auto slot = 0; slot < ActorEquipment::getArmorContainer(getEntityContext()).getContainerSize(); slot++) {
        auto slotItem = ActorEquipment::getArmorContainer(getEntityContext()).getItem(slot);
        auto isSame   = false;
        if (requireExtraData) {
            isSame = comparator(slotItem);
        } else {
            isSame = (slotItem.getTypeName() == item.getTypeName());
        }
        if (isSame) {
            result += slotItem.mCount;
        }
    }

    auto& offhand = getOffhandSlot();
    auto  isSame  = false;
    if (requireExtraData) {
        isSame = comparator(offhand);
    } else {
        isSame = (offhand.getTypeName() == item.getTypeName());
    }
    if (isSame) {
        result += offhand.mCount;
    }

    return result;
}

int GMLIB_Player::getItemCount(std::string_view name, int data) {
    auto item = ItemStack{name, 1, data};
    return getItemCount(
        item,
        [&](const ItemStack& comparator) -> bool {
            return (
                item.getTypeName() == comparator.getTypeName()
                && item.getAuxValue()
                       == (comparator.hasDamageValue() ? comparator.getDamageValue() : comparator.getAuxValue())
            );
        },
        true
    );
}

int GMLIB_Player::getItemCount(std::string_view name) {
    auto item = ItemStack{name};
    return getItemCount(item);
}

int GMLIB_Player::clearItem(
    ItemStack const&                      item,
    int                                   clearCount,
    std::function<bool(const ItemStack&)> comparator,
    bool                                  requireExtraData
) {
    int result = 0;

    for (auto slot = 0; slot < getInventory().getContainerSize(); slot++) {
        auto slotItem = getInventory().getItem(slot);
        auto isSame   = false;
        if (requireExtraData) {
            isSame = comparator(slotItem);
        } else {
            isSame = (slotItem.getTypeName() == item.getTypeName());
        }
        if (isSame) {
            if (slotItem.mCount < clearCount) {
                result += slotItem.mCount;
            } else {
                result += clearCount;
            }
            getInventory().removeItem(slot, clearCount);
        }
    }

    for (auto slot = 0; slot < ActorEquipment::getArmorContainer(getEntityContext()).getContainerSize(); slot++) {
        auto slotItem = ActorEquipment::getArmorContainer(getEntityContext()).getItem(slot);
        auto isSame   = false;
        if (requireExtraData) {
            isSame = comparator(slotItem);
        } else {
            isSame = (slotItem.getTypeName() == item.getTypeName());
        }
        if (isSame) {
            if (slotItem.mCount < clearCount) {
                result += slotItem.mCount;
            } else {
                result += clearCount;
            }
            ActorEquipment::getArmorContainer(getEntityContext()).removeItem(slot, clearCount);
        }
    }

    auto offhand = getOffhandSlot();
    auto isSame  = false;
    if (requireExtraData) {
        isSame = comparator(offhand);
    } else {
        isSame = (offhand.getTypeName() == item.getTypeName());
    }
    if (isSame) {
        if (offhand.mCount < clearCount - result) {
            result += offhand.mCount;
            offhand.setNull({});
        } else {
            result += clearCount;
            offhand.remove(clearCount - result);
        }
        setEquippedSlot(Puv::Legacy::EquipmentSlot::Offhand, offhand);
    }

    refreshInventory();
    return result;
}

int GMLIB_Player::clearItem(std::string_view name, int data, int clearCount) {
    if (data < 0) {
        auto item = ItemStack{name};
        return clearItem(item, clearCount);
    } else {
        auto item = ItemStack{name, 1, data};
        return clearItem(
            item,
            clearCount,
            [&](const ItemStack& comparator) -> bool {
                return (
                    item.getTypeName() == comparator.getTypeName()
                    && item.getAuxValue()
                           == (comparator.hasDamageValue() ? comparator.getDamageValue() : comparator.getAuxValue())
                );
            },
            true
        );
    }
}

int GMLIB_Player::clearAllItems() {
    auto result = getFullPlayerInventoryWrapper().clearAllItems();
    refreshInventory();
    return result;
}

bool GMLIB_Player::isInStructureFeature(StructureFeatureType structure) {
    return getDimension().getWorldGenerator()->isStructureFeatureTypeAt(BlockPos(getPosition()), structure);
}

StructureFeatureType GMLIB_Player::getStructureFeature() {
    return getDimension().getWorldGenerator()->findStructureFeatureTypeAt(BlockPos(getPosition()));
}

bool GMLIB_Player::isInStructureFeature(std::string const& structure) {
    auto type = StructureFeatureTypeNames::getFeatureType(structure);
    return isInStructureFeature(type);
}

std::string_view GMLIB_Player::getStructureFeatureName() {
    return StructureFeatureTypeNames::getFeatureName(getStructureFeature());
}

std::optional<BlockPos>
GMLIB_Player::locateNearestStructureFeature(StructureFeatureType structure, bool useNewChunksOnly) {
    BlockPos result = {0, 64, 0};
    if (auto find =
            getDimension()
                .getWorldGenerator()
                ->findNearestStructureFeature(structure, BlockPos(getPosition()), result, useNewChunksOnly, {})) {
        return result;
    }
    return {};
}

std::optional<BlockPos>
GMLIB_Player::locateNearestStructureFeature(std::string const& structure, bool useNewChunksOnly) {
    auto type = StructureFeatureTypeNames::getFeatureType(structure);
    return locateNearestStructureFeature(type, useNewChunksOnly);
}

void GMLIB_Player::sendPacket(Packet& packet) {
    GMLIB_BinaryStream bs;
    packet.writeWithHeader(SubClientId::PrimaryClient, bs);
    bs.sendTo(*this);
}