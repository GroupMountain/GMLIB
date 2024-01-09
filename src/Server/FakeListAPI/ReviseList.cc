#include "Global.h"
#include <GMLIB/Server/FakeListAPI.h>
namespace GMLIB::FakeListAPI {
extern std::unordered_map<std::string, std::string>     replaceMap;
extern std::unordered_map<std::string, PlayerListEntry> fakeListMap;
extern bool                                             simulatedPlayerOptList;
extern void                                             sendAddFakeListPacket(PlayerListEntry entry);
void replaceList(bool add, std::string oldName, std::string newName) {
    if (add) {
        replaceMap[oldName] = newName;
    } else {
        replaceMap.erase(oldName);
    }
    ll::service::getLevel()->forEachPlayer([add, oldName, newName](Player& pl) -> bool {
        if (pl.getRealName() == oldName) {
            if (pl.isSimulatedPlayer() && !simulatedPlayerOptList) {
                return true;
            }
            auto entry = PlayerListEntry(pl);
            sendAddFakeListPacket(entry);
        }
        return true;
    });
    if (fakeListMap.count(oldName)) {
        sendAddFakeListPacket(fakeListMap[oldName]);
    }
}
} // namespace GMLIB::FakeListAPI