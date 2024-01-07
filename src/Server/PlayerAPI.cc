#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>
#include <GMLIB/Server/PlayerAPI.h>

namespace GMLIB::PlayerAPI {

void forEachUuid(bool includeSelfSignedId, std::function<void(std::string_view const& uuid)> callback) {
    static size_t count;
    count = 0;
    GMLIB::Global<DBStorage>->forEachKeyWithPrefix(
        "player_",
        DBHelpers::Category::Player,
        [&callback, includeSelfSignedId](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto  tag   = CompoundTag::fromBinaryNbt(data);
                auto& msaId = tag->getString("MsaId");
                if (!msaId.empty()) {
                    if (msaId == key_left) {
                        count++;
                        callback(msaId);
                    }
                    return;
                }
                if (!includeSelfSignedId) {
                    return;
                }
                auto& selfSignedId = tag->getString("SelfSignedId");
                if (!selfSignedId.empty()) {
                    if (selfSignedId == key_left) {
                        count++;
                        callback(selfSignedId);
                    }
                    return;
                }
            }
        }
    );
}

std::vector<std::string> getAllUuids(bool includeSelfSignedId) {
    std::vector<std::string> uuids;
    forEachUuid(includeSelfSignedId, [&uuids](std::string_view uuid) { uuids.push_back(std::string(uuid)); });
    return uuids;
}

std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid) {
    auto& dbStorage = *GMLIB::Global<DBStorage>;
    auto  playerKey = "player_" + uuid.asString();
    if (dbStorage.hasKey(playerKey, DBHelpers::Category::Player)) {
        return dbStorage.getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

std::string getServeridFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        return "";
    }
    return DBTag->getString("ServerId");
}

std::unique_ptr<CompoundTag> getOfflineNbt(std::string serverid) {
    if (!GMLIB::Global<DBStorage>->hasKey(serverid, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return GMLIB::Global<DBStorage>->getCompoundTag(serverid, DBHelpers::Category::Player);
}

bool setOfflineNbt(std::string serverid, CompoundTag* nbt) {
    try {
        auto& data = *nbt;
        if (serverid.empty()) {
            return false;
        }
        GMLIB::Global<DBStorage>->saveData(serverid, data.toBinaryNbt(), DBHelpers::Category::Player);
        return true;
    } catch (...) {
        logger.error("Fail to set offline player nbt! Serverid: {}", serverid);
        return false;
    }
}

std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID uuid) {
    auto player = ll::service::bedrock::getLevel()->getPlayer(uuid);
    if (player) {
        auto nbt = player->saveToNbt();
        return nbt;
    } else {
        auto serverid = getServeridFromUuid(uuid);
        return getOfflineNbt(serverid);
    }
}

std::unique_ptr<CompoundTag> getPlayerNbt(Player* pl) { return pl->saveToNbt(); }

bool setPlayerNbt(mce::UUID const& uuid, CompoundTag* nbt) {
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

void setNbtTags(CompoundTag* originNbt, CompoundTag* dataNbt, const std::vector<std::string>& tags) {
    for (auto tag : tags) {
        if (dataNbt->get(tag)) {
            originNbt->put(tag, dataNbt->get(tag)->copy());
        }
    }
}

bool setPlayerNbtTags(mce::UUID const& uuid, CompoundTag* nbt, const std::vector<std::string>& tags) {
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

bool deletePlayerNbt(std::string serverid) {
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

LL_AUTO_TYPED_INSTANCE_HOOK(
    DBStorageInitEvent,
    ll::memory::HookPriority::Normal,
    DBStorage,
    "??0DBStorage@@QEAA@UDBStorageConfig@@V?$not_null@V?$NonOwnerPointer@VLevelDbEnv@@@Bedrock@@@gsl@@@Z",
    DBStorage*,
    struct DBStorageConfig&                        a1,
    Bedrock::NotNullNonOwnerPtr<class LevelDbEnv>& a2
) {
    auto res                 = origin(a1, a2);
    GMLIB::Global<DBStorage> = this;
    logger.warn("init");
    return res;
}