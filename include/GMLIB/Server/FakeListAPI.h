#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/network/packet/PlayerListEntry.h"

class GMLIB_FakeList {
public:
    GMLIB_API static bool addFakeList(PlayerListEntry entry);

    GMLIB_API static bool
    addFakeList(std::string name, std::string xuid, ActorUniqueID uniqueId, mce::UUID uuid = mce::UUID::random());

    GMLIB_API static bool removeFakeList(std::string nameOrXuid);

    GMLIB_API static void removeAllFakeLists();

    GMLIB_API static PlayerListEntry getFakeList(std::string name);

    GMLIB_API static bool checkFakeListExistsName(std::string name);

    GMLIB_API static bool checkFakeListExists(std::string name, std::string xuid);

    GMLIB_API static std::vector<std::string> getAllFakeNames();

    GMLIB_API static void setListName(std::string realName, std::string fakeName);

    GMLIB_API static void resetListName(std::string realName);

    GMLIB_API static void setSimulatedPlayerListOptimizeEnabled(bool value = true);

    GMLIB_API static bool getSimulatedPlayerListOptimizeEnabled();
};