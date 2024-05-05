#pragma once
#include "GMLIB/Macros.h"
#include "mc/deps/core/mce/UUID.h"
#include "nlohmann/json.hpp"

namespace GMLIB {

class UserCache {
public:
    struct UserCacheEntry {
        mce::UUID const   mUuid;
        std::string const mName;
        std::string const mXuid;
    };

public:
    GMLIB_API static UserCacheEntry* fromUuid(mce::UUID const& uuid);

    GMLIB_API static UserCacheEntry* fromXuid(std::string const& xuid);

    GMLIB_API static UserCacheEntry* fromName(std::string const& name);

    GMLIB_API static void forEach(std::function<void(UserCacheEntry const&)> const& func);

    GMLIB_API static std::optional<std::string> getXuidByUuid(mce::UUID const& uuid);

    GMLIB_API static std::optional<std::string> getNameByUuid(mce::UUID const& uuid);

    GMLIB_API static std::optional<mce::UUID> getUuidByXuid(std::string const& xuid);

    GMLIB_API static std::optional<std::string> getNameByXuid(std::string const& xuid);

    GMLIB_API static std::optional<std::string> getXuidByName(std::string const& name);

    GMLIB_API static std::optional<mce::UUID> getUuidByName(std::string const& name);
};

} // namespace GMLIB