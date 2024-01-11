#pragma once
#include "GMLIB/DllExport.h"
#include "mc/network/packet/PlayerListEntry.h"

namespace GMLIB::FakeListAPI {

GMLIB_API bool addFakeList(PlayerListEntry entry);

GMLIB_API bool
addFakeList(std::string name, std::string xuid, ActorUniqueID uniqueId, mce::UUID uuid = mce::UUID::random());

GMLIB_API bool removeFakeList(std::string nameOrXuid);

GMLIB_API void removeAllFakeLists();

GMLIB_API PlayerListEntry getFakeList(std::string name);

GMLIB_API bool checkFakeListExistsName(std::string name);

GMLIB_API bool checkFakeListExists(std::string name, std::string xuid);

GMLIB_API std::vector<std::string> getAllFakeNames();

GMLIB_API void replaceList(bool add, std::string oldName, std::string newName);

GMLIB_API void setSimulatedPlayerOpt(bool set);

GMLIB_API bool getSimulatedPlayerOpt();

} // namespace GMLIB::FakeListAPI