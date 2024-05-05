#include "Global.h"
#include "parallel_hashmap/phmap.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Server/StorageAPI.h>
#include <GMLIB/Server/UserCache.h>
#include <mc/network/packet/LoginPacket.h>

namespace GMLIB {

phmap::flat_hash_map<mce::UUID, std::shared_ptr<UserCache::UserCacheEntry>>   mUuidEntry;
phmap::flat_hash_map<std::string, std::shared_ptr<UserCache::UserCacheEntry>> mNameEntry;
phmap::flat_hash_map<std::string, std::shared_ptr<UserCache::UserCacheEntry>> mXuidEntry;

UserCache::UserCacheEntry* UserCache::fromUuid(mce::UUID const& uuid) {
    if (mUuidEntry.contains(uuid)) {
        return mUuidEntry[uuid].get();
    }
    return nullptr;
}

UserCache::UserCacheEntry* UserCache::fromXuid(std::string const& xuid) {
    if (mXuidEntry.contains(xuid)) {
        return mXuidEntry[xuid].get();
    }
    return nullptr;
}

UserCache::UserCacheEntry* UserCache::fromName(std::string const& name) {
    if (mNameEntry.contains(name)) {
        return mNameEntry[name].get();
    }
    return nullptr;
}

std::optional<std::string> UserCache::getXuidByUuid(mce::UUID const& uuid) {
    if (auto entry = fromUuid(uuid)) {
        return entry->mXuid;
    }
    return {};
}

std::optional<std::string> UserCache::getNameByUuid(mce::UUID const& uuid) {
    if (auto entry = fromUuid(uuid)) {
        return entry->mName;
    }
    return {};
}

std::optional<mce::UUID> UserCache::getUuidByXuid(std::string const& xuid) {
    if (auto entry = fromUuid(xuid)) {
        return entry->mUuid;
    }
    return {};
}

std::optional<std::string> UserCache::getNameByXuid(std::string const& xuid) {
    if (auto entry = fromUuid(xuid)) {
        return entry->mName;
    }
    return {};
}

std::optional<std::string> UserCache::getXuidByName(std::string const& name) {
    if (auto entry = fromUuid(name)) {
        return entry->mXuid;
    }
    return {};
}

std::optional<mce::UUID> UserCache::getUuidByName(std::string const& name) {
    if (auto entry = fromUuid(name)) {
        return entry->mUuid;
    }
    return {};
}

void UserCache::forEach(std::function<void(UserCache::UserCacheEntry const&)> const& func) {
    StorageAPI::getInstance()->forEachKeyWithPrefix(
        "GMLIB_UserCache_",
        DBHelpers::Category::All,
        [&func](std::string_view key, std::string_view data) {
            auto uuid  = mce::UUID::fromString(std::string(key));
            auto info  = CompoundTag::fromBinaryNbt(data);
            auto xuid  = info->getString("xuid");
            auto name  = info->getString("name");
            auto entry = UserCache::UserCacheEntry(std::move(uuid), std::move(name), std::move(xuid));
            func(entry);
        }
    );
}

void updateUserCache(const Certificate* cert) {
    auto uuid        = ExtendedCertificate::getIdentity(*cert);
    auto xuid        = ExtendedCertificate::getXuid(*cert, false);
    auto name        = ExtendedCertificate::getIdentityName(*cert);
    auto entry       = std::make_shared<UserCache::UserCacheEntry>(uuid, name, xuid);
    mUuidEntry[uuid] = entry;
    mXuidEntry[xuid] = entry;
    mNameEntry[name] = entry;
    auto nbt         = CompoundTag{
                {"xuid", xuid},
                {"name", name}
    };
    auto key = "GMLIB_UserCache_" + uuid.asString();
    StorageAPI::getInstance()->setData(key, nbt);
}

void initUserCache() {
    StorageAPI::getInstance()->forEachKeyWithPrefix(
        "GMLIB_UserCache_",
        DBHelpers::Category::All,
        [](std::string_view key, std::string_view data) {
            auto uuid        = mce::UUID::fromString(std::string(key));
            auto info        = CompoundTag::fromBinaryNbt(data);
            auto xuid        = info->getString("xuid");
            auto name        = info->getString("name");
            auto entry       = std::make_shared<UserCache::UserCacheEntry>(uuid, name, xuid);
            mUuidEntry[uuid] = entry;
            mXuidEntry[xuid] = entry;
            mNameEntry[name] = entry;
        }
    );
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerLoginHook,
    ll::memory::HookPriority::Highest,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVLoginPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class LoginPacket const&       packet
) {
    origin(source, packet);
    updateUserCache(packet.mConnectionRequest->getCertificate());
}

} // namespace GMLIB