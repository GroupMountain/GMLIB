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

GMLIB_API void setSidebar(
    Player*                                         player,
    const std::string&                              title,
    const std::vector<std::pair<std::string, int>>& data,
    ObjectiveSortOrder                              sortOrder = ObjectiveSortOrder::Ascending
);
GMLIB_API void removeSidebar(Player* player);

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

} // namespace GMLIB::PlayerAPI
