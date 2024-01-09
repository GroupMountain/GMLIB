#include "Global.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/actor/player/PlayerListPacketType.h"
#include <GMLIB/Server/FakeListAPI.h>
LoopbackPacketSender* pktSender;
namespace GMLIB::FakeListAPI{
    std::unordered_set<std::string> invisibleMap;
    std::unordered_map<std::string, std::string> replaceMap;
    std::unordered_map<std::string, PlayerListEntry> fakeListMap;
    bool simulatedPlayerOptList=false;
    extern void sendAddFakeListPacket(PlayerListEntry entry);
    extern void sendRemoveFakeListPacket(std::vector<PlayerListEntry> entries);


    LL_AUTO_TYPED_INSTANCE_HOOK(
    sendAllFakeListPlayerJoin,
    HookPriority::Normal,
    ServerPlayer,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
    ) {
    auto pkt = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    for(auto fakeListPair:fakeListMap){
    pkt.emplace(std::move(fakeListPair.second));
    }
    BinaryStream bs;//DefaultPermission
    bs.writeUnsignedInt64(-1,0,0);
    bs.writeUnsignedChar((unsigned char)1,0,0);
    bs.writeUnsignedChar((unsigned char)CommandPermissionLevel::Any,0,0);
    bs.writeUnsignedVarInt(0,0,0);
    auto ablitiespkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateAbilitiesPacket);
    ablitiespkt->read(bs);
    pktSender->sendToClient(this->getNetworkIdentifier(),pkt,this->getClientSubId());
    pktSender->sendToClient(this->getNetworkIdentifier(),*ablitiespkt,this->getClientSubId());
    return origin();
}
LL_AUTO_TYPED_INSTANCE_HOOK(
    fakeListEmplace,
    HookPriority::Normal,
    PlayerListPacket,
    "?emplace@PlayerListPacket@@QEAAX$$QEAVPlayerListEntry@@@Z",
    void,
    PlayerListEntry&entry)
{
    if(this->mAction==PlayerListPacketType::Add){
        if (invisibleMap.count(entry.mName)) {
            return;
        }
        if (simulatedPlayerOptList) {
            //auto pl=ll::service::getLevel()->getPlayer(entry.mId); 安全性检测?
            if (ll::service::getLevel()->getPlayer(entry.mId)->isSimulatedPlayer()) {
                return;
            }
        }
        if(replaceMap.count(entry.mName)){
            entry.mName=replaceMap[entry.mName];
        }
    }
    return origin(entry);
}
}