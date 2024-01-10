#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::PlayerAPI {

GMLIB_API std::vector<std::string> getAllUuids(bool includeOfflineSignedId = false);
GMLIB_API std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid);
GMLIB_API std::string getServeridFromUuid(mce::UUID const& uuid);

GMLIB_API std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverid);
GMLIB_API bool                         setOfflineNbt(std::string& serverid, CompoundTag* nbt);

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid);
GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(Player* pl);
GMLIB_API bool                         setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt);
GMLIB_API bool                         setPlayerNbt(Player* pl, CompoundTag* nbt);
GMLIB_API void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags);
GMLIB_API bool setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags);
GMLIB_API bool deletePlayerNbt(std::string& serverid);

GMLIB_API void setClientSidebar(
    Player*                                         player,
    const std::string&                              title,
    const std::vector<std::pair<std::string, int>>& data,
    ObjectiveSortOrder                              sortOrder = ObjectiveSortOrder::Ascending
);
GMLIB_API void removeClientSidebar(Player* player);

GMLIB_API void setHealth(Player* player, int value);
GMLIB_API void setMaxHealth(Player* player, int value);
GMLIB_API void setAbsorption(Player* player, int value);
GMLIB_API void setMaxAbsorption(Player* player, int value);
GMLIB_API void setAttackDamage(Player* player, int value);
GMLIB_API void setJumpStrength(Player* player, int value);
GMLIB_API void setKnockbackResistance(Player* player, int value);
GMLIB_API void setLavaMovementSpeed(Player* player, int value);
GMLIB_API void setLuck(Player* player, int value);
GMLIB_API void setMovementSpeed(Player* player, int value);
GMLIB_API void setUnderwaterMovementSpeed(Player* player, int value);

GMLIB_API void setClientGamemode(Player* player, GameType gamemode);

GMLIB_API void setClientBossbar(
    Player*        player,
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
);
GMLIB_API void removeClientBossbar(Player* player, int64_t bossbarId);
GMLIB_API void updateClientBossbar(
    Player*        player,
    int64_t        bossbarId,
    std::string    name,
    float          percentage,
    ::BossBarColor color,
    int            overlay
);

GMLIB_API void addEffect(
    Player*               player,
    MobEffect::EffectType effectType,
    int                   duration      = 600,
    int                   amplifier     = 0,
    bool                  showParticles = true,
    bool                  ambient       = false,
    bool                  showAnimation = false
);
GMLIB_API void removeEffect(Player* player, MobEffect::EffectType effectType);
GMLIB_API void removeAllEffects(Player* player);

GMLIB_API std::vector<MobEffectInstance> getAllEffects(Player* player);


} // namespace GMLIB::PlayerAPI
