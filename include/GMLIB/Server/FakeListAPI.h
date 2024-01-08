#pragma once
#include "GMLIB/DllExport.h"
#include "mc/network/packet/PlayerListEntry.h"
namespace GMLIB::FakeListAPI{
    GMLIB_API bool addFakeList(PlayerListEntry entry);

    GMLIB_API bool addFakeList(std::string name, std::string xuid, long long auid, mce::UUID UUID);

    GMLIB_API bool removeFakeList(std::string nameOrXuid);

    GMLIB_API void removeAllFakeLists();

    GMLIB_API bool checkFakeListExists(std::string name,std::string xuid);

    GMLIB_API std::vector<std::string> externAllFakeListName();

    //GMLIB_API

    //GMLIB_API

    //GMLIB_API
}