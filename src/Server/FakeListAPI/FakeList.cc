#include "Global.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::FakeListAPI {

extern std::unordered_map<std::string, PlayerListEntry> mFakeListMap;

inline void sendAddFakeListPacket(PlayerListEntry entry) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    pkt.emplace(std::move(entry));
    pkt.sendToClients();
}

inline void sendRemoveFakeListPacket(std::vector<PlayerListEntry> entries) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Remove;
    for (auto& entry : entries) {
        pkt.emplace(std::move(entry));
    }
    pkt.sendToClients();
}

GMLIB_API bool addFakeList(PlayerListEntry entry) {
    if (entry.mName.empty()) {
        return false;
    }
    mFakeListMap.insert({entry.mName, entry});
    sendAddFakeListPacket(entry);
    return true;
}

GMLIB_API bool addFakeList(std::string name, std::string xuid, ActorUniqueID uniqueId, mce::UUID uuid) {
    if (name.empty()) {
        return false;
    }
    auto entry   = PlayerListEntry(uuid);
    entry.mName  = name;
    entry.mXuid  = xuid;
    entry.mId.id = uniqueId.id;
    if (mFakeListMap.count(name)) {
        sendRemoveFakeListPacket({mFakeListMap[name]});
    }
    mFakeListMap.insert({name, entry});
    sendAddFakeListPacket(entry);
    return true;
}

GMLIB_API bool removeFakeList(std::string nameOrXuid) {
    bool isRemoved = false;
    for (auto fakeListPair : mFakeListMap) {
        if (fakeListPair.first == nameOrXuid || fakeListPair.second.mXuid == nameOrXuid) {
            mFakeListMap.erase(fakeListPair.first);
            sendRemoveFakeListPacket({fakeListPair.second});
            isRemoved = true;
        }
    }
    return isRemoved;
}

GMLIB_API void removeAllFakeLists() {
    std::vector<PlayerListEntry> entries;
    for (auto fakeListPair : mFakeListMap) {
        entries.push_back(fakeListPair.second);
    }
    sendRemoveFakeListPacket(entries);
    mFakeListMap.clear();
}

GMLIB_API PlayerListEntry getFakeList(std::string name) { return mFakeListMap[name]; }

GMLIB_API bool checkFakeListExistsName(std::string name) { return mFakeListMap.count(name); }

GMLIB_API bool checkFakeListExists(std::string name, std::string xuid) {
    for (auto fakeListPair : mFakeListMap) {
        if (fakeListPair.first == name && fakeListPair.second.mXuid == xuid) {
            return true;
        }
    }
    return false;
}

GMLIB_API std::vector<std::string> getAllFakeNames() {
    std::vector<std::string> allFakeLists;
    for (auto fakeListPair : mFakeListMap) {
        allFakeLists.push_back(fakeListPair.first);
    }
    return allFakeLists;
}

} // namespace GMLIB::FakeListAPI