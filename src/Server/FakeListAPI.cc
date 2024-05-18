#include "Global.h"
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FakeListAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/network/MinecraftPackets.h>
#include <mc/network/packet/PlayerListEntry.h>
#include <mc/network/packet/PlayerListPacket.h>
#include <unordered_map>

namespace GMLIB::Server {

namespace FakeListAPI {

std::unordered_map<std::string, std::string>     mReplaceMap;
std::unordered_map<std::string, PlayerListEntry> mFakeListMap;
bool                                               mSimulatedPlayerOptList = false;

} // namespace FakeListAPI

LL_TYPE_INSTANCE_HOOK(
    SendAllFakeListPlayerJoinHook,
    HookPriority::Normal,
    ServerPlayer,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    for (auto& fakeListPair : GMLIB::Server::FakeListAPI::mFakeListMap) {
        pkt.emplace(std::move(fakeListPair.second));
    }
    GMLIB_BinaryStream bs; // DefaultPermission
    bs.writePacketHeader(MinecraftPacketIds::UpdateAbilities);
    bs.writeUnsignedInt64(-1);
    bs.writeUnsignedChar((uchar)1);
    bs.writeUnsignedChar((uchar)CommandPermissionLevel::Any);
    bs.writeUnsignedVarInt(0);
    pkt.sendTo(*this);
    bs.sendTo(*this);
    return origin();
}

LL_TYPE_INSTANCE_HOOK(
    FakeListEmplaceHook,
    HookPriority::Normal,
    PlayerListPacket,
    "?emplace@PlayerListPacket@@QEAAX$$QEAVPlayerListEntry@@@Z",
    void,
    PlayerListEntry& entry
) {
    if (this->mAction == PlayerListPacketType::Add) {
        if (GMLIB::Server::FakeListAPI::mSimulatedPlayerOptList) {
            if (ll::service::getLevel()->getPlayer(entry.mId)->isSimulatedPlayer()) {
                return;
            }
        }
        if (GMLIB::Server::FakeListAPI::mReplaceMap.count(entry.mName)) {
            entry.mName = GMLIB::Server::FakeListAPI::mReplaceMap[entry.mName];
        }
    }
    return origin(entry);
}

struct Fakelist_Impl_1 {
    ll::memory::HookRegistrar<SendAllFakeListPlayerJoinHook> r;
};

std::unique_ptr<Fakelist_Impl_1> fakelist_Impl_1;

void enableHook1() {
    if (!fakelist_Impl_1) {
        fakelist_Impl_1 = std::make_unique<Fakelist_Impl_1>();
    }
}

struct Fakelist_Impl_2 {
    ll::memory::HookRegistrar<FakeListEmplaceHook> r;
};

std::unique_ptr<Fakelist_Impl_2> fakelist_Impl_2;

void changeHook2(bool enable) {
    if (enable) {
        if (!fakelist_Impl_2) fakelist_Impl_2 = std::make_unique<Fakelist_Impl_2>();
    } else {
        fakelist_Impl_2.reset();
    }
}

inline void sendAddFakeListPacket(PlayerListEntry entry) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    pkt.emplace(std::move(entry));
    GMLIB_Level::getInstance()->sendPacketToClients(pkt);
}

inline void sendRemoveFakeListPacket(std::vector<PlayerListEntry> entries) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Remove;
    for (auto& entry : entries) {
        pkt.emplace(std::move(entry));
    }
    GMLIB_Level::getInstance()->sendPacketToClients(pkt);
}

bool FakeList::addFakeList(PlayerListEntry const& entry) {
    if (entry.mName.empty()) {
        return false;
    }
    GMLIB::Server::enableHook1();
    GMLIB::Server::FakeListAPI::mFakeListMap.insert({entry.mName, entry});
    sendAddFakeListPacket(entry);
    return true;
}

bool FakeList::addFakeList(
    std::string const&   name,
    std::string const&   xuid,
    ActorUniqueID const& uniqueId,
    mce::UUID const&     uuid
) {
    if (name.empty()) {
        return false;
    }
    auto entry   = PlayerListEntry(uuid);
    entry.mName  = name;
    entry.mXuid  = xuid;
    entry.mId.id = uniqueId.id;
    GMLIB::Server::enableHook1();
    if (GMLIB::Server::FakeListAPI::mFakeListMap.count(name)) {
        sendRemoveFakeListPacket({GMLIB::Server::FakeListAPI::mFakeListMap[name]});
    }
    GMLIB::Server::FakeListAPI::mFakeListMap.insert({name, entry});
    sendAddFakeListPacket(entry);
    return true;
}

bool FakeList::removeFakeList(std::string const& nameOrXuid) {
    bool isRemoved = false;
    GMLIB::Server::enableHook1();
    for (auto& fakeListPair : GMLIB::Server::FakeListAPI::mFakeListMap) {
        if (fakeListPair.first == nameOrXuid || fakeListPair.second.mXuid == nameOrXuid) {
            GMLIB::Server::FakeListAPI::mFakeListMap.erase(fakeListPair.first);
            sendRemoveFakeListPacket({fakeListPair.second});
            isRemoved = true;
        }
    }
    return isRemoved;
}

void FakeList::removeAllFakeLists() {
    std::vector<PlayerListEntry> entries;
    GMLIB::Server::enableHook1();
    for (auto& fakeListPair : GMLIB::Server::FakeListAPI::mFakeListMap) {
        entries.push_back(fakeListPair.second);
    }
    sendRemoveFakeListPacket(entries);
    GMLIB::Server::FakeListAPI::mFakeListMap.clear();
}

PlayerListEntry FakeList::getFakeList(std::string const& name) {
    return GMLIB::Server::FakeListAPI::mFakeListMap[name];
}

bool FakeList::checkFakeListExistsName(std::string const& name) {
    return GMLIB::Server::FakeListAPI::mFakeListMap.count(name);
}

bool FakeList::checkFakeListExists(std::string const& name, std::string const& xuid) {
    for (auto& fakeListPair : GMLIB::Server::FakeListAPI::mFakeListMap) {
        if (fakeListPair.first == name && fakeListPair.second.mXuid == xuid) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> FakeList::getAllFakeNames() {
    std::vector<std::string> allFakeLists;
    for (auto& fakeListPair : GMLIB::Server::FakeListAPI::mFakeListMap) {
        allFakeLists.push_back(fakeListPair.first);
    }
    return allFakeLists;
}

inline void updatePlayerList(std::string const& realName) {
    ll::service::getLevel()->forEachPlayer([realName](Player& pl) -> bool {
        if (pl.getRealName() == realName) {
            if (pl.isSimulatedPlayer() && !GMLIB::Server::FakeListAPI::mSimulatedPlayerOptList) {
                return true;
            }
            auto entry = PlayerListEntry(pl);
            sendAddFakeListPacket(entry);
        }
        return true;
    });
    if (GMLIB::Server::FakeListAPI::mFakeListMap.count(realName)) {
        sendAddFakeListPacket(GMLIB::Server::FakeListAPI::mFakeListMap[realName]);
    }
}

void FakeList::setListName(std::string const& realName, std::string const& fakeName) {
    GMLIB::Server::FakeListAPI::mReplaceMap[realName] = fakeName;
    updatePlayerList(realName);
}

void FakeList::resetListName(std::string const& realName) {
    GMLIB::Server::FakeListAPI::mReplaceMap.erase(realName);
    updatePlayerList(realName);
}

void FakeList::setSimulatedPlayerListOptimizeEnabled(bool value) {
    GMLIB::Server::FakeListAPI::mSimulatedPlayerOptList = value;
    changeHook2(value);
}

bool FakeList::getSimulatedPlayerListOptimizeEnabled() { return GMLIB::Server::FakeListAPI::mSimulatedPlayerOptList; }

} // namespace GMLIB::Server