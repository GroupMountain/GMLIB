#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::PlayerAPI {

GMLIB_API std::vector<std::string> getAllUuids(bool includeOfflineSignedId = false);

GMLIB_API std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid);

GMLIB_API std::string getServeridFromUuid(mce::UUID const& uuid);

GMLIB_API std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverid);

GMLIB_API bool setOfflineNbt(std::string& serverid, CompoundTag* nbt);

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid);

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(Player* pl);

GMLIB_API bool setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt);

GMLIB_API bool setPlayerNbt(Player* pl, CompoundTag* nbt);

GMLIB_API void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags);

GMLIB_API bool setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags);

GMLIB_API bool deletePlayerNbt(std::string& serverid);

} // namespace GMLIB::PlayerAPI
