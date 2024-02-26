#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Server::UserCache {

GMLIB_API void enableUserCache();

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromUuid(std::string& uuid);

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromXuid(std::string& xuid);

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromName(std::string& name);

GMLIB_API std::optional<std::string> getXuidByUuid(std::string& uuid);

GMLIB_API std::optional<std::string> getNameByUuid(std::string& uuid);

GMLIB_API std::optional<std::string> getUuidByXuid(std::string& xuid);

GMLIB_API std::optional<std::string> getNameByXuid(std::string& xuid);

GMLIB_API std::optional<std::string> getXuidByName(std::string& name);

GMLIB_API std::optional<std::string> getUuidByName(std::string& name);

} // namespace GMLIB::Server::UserCache