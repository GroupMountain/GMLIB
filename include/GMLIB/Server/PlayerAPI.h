#pragma once
#include "GMLIB/Macros.h"
#include "GMLIB/Server/ActorAPI.h"
#include "GMLIB/Server/LevelAPI.h"
#include "mc/codebuilder/MCRESULT.h"
#include "mc/enums/BossBarColor.h"
#include "mc/enums/BossBarOverlay.h"
#include "mc/enums/ObjectiveSortOrder.h"
#include "mc/network/packet/SetTitlePacket.h"
#include "mc/network/packet/UpdateBlockPacket.h"
#include "mc/world/actor/player/FullPlayerInventoryWrapper.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/actor/player/PlayerScoreSetFunction.h"
#include "mc/world/inventory/transaction/InventoryTransactionManager.h"
#include "mc/world/level/block/Block.h"

class GMLIB_Player : public Player {
public:
    using Player::addEffect;
    using Player::getAllEffects;
    using Player::removeEffect;

public:
    GMLIB_API static std::vector<std::string> getAllServerIds();

    GMLIB_API static std::vector<mce::UUID> getAllUuids(bool includeOfflineSignedId = false);

    GMLIB_API static std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid);

    GMLIB_API static bool deleteUuidDBTag(mce::UUID const& uuid);

    GMLIB_API static std::string getServerIdFromUuid(mce::UUID const& uuid);

    GMLIB_API static std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverId);

    GMLIB_API static bool setOfflineNbt(std::string& serverId, CompoundTag& nbt);

    GMLIB_API static bool createNewPlayerTag(mce::UUID const& uuid, std::string serverId);

    GMLIB_API static std::unique_ptr<CompoundTag> getPlayerNbt(std::string& serverId);

    GMLIB_API static std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID const& uuid);

    GMLIB_API static bool setPlayerNbt(std::string& serverId, CompoundTag& nbt);

    GMLIB_API static bool setPlayerNbt(mce::UUID const& uuid, CompoundTag& nbt, bool forceCreate = true);

    GMLIB_API static bool
    setPlayerNbtTags(std::string& serverId, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool
    setPlayerNbtTags(mce::UUID const& uuid, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool deletePlayerNbt(std::string& serverId);

    GMLIB_API static bool deletePlayerNbt(mce::UUID const& uuid);

    GMLIB_API static ActorUniqueID getPlayerUniqueID(std::string& serverId);

    GMLIB_API static ActorUniqueID getPlayerUniqueID(mce::UUID const& uuid);

    GMLIB_API static std::unordered_map<ActorUniqueID, std::string> getUniqueIdToServerIdMap();

    GMLIB_API static std::unordered_map<ActorUniqueID, mce::UUID> getUniqueIdToUuidMap();

    GMLIB_API static std::optional<int> getPlayerScore(std::string& serverId, std::string objective);

    GMLIB_API static std::optional<int> getPlayerScore(mce::UUID const& uuid, std::string objective);

    GMLIB_API static std::optional<int> setPlayerScore(
        std::string&           serverId,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static std::optional<int> setPlayerScore(
        mce::UUID const&       uuid,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static bool resetPlayerScore(std::string& serverId, std::string objective);

    GMLIB_API static bool resetPlayerScore(mce::UUID const& uuid, std::string objective);

    GMLIB_API static bool resetPlayerScore(std::string& serverId);

    GMLIB_API static bool resetPlayerScore(mce::UUID const& uuid);

    GMLIB_API static std::optional<std::pair<Vec3, int>> getPlayerPosition(std::string& serverId);

    GMLIB_API static std::optional<std::pair<Vec3, int>> getPlayerPosition(mce::UUID const& uuid);

    GMLIB_API static bool setPlayerPosition(std::string& serverId, Vec3 pos, DimensionType dimId);

    GMLIB_API static bool setPlayerPosition(mce::UUID const& uuid, Vec3 pos, DimensionType dimId);

public:
    GMLIB_API std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API bool setNbtTags(CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API std::optional<int> getScore(std::string objective);

    GMLIB_API std::string_view getIP();

    GMLIB_API ushort getPort();

    GMLIB_API int getAvgPing();

    GMLIB_API int getLastPing();

    GMLIB_API std::string_view getLanguageCode();

    GMLIB_API std::optional<int>
              setScore(std::string objective, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API bool resetScore(std::string objective);

    GMLIB_API GMLIB_Actor* shootProjectile(std::string typeName, float speed = 2, float offset = 3);

    GMLIB_API bool resetScore();

    GMLIB_API void setClientSidebar(
        const std::string                               title,
        const std::vector<std::pair<std::string, int>>& data,
        ObjectiveSortOrder                              sortOrder = ObjectiveSortOrder::Ascending
    );

    GMLIB_API void removeClientSidebar();

    GMLIB_API void setHealth(int value);

    GMLIB_API void setMaxHealth(int value);

    GMLIB_API void setAbsorption(int value);

    GMLIB_API void setMaxAbsorption(int value);

    GMLIB_API void setAttackDamage(int value);

    GMLIB_API void setJumpStrength(int value);

    GMLIB_API void setKnockbackResistance(int value);

    GMLIB_API void setLavaMovementSpeed(int value);

    GMLIB_API void setLuck(int value);

    GMLIB_API void setMovementSpeed(int value);

    GMLIB_API void setUnderwaterMovementSpeed(int value);

    GMLIB_API void setClientGamemode(GameType gamemode);

    GMLIB_API void setClientBossbar(
        int64_t          bossbarId,
        std::string      name,
        float            percentage,
        ::BossBarColor   color   = BossBarColor::Purple,
        ::BossBarOverlay overlay = BossBarOverlay::Progress
    );

    GMLIB_API int64_t setClientBossbar(
        std::string      name,
        float            percentage = 1.00f,
        ::BossBarColor   color      = BossBarColor::Purple,
        ::BossBarOverlay overlay    = BossBarOverlay::Progress
    );

    GMLIB_API void removeClientBossbar(int64_t bossbarId);

    GMLIB_API void updateClientBossbarPercentage(int64_t bossbarId, float percentage);

    GMLIB_API void updateClientBossbarName(int64_t bossbarId, std::string name);

    GMLIB_API void updateClientBossbarColor(int64_t bossbarId, ::BossBarColor color);

    GMLIB_API void setClientWeather(WeatherType weather);

    GMLIB_API void sendToast(std::string_view title, std::string_view message);

    GMLIB_API void sendTitle(std::string_view title, SetTitlePacket::TitleType type = SetTitlePacket::TitleType::Title);

    GMLIB_API void sendTitle(
        std::string_view          title,
        SetTitlePacket::TitleType type,
        int                       fadeInDuration,
        int                       remainDuration,
        int                       fadeOutDuration
    );

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

    GMLIB_API ItemStack* getMainHandSlot();

    GMLIB_API void setMainHandSlot(ItemStack& itemStack);

    GMLIB_API ItemStack* getOffHandSlot();

    GMLIB_API void setOffHandSlot(ItemStack& itemStack);

    GMLIB_API void setFreezing(float percentage = 1.0f);

    GMLIB_API InventoryTransactionManager* getInventoryTransactionManager();

    GMLIB_API FullPlayerInventoryWrapper getFullPlayerInventoryWrapper();

    GMLIB_API void hurtPlayer(float damage, std::string causeName = "override", Actor* source = nullptr);

    GMLIB_API int clearAllItems();

    GMLIB_API void updateClientBlock(
        BlockPos const&               pos,
        uint                          runtimeId,
        BlockUpdateFlag               flag  = BlockUpdateFlag::All,
        UpdateBlockPacket::BlockLayer layer = UpdateBlockPacket::BlockLayer::Standard
    );

    GMLIB_API bool updateClientBlock(
        BlockPos const&               pos,
        Block*                        block,
        BlockUpdateFlag               flag  = BlockUpdateFlag::All,
        UpdateBlockPacket::BlockLayer layer = UpdateBlockPacket::BlockLayer::Standard
    );

    GMLIB_API bool updateClientBlock(
        BlockPos const&               pos,
        std::string_view              blockName,
        Block::BlockStatesType        blockState,
        BlockUpdateFlag               flag  = BlockUpdateFlag::All,
        UpdateBlockPacket::BlockLayer layer = UpdateBlockPacket::BlockLayer::Standard
    );

    GMLIB_API bool updateClientBlock(
        BlockPos const&               pos,
        std::string_view              blockName,
        ushort                        auxValue = 0,
        BlockUpdateFlag               flag     = BlockUpdateFlag::All,
        UpdateBlockPacket::BlockLayer layer    = UpdateBlockPacket::BlockLayer::Standard
    );

    GMLIB_API Biome* getBiome();

    GMLIB_API std::pair<BlockPos, DimensionType> getSpawnPoint();

    GMLIB_API void setSpawnPoint(BlockPos pos, DimensionType dimId);

    GMLIB_API void clearSpawnPoint();

    GMLIB_API bool hasSpawnPoint();

    GMLIB_API MCRESULT executeCommand(std::string_view command);

    GMLIB_API bool giveItem(ItemStack& item, bool drop = true);

    // GMLIB_API bool giveItem(std::string name, int count = 1, short aux = 0, bool drop = true);

    // GMLIB_API int clearItem(std::string name, int count = -1, short aux = -1);

    // GMLIB_API int hasItem(std::string name, short aux = -1);
};