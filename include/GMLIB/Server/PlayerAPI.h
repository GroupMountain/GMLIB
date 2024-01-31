#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Server/ActorAPI.h"
#include "mc/world/actor/player/Player.h"

class GMLIB_Player : public Player {
public:
    using Player::addEffect;
    using Player::getAllEffects;
    using Player::removeEffect;

public:
    GMLIB_API static std::vector<std::string> getAllUuids(bool includeOfflineSignedId = false);

    GMLIB_API static std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid);

    GMLIB_API static std::string getServerIdFromUuid(mce::UUID const& uuid);

    GMLIB_API static std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverId);

    GMLIB_API static bool setOfflineNbt(std::string& serverId, CompoundTag& nbt);

    GMLIB_API static std::unique_ptr<CompoundTag> getPlayerNbt(std::string& serverId); //

    GMLIB_API static std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid);

    GMLIB_API static bool setPlayerNbt(std::string& serverId, CompoundTag& nbt);

    GMLIB_API static bool setPlayerNbt(mce::UUID const& uuid, CompoundTag& nbt);

    GMLIB_API static bool
    setPlayerNbtTags(std::string& serverId, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool
    setPlayerNbtTags(mce::UUID const& uuid, CompoundTag& nbt, const std::vector<std::string>& tags);

    GMLIB_API static bool deletePlayerNbt(std::string& serverId);

    GMLIB_API static bool deletePlayerNbt(mce::UUID& uuid);

    GMLIB_API static std::optional<int> getPlayerScore(std::string& serverId, std::string objective);

    GMLIB_API static std::optional<int> getPlayerScore(mce::UUID& uuid, std::string objective);

    GMLIB_API static std::optional<int> setPlayerScore(
        std::string&           serverId,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static std::optional<int> setPlayerScore(
        mce::UUID&             uuid,
        std::string            objective,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API static bool resetPlayerScore(std::string& serverId, std::string objective);

    GMLIB_API static bool resetPlayerScore(mce::UUID& uuid, std::string objective);

    GMLIB_API static bool resetPlayerScore(std::string& serverId);

    GMLIB_API static bool resetPlayerScore(mce::UUID& uuid);

public:
    GMLIB_API std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API bool setNbt(CompoundTag& nbt);

    GMLIB_API std::optional<int> getScore(std::string objective);

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
        int64_t        bossbarId,
        std::string    name,
        float          percentage,
        ::BossBarColor color   = BossBarColor::Purple,
        int            overlay = 1
    );

    GMLIB_API int64_t
    setClientBossbar(std::string name, float percentage, ::BossBarColor color = BossBarColor::Purple, int overlay = 1);

    GMLIB_API void removeClientBossbar(int64_t bossbarId);

    GMLIB_API void
    updateClientBossbar(int64_t bossbarId, std::string name, float percentage, ::BossBarColor color, int overlay);

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

    // ToDo API
    // If you need any API, please open an issue on https://github.com/GroupMountain/GMLIB/issues

    // GMLIB_API bool giveItem(ItemStack& item, bool drop = false);

    // GMLIB_API bool giveItem(std::string name, int count = 1, short aux = 0, bool drop = false);

    // GMLIB_API int clearItem(std::string name, int count = 1, short aux = -1);

    // GMLIB_API int hasItem(std::string name, short aux = -1);
};