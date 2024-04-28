#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Server::UserCache {

GMLIB_API void enableUserCache();

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromUuid(std::string const& uuid);

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromXuid(std::string const& xuid);

GMLIB_API std::optional<nlohmann::json> tryFindCacheInfoFromName(std::string const& name);

GMLIB_API std::optional<std::string> getXuidByUuid(std::string const& uuid);

GMLIB_API std::optional<std::string> getNameByUuid(std::string const& uuid);

GMLIB_API std::optional<std::string> getUuidByXuid(std::string const& xuid);

GMLIB_API std::optional<std::string> getNameByXuid(std::string const& xuid);

GMLIB_API std::optional<std::string> getXuidByName(std::string const& name);

GMLIB_API std::optional<std::string> getUuidByName(std::string const& name);

} // namespace GMLIB::Server::UserCache