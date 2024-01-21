#include "Server/FakeListAPI/FakeListAPI.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::Server {

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

bool FakeList::addFakeList(PlayerListEntry entry) {
    if (entry.mName.empty()) {
        return false;
    }
    GMLIB::FakeListAPI::mFakeListMap.insert({entry.mName, entry});
    sendAddFakeListPacket(entry);
    return true;
}

bool FakeList::addFakeList(std::string name, std::string xuid, ActorUniqueID uniqueId, mce::UUID uuid) {
    if (name.empty()) {
        return false;
    }
    auto entry   = PlayerListEntry(uuid);
    entry.mName  = name;
    entry.mXuid  = xuid;
    entry.mId.id = uniqueId.id;
    if (GMLIB::FakeListAPI::mFakeListMap.count(name)) {
        sendRemoveFakeListPacket({GMLIB::FakeListAPI::mFakeListMap[name]});
    }
    GMLIB::FakeListAPI::mFakeListMap.insert({name, entry});
    sendAddFakeListPacket(entry);
    return true;
}

bool FakeList::removeFakeList(std::string nameOrXuid) {
    bool isRemoved = false;
    for (auto fakeListPair : GMLIB::FakeListAPI::mFakeListMap) {
        if (fakeListPair.first == nameOrXuid || fakeListPair.second.mXuid == nameOrXuid) {
            GMLIB::FakeListAPI::mFakeListMap.erase(fakeListPair.first);
            sendRemoveFakeListPacket({fakeListPair.second});
            isRemoved = true;
        }
    }
    return isRemoved;
}

void FakeList::removeAllFakeLists() {
    std::vector<PlayerListEntry> entries;
    for (auto fakeListPair : GMLIB::FakeListAPI::mFakeListMap) {
        entries.push_back(fakeListPair.second);
    }
    sendRemoveFakeListPacket(entries);
    GMLIB::FakeListAPI::mFakeListMap.clear();
}

PlayerListEntry FakeList::getFakeList(std::string name) { return GMLIB::FakeListAPI::mFakeListMap[name]; }

bool FakeList::checkFakeListExistsName(std::string name) { return GMLIB::FakeListAPI::mFakeListMap.count(name); }

bool FakeList::checkFakeListExists(std::string name, std::string xuid) {
    for (auto fakeListPair : GMLIB::FakeListAPI::mFakeListMap) {
        if (fakeListPair.first == name && fakeListPair.second.mXuid == xuid) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> FakeList::getAllFakeNames() {
    std::vector<std::string> allFakeLists;
    for (auto fakeListPair : GMLIB::FakeListAPI::mFakeListMap) {
        allFakeLists.push_back(fakeListPair.first);
    }
    return allFakeLists;
}

inline void updatePlayerList(std::string realName) {

    ll::service::getLevel()->forEachPlayer([realName](Player& pl) -> bool {
        if (pl.getRealName() == realName) {
            if (pl.isSimulatedPlayer() && !GMLIB::FakeListAPI::mSimulatedPlayerOptList) {
                return true;
            }
            auto entry = PlayerListEntry(pl);
            sendAddFakeListPacket(entry);
        }
        return true;
    });
    if (GMLIB::FakeListAPI::mFakeListMap.count(realName)) {
        sendAddFakeListPacket(GMLIB::FakeListAPI::mFakeListMap[realName]);
    }
}

void FakeList::setListName(std::string realName, std::string fakeName) {
    GMLIB::FakeListAPI::mReplaceMap[realName] = fakeName;
    updatePlayerList(realName);
}

void FakeList::resetListName(std::string realName) {
    GMLIB::FakeListAPI::mReplaceMap.erase(realName);
    updatePlayerList(realName);
}

}