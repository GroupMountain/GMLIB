#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/PlayerAPI.h>

namespace GMLIB::PlayerAPI {

GMLIB_API void forEachUuid(bool includeOfflineSignedId, std::function<void(std::string_view const& uuid)> callback) {
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "player_",
        DBHelpers::Category::Player,
        [&callback, includeOfflineSignedId](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto  tag   = CompoundTag::fromBinaryNbt(data);
                auto& msaId = tag->getString("MsaId");
                if (!msaId.empty()) {
                    if (msaId == key_left) {
                        callback(msaId);
                    }
                    return;
                }
                if (!includeOfflineSignedId) {
                    return;
                }
                auto& selfSignedId = tag->getString("SelfSignedId");
                if (!selfSignedId.empty()) {
                    if (selfSignedId == key_left) {
                        callback(selfSignedId);
                    }
                    return;
                }
            }
        }
    );
}

GMLIB_API std::vector<std::string> getAllUuids(bool includeOfflineSignedId) {
    std::vector<std::string> uuids;
    forEachUuid(includeOfflineSignedId, [&uuids](std::string_view uuid) { uuids.push_back(std::string(uuid)); });
    return uuids;
}

GMLIB_API std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid) {
    auto& dbStorage = *GMLIB::Global<DBStorage>;
    auto  playerKey = "player_" + uuid.asString();
    if (dbStorage.hasKey(playerKey, DBHelpers::Category::Player)) {
        return dbStorage.getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

GMLIB_API std::string getServeridFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

GMLIB_API std::unique_ptr<CompoundTag> getOfflineNbt(std::string& serverid) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverid, DBHelpers::Category::Player);
}

GMLIB_API bool setOfflineNbt(std::string& serverid, CompoundTag* nbt) {
    try {
        auto& data = *nbt;
        if (serverid.empty()) {
            return false;
        }
        GMLIB::Global<DBStorage>->saveData(serverid, data.toBinaryNbt(), DBHelpers::Category::Player);
        return true;
    } catch (...) {
        return false;
    }
}

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto nbt = player->saveToNbt();
        return nbt;
    } else {
        auto serverid = getServeridFromUuid(uuid);
        return getOfflineNbt(serverid);
    }
}

GMLIB_API std::unique_ptr<CompoundTag> getPlayerNbt(Player* pl) { return pl->saveToNbt(); }

GMLIB_API bool setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        return GMLIB::CompoundTagHelper::setNbt(player, nbt);
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    return setOfflineNbt(serverid, nbt);
}

GMLIB_API bool setPlayerNbt(Player* pl, CompoundTag* nbt) { return GMLIB::CompoundTagHelper::setNbt(pl, nbt); }

GMLIB_API void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags) {
    for (auto tag : tags) {
        if (dataNbt->get(tag)) {
            originNbt->put(tag, dataNbt->get(tag)->copy());
        }
    }
}

GMLIB_API bool setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto data = player->saveToNbt();
        setNbtTags(data.get(), nbt, tags);
        return GMLIB::CompoundTagHelper::setNbt(player, data.get());
    }
    auto serverid = getServeridFromUuid(uuid);
    if (serverid.empty()) {
        return false;
    }
    auto data = getOfflineNbt(serverid);
    setNbtTags(data.get(), nbt, tags);
    return setOfflineNbt(serverid, data.get());
}

GMLIB_API bool deletePlayerNbt(std::string& serverid) {
    if (serverid.empty()) {
        return false;
    }
    if (GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        GMLIB::Global<DBStorage>->deleteData(serverid, DBHelpers::Category::Player);
        return true;
    }
    return false;
}

} // namespace GMLIB::PlayerAPI