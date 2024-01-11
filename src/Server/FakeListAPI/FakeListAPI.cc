#include "Global.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/actor/player/PlayerListPacketType.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::FakeListAPI {

std::unordered_set<std::string>                  mInvisibleMap;
std::unordered_map<std::string, std::string>     mReplaceMap;
std::unordered_map<std::string, PlayerListEntry> mFakeListMap;
bool                                             mSimulatedPlayerOptList = false;

GMLIB_API void setSimulatedPlayerOpt(bool set) { mSimulatedPlayerOptList = set; }

GMLIB_API bool getSimulatedPlayerOpt() { return mSimulatedPlayerOptList; }

LL_AUTO_TYPED_INSTANCE_HOOK(
    sendAllFakeListPlayerJoin,
    HookPriority::Normal,
    ServerPlayer,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
) {
    auto pkt    = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    for (auto fakeListPair : mFakeListMap) {
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

LL_AUTO_TYPED_INSTANCE_HOOK(
    fakeListEmplace,
    HookPriority::Normal,
    PlayerListPacket,
    "?emplace@PlayerListPacket@@QEAAX$$QEAVPlayerListEntry@@@Z",
    void,
    PlayerListEntry& entry
) {
    if (this->mAction == PlayerListPacketType::Add) {
        if (mInvisibleMap.count(entry.mName)) {
            return;
        }
        if (mSimulatedPlayerOptList) {
            // auto pl=ll::service::getLevel()->getPlayer(entry.mId); 安全性检测?
            if (ll::service::getLevel()->getPlayer(entry.mId)->isSimulatedPlayer()) {
                return;
            }
        }
        if (mReplaceMap.count(entry.mName)) {
            entry.mName = mReplaceMap[entry.mName];
        }
    }
    return origin(entry);
}
} // namespace GMLIB::FakeListAPI