#pragma once
#include "GMLIB/Macros.h"
#include "GMLIB/Server/ActorAPI.h"
#include "GMLIB/Server/LevelAPI.h"
#include "mc/codebuilder/MCRESULT.h"
#include "mc/enums/BossBarColor.h"
#include "mc/enums/BossBarOverlay.h"
#include "mc/enums/ObjectiveSortOrder.h"
#include "mc/network/packet/Packet.h"
#include "mc/network/packet/SetTitlePacket.h"
#include "mc/network/packet/UpdateBlockPacket.h"
#include "mc/world/actor/player/FullPlayerInventoryWrapper.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/actor/player/PlayerScoreSetFunction.h"
#include "mc/world/inventory/transaction/InventoryTransactionManager.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/levelgen/structure/StructureFeatureType.h"

class GMLIB_Player : public Player {
public:
    using Player::addEffect;
    using Player::getAllEffects;
    using Player::removeEffect;

public:
    GMLIB_NDAPI static std::vector<std::string> getAllServerIds();

    GMLIB_NDAPI static std::vector<mce::UUID> getAllUuids(bool includeOfflineSignedId = false);

    GMLIB_NDAPI static std::unordered_map<mce::UUID, std::string>
    getUuidToServerIdMap(bool includeOfflineSignedId = false);

    GMLIB_NDAPI static std::unordered_map<std::string, mce::UUID> getServerIdToUuidMap();

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid);

    GMLIB_API static bool deleteUuidDBTag(mce::UUID const& uuid);

    GMLIB_NDAPI static std::string getServerIdFromUuid(mce::UUID const& uuid);

    GMLIB_NDAPI static mce::UUID getUuidFromServerId(std::string const& serverId);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getOfflineNbt(std::string const& serverId);

    GMLIB_API static bool setOfflineNbt(std::string const& serverId, CompoundTag& nbt);

    GMLIB_API static bool createNewPlayerTag(mce::UUID const& uuid, std::string const& serverId);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getPlayerNbt(std::string const& serverId);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID const& uuid);

    GMLIB_API static bool setPlayerNbt(std::string const& serverId, CompoundTag& nbt);

    GMLIB_API static bool setPlayerNbt(mce::UUID const& uuid, CompoundTag& nbt, bool forceCreate = true);

    GMLIB_API static bool
    setPlayerNbtTags(std::string const& serverId, CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_API static bool
    setPlayerNbtTags(mce::UUID const& uuid, CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_API static bool deletePlayerNbt(std::string const& serverId);

    GMLIB_API static bool deletePlayerNbt(mce::UUID const& uuid);

    GMLIB_API static bool deletePlayer(std::string const& serverId);

    GMLIB_API static bool deletePlayer(mce::UUID const& uuid);

    GMLIB_NDAPI static ActorUniqueID getPlayerUniqueID(std::string const& serverId);

    GMLIB_NDAPI static ActorUniqueID getPlayerUniqueID(mce::UUID const& uuid);

    GMLIB_NDAPI static std::unordered_map<int64, std::string> getUniqueIdToServerIdMap();

    GMLIB_NDAPI static std::unordered_map<int64, mce::UUID> getUniqueIdToUuidMap();

    GMLIB_NDAPI static std::optional<int> getPlayerScore(std::string const& serverId, std::string const& objective);

    GMLIB_NDAPI static std::optional<int> getPlayerScore(mce::UUID const& uuid, std::string const& objective);

    GMLIB_API static std::optional<int> setPlayerScore(
        std::string const&     serverId,
        std::string const&     objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static std::optional<int> setPlayerScore(
        mce::UUID const&       uuid,
        std::string const&     objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static bool resetPlayerScore(std::string const& serverId, std::string const& objective);

    GMLIB_API static bool resetPlayerScore(mce::UUID const& uuid, std::string const& objective);

    GMLIB_API static bool resetPlayerScore(std::string const& serverId);

    GMLIB_API static bool resetPlayerScore(mce::UUID const& uuid);

    GMLIB_NDAPI static std::optional<std::pair<Vec3, int>> getPlayerPosition(std::string const& serverId);

    GMLIB_NDAPI static std::optional<std::pair<Vec3, int>> getPlayerPosition(mce::UUID const& uuid);

    GMLIB_API static bool setPlayerPosition(std::string const& serverId, Vec3 const& pos, DimensionType dimId);

    GMLIB_API static bool setPlayerPosition(mce::UUID const& uuid, Vec3 const& pos, DimensionType dimId);

public:
    GMLIB_NDAPI std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API bool setNbtTags(CompoundTag& nbt, std::vector<std::string> const& tags);

    GMLIB_NDAPI std::string getDimensionName();

    GMLIB_NDAPI std::string getDimensionTypeName();

    GMLIB_NDAPI std::optional<int> getScore(std::string const& objective);

    GMLIB_NDAPI std::string_view getIP();

    GMLIB_NDAPI ushort getPort();

    GMLIB_NDAPI int getAvgPing();

    GMLIB_NDAPI int getLastPing();

    GMLIB_NDAPI std::string_view getLanguageCode();

    GMLIB_API std::optional<int>
    setScore(std::string const& objective, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API bool resetScore(std::string const& objective);

    GMLIB_API optional_ref<Actor> shootProjectile(std::string_view typeName, float speed = 2, float offset = 3);

    GMLIB_API bool resetScore();

    GMLIB_API void setClientSidebar(
        std::string const&                              title,
        std::vector<std::pair<std::string, int>> const& data,
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
        int64_t            bossbarId,
        std::string const& name,
        float              percentage,
        ::BossBarColor     color   = BossBarColor::Purple,
        ::BossBarOverlay   overlay = BossBarOverlay::Progress
    );

    GMLIB_API int64_t setClientBossbar(
        std::string const& name,
        float              percentage = 1.00f,
        ::BossBarColor     color      = BossBarColor::Purple,
        ::BossBarOverlay   overlay    = BossBarOverlay::Progress
    );

    GMLIB_API void removeClientBossbar(int64_t bossbarId);

    GMLIB_API void updateClientBossbarPercentage(int64_t bossbarId, float percentage);

    GMLIB_API void updateClientBossbarName(int64_t bossbarId, std::string const& name);

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

    GMLIB_API void addEffect(
        std::string const& effectType,
        int                duration      = 600,
        int                amplifier     = 0,
        bool               showParticles = true,
        bool               ambient       = false,
        bool               showAnimation = false
    );

    GMLIB_API void removeEffect(MobEffect::EffectType effectType);

    GMLIB_API void removeEffect(std::string const& effectType);

    GMLIB_NDAPI std::vector<MobEffectInstance> getAllEffects();

    GMLIB_NDAPI ItemStack& getMainHandSlot();

    GMLIB_API void setMainHandSlot(ItemStack& itemStack);

    GMLIB_NDAPI ItemStack& getOffHandSlot();

    GMLIB_API void setOffHandSlot(ItemStack& itemStack);

    GMLIB_API void setFreezing(float percentage = 1.0f);

    GMLIB_NDAPI FullPlayerInventoryWrapper getFullPlayerInventoryWrapper();

    GMLIB_API void
    hurtPlayer(float damage, std::string const& causeName = "override", optional_ref<Actor> source = nullptr);

    GMLIB_API void updateClientBlock(
        BlockPos const&               pos,
        uint                          runtimeId,
        BlockUpdateFlag               flag  = BlockUpdateFlag::All,
        UpdateBlockPacket::BlockLayer layer = UpdateBlockPacket::BlockLayer::Standard
    );

    GMLIB_API void updateClientBlock(
        BlockPos const&               pos,
        Block const&                  block,
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

    GMLIB_NDAPI Biome& getBiome();

    GMLIB_NDAPI std::pair<BlockPos, DimensionType> getSpawnPoint();

    GMLIB_API void setSpawnPoint(BlockPos pos, DimensionType dimId);

    GMLIB_API void clearSpawnPoint();

    GMLIB_API bool hasSpawnPoint();

    GMLIB_API MCRESULT executeCommand(std::string_view command);

    GMLIB_API int giveItems(std::vector<ItemStack>& items, bool drop = true);

    GMLIB_API int giveItem(ItemStack const& item, bool drop = true, bool fixStackSize = true);

    GMLIB_API int
    giveItem(std::string_view name, int count = 1, int data = 0, bool drop = true, bool inventoryLimit = true);

    GMLIB_API int clearAllItems();

    GMLIB_NDAPI int getItemCount(
        ItemStack const&                      item,
        std::function<bool(const ItemStack&)> comparator       = nullptr,
        bool                                  requireExtraData = false
    );

    GMLIB_NDAPI int getItemCount(std::string_view name, int data);

    GMLIB_NDAPI int getItemCount(std::string_view name);

    GMLIB_API int clearItem(
        ItemStack const&                      item,
        int                                   clearCount,
        std::function<bool(const ItemStack&)> comparator       = nullptr,
        bool                                  requireExtraData = false
    );

    GMLIB_API int clearItem(std::string_view name, int data = -1, int count = 2147483647);

    GMLIB_API bool isInStructureFeature(StructureFeatureType structure);

    GMLIB_API bool isInStructureFeature(std::string const& structure);

    GMLIB_NDAPI StructureFeatureType getStructureFeature();

    GMLIB_NDAPI std::string_view getStructureFeatureName();

    GMLIB_NDAPI std::optional<BlockPos>
                locateNearestStructureFeature(StructureFeatureType structure, bool useNewChunksOnly = false);

    GMLIB_NDAPI std::optional<BlockPos>
                locateNearestStructureFeature(std::string const& structure, bool useNewChunksOnly = false);

    GMLIB_API void sendPacket(Packet& packet);

    GMLIB_API void sendText(std::string_view message);

    GMLIB_API void sendText(std::string const& message, std::vector<std::string> const& params);

    GMLIB_API void talkAs(std::string_view message);
};