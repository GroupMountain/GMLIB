#include "Global.h"

LoopbackPacketSender* pktSender;
namespace GMLIB::FakeListAPI{
    std::unordered_set<std::string> invisibleMap;
    std::unordered_map<std::string, std::string> replaceMap;
    std::unordered_map<std::string, PlayerListEntry> fakeListMap;
    extern void sendAddFakeListPacket(PlayerListEntry entry);
    extern void sendRemoveFakeListPacket(std::vector<PlayerListEntry> entries);
}