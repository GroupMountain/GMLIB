#include "Global.h"
namespace GMLIB::FakeListAPI{
    extern std::unordered_map<std::string, PlayerListEntry> fakeListMap;

inline void sendAddFakeListPacket(PlayerListEntry entry) {
    auto pkt = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Add;
    pkt.emplace(std::move(entry));
    pktSender->send(pkt);
    }
    inline void sendRemoveFakeListPacket(std::vector<PlayerListEntry> entries) {
    auto pkt = PlayerListPacket();
    pkt.mAction = PlayerListPacketType::Remove;
    for(auto &entry:entries){
    pkt.emplace(std::move(entry));
    }
    pktSender->send(pkt);
    }

bool addFakeList(PlayerListEntry entry){
    if(entry.mName.empty()){
        return false;
    }
    fakeListMap.insert({entry.mName,entry});
    sendAddFakeListPacket(entry);
    return true;
}
bool addFakeList(std::string name, std::string xuid, long long auid = -1, mce::UUID UUID = mce::UUID()){
    if(name.empty()){
        return false;
    }
    auto entry=PlayerListEntry(UUID);
    entry.mName=name;
    entry.mXuid=xuid;
    entry.mId.id=auid;
    if (fakeListMap.count(name)) {
        sendRemoveFakeListPacket({fakeListMap[name]});
    }
    fakeListMap.insert({name,entry});
    sendAddFakeListPacket(entry);
    return true;
}
bool removeFakeList(std::string nameOrXuid) {
    bool isRemoved=false;
    for (auto fakeListPair : fakeListMap) {
        if (fakeListPair.first == nameOrXuid || fakeListPair.second.mXuid == nameOrXuid) {
            fakeListMap.erase(fakeListPair.first);
            sendRemoveFakeListPacket({fakeListPair.second});
            isRemoved=true;
        }
    }
    return isRemoved;
}
void removeAllFakeLists() {
    std::vector<PlayerListEntry> entries;
    for (auto fakeListPair : fakeListMap) {
        entries.push_back(fakeListPair.second);
    }
    sendRemoveFakeListPacket(entries);
    fakeListMap.clear();
}
bool CheckFakeListExists(std::string name,std::string xuid){
    for (auto fakeListPair : fakeListMap) {
        if (fakeListPair.first == name && fakeListPair.second.mXuid == xuid) {
            return true;
        }
    }
    return false;
}
}