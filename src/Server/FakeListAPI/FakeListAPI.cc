#include "Global.h"
#include <GMLIB/Server/FakeListAPI.h>
LoopbackPacketSender* pktSender;
namespace GMLIB::FakeListAPI{
    std::unordered_set<std::string> invisibleMap;
    std::unordered_map<std::string, std::string> replaceMap;
    std::unordered_map<std::string, PlayerListEntry> fakeListMap;
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
    pktSender->sendToClient(this->getNetworkIdentifier(),pkt,this->getClientSubId());
    return origin();
}
}