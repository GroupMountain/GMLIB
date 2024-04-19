#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Server/UserCache.h>
#include <mc/network/packet/LoginPacket.h>

namespace GMLIB::Server::UserCache {

nlohmann::json mUserCache;

void saveUserCacheFile() {
    std::string path = "./usercache.json";
    GMLIB::Files::JsonFile::writeFile(path, mUserCache);
}

void updateUserCache(mce::UUID const& uuid, std::string& xuid, std::string& realName) {
    for (auto& key : mUserCache) {
        if (key["uuid"] == uuid.asString()) {
            key["uuid"]     = uuid.asString();
            key["xuid"]     = xuid;
            key["realName"] = realName;
            saveUserCacheFile();
            return;
        }
    }
    auto info        = nlohmann::json::object();
    info["uuid"]     = uuid.asString();
    info["xuid"]     = xuid;
    info["realName"] = realName;
    mUserCache.push_back(info);
    saveUserCacheFile();
}

std::optional<nlohmann::json> tryFindCacheInfoFromUuid(std::string& uuid) {
    for (auto& key : mUserCache) {
        if (key["uuid"] == uuid) {
            return key;
        }
    }
    return {};
}

std::optional<nlohmann::json> tryFindCacheInfoFromXuid(std::string& xuid) {
    for (auto& key : mUserCache) {
        if (key["xuid"] == xuid) {
            return key;
        }
    }
    return {};
}

std::optional<nlohmann::json> tryFindCacheInfoFromName(std::string& name) {
    for (auto& key : mUserCache) {
        if (key["realName"] == name) {
            return key;
        }
    }
    return {};
}

std::optional<std::string> getXuidByUuid(std::string& uuid) {
    auto info = tryFindCacheInfoFromUuid(uuid);
    if (info.has_value()) {
        auto res = info.value().at("xuid").get<std::string>();
    }
    return {};
}

std::optional<std::string> getNameByUuid(std::string& uuid) {
    auto info = tryFindCacheInfoFromUuid(uuid);
    if (info.has_value()) {
        auto res = info.value().at("realName").get<std::string>();
    }
    return {};
}

std::optional<std::string> getUuidByXuid(std::string& xuid) {
    auto info = tryFindCacheInfoFromUuid(xuid);
    if (info.has_value()) {
        auto res = info.value().at("uuid").get<std::string>();
    }
    return {};
}

std::optional<std::string> getNameByXuid(std::string& xuid) {
    auto info = tryFindCacheInfoFromUuid(xuid);
    if (info.has_value()) {
        auto res = info.value().at("realName").get<std::string>();
    }
    return {};
}

std::optional<std::string> getXuidByName(std::string& name) {
    auto info = tryFindCacheInfoFromUuid(name);
    if (info.has_value()) {
        auto res = info.value().at("xuid").get<std::string>();
    }
    return {};
}

std::optional<std::string> getUuidByName(std::string& name) {
    auto info = tryFindCacheInfoFromUuid(name);
    if (info.has_value()) {
        auto res = info.value().at("uuid").get<std::string>();
    }
    return {};
}

LL_TYPE_INSTANCE_HOOK(
    PlayerLoginHook,
    ll::memory::HookPriority::Highest,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVLoginPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class LoginPacket const&       packet
) {
    origin(source, packet);
    auto cert     = packet.mConnectionRequest->getCertificate();
    auto uuid     = ExtendedCertificate::getIdentity(*cert);
    auto xuid     = ExtendedCertificate::getXuid(*cert, false);
    auto realName = ExtendedCertificate::getIdentityName(*cert);
    GMLIB::Server::UserCache::updateUserCache(uuid, xuid, realName);
}

struct UserCache_Impl {
    ll::memory::HookRegistrar<PlayerLoginHook> r;
};

std::unique_ptr<UserCache_Impl> impl;

void initUserCache() {
    auto emptyFile = nlohmann::json::array();
    try {
        mUserCache = GMLIB::Files::JsonFile::initJson("./usercache.json", emptyFile);
    } catch (...) {
        mUserCache = emptyFile;
        saveUserCacheFile();
    }
}

void enableUserCache() {
    if (!impl) {
        impl = std::make_unique<UserCache_Impl>();
        initUserCache();
    }
}

} // namespace GMLIB::Server::UserCache