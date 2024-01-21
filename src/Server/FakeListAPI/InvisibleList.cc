#include "Server/FakeListAPI/FakeListAPI.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::Server {

void FakeList::setSimulatedPlayerListOptimizeEnabled(bool value) {
    GMLIB::FakeListAPI::mSimulatedPlayerOptList = value;
}

bool FakeList::getSimulatedPlayerListOptimizeEnabled() { return GMLIB::FakeListAPI::mSimulatedPlayerOptList; }


LL_AUTO_TYPE_INSTANCE_HOOK(
    sendAllFakeListPlayerJoin,
    HookPriority::Normal,
    ServerPlayer,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    for (auto fakeListPair : GMLIB::FakeListAPI::mFakeListMap) {
        pkt.emplace(std::move(fakeListPair.second));
    }
    BinaryStream bs; // DefaultPermission
    bs.writeUnsignedInt64(-1, 0, 0);
    bs.writeUnsignedChar((uchar)1, 0, 0);
    bs.writeUnsignedChar((uchar)CommandPermissionLevel::Any, 0, 0);
    bs.writeUnsignedVarInt(0, 0, 0);
    auto ablitiespkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateAbilitiesPacket);
    ablitiespkt->read(bs);
    pkt.sendToClients();
    ablitiespkt->sendToClients();
    return origin();
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    fakeListEmplace,
    HookPriority::Normal,
    PlayerListPacket,
    "?emplace@PlayerListPacket@@QEAAX$$QEAVPlayerListEntry@@@Z",
    void,
    PlayerListEntry& entry
) {
    if (this->mAction == PlayerListPacketType::Add) {
        if (GMLIB::FakeListAPI::mInvisibleMap.count(entry.mName)) {
            return;
        }
        if (GMLIB::FakeListAPI::mSimulatedPlayerOptList) {
            // auto pl=ll::service::getLevel()->getPlayer(entry.mId); 安全性检测?
            if (ll::service::getLevel()->getPlayer(entry.mId)->isSimulatedPlayer()) {
                return;
            }
        }
        if (GMLIB::FakeListAPI::mReplaceMap.count(entry.mName)) {
            entry.mName = GMLIB::FakeListAPI::mReplaceMap[entry.mName];
        }
    }
    return origin(entry);
}

}