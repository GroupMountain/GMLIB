#pragma once
#include "GMLIB/Macros.h"
#include "mc/network/packet/PlayerListEntry.h"

namespace GMLIB::Server {

class FakeList {
public:
    GMLIB_API static bool addFakeList(PlayerListEntry const& entry);

    GMLIB_API static bool addFakeList(
        std::string const&   name,
        std::string const&   xuid,
        ActorUniqueID const& uniqueId,
        mce::UUID const&     uuid = mce::UUID::random()
    );

    GMLIB_API static bool removeFakeList(std::string const& nameOrXuid);

    GMLIB_API static void removeAllFakeLists();

    GMLIB_API static PlayerListEntry getFakeList(std::string const& name);

    GMLIB_API static bool checkFakeListExistsName(std::string const& name);

    GMLIB_API static bool checkFakeListExists(std::string const& name, std::string const& xuid);

    GMLIB_NDAPI static std::vector<std::string> getAllFakeNames();

    GMLIB_API static void setListName(std::string const& realName, std::string const& fakeName);

    GMLIB_API static void resetListName(std::string const& realName);

    GMLIB_API static void setSimulatedPlayerListOptimizeEnabled(bool value = true);

    GMLIB_API static bool getSimulatedPlayerListOptimizeEnabled();

    // ToDo API
    // If you need any API, please open an issue on https://github.com/GroupMountain/GMLIB/issues

    // GMLIB_API static bool setFakePermission();

    // GMLIB_API static bool removePlayerFromList();
};

} // namespace GMLIB::Server