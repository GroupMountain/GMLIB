#include "Global.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::FakeListAPI {

extern std::unordered_map<std::string, std::string>     mReplaceMap;
extern std::unordered_map<std::string, PlayerListEntry> mFakeListMap;
extern bool                                             mSimulatedPlayerOptList;

extern void sendAddFakeListPacket(PlayerListEntry entry);

GMLIB_API void replaceList(bool add, std::string oldName, std::string newName) {
    if (add) {
        mReplaceMap[oldName] = newName;
    } else {
        mReplaceMap.erase(oldName);
    }
    ll::service::getLevel()->forEachPlayer([add, oldName, newName](Player& pl) -> bool {
        if (pl.getRealName() == oldName) {
            if (pl.isSimulatedPlayer() && !mSimulatedPlayerOptList) {
                return true;
            }
            auto entry = PlayerListEntry(pl);
            sendAddFakeListPacket(entry);
        }
        return true;
    });
    if (mFakeListMap.count(oldName)) {
        sendAddFakeListPacket(mFakeListMap[oldName]);
    }
}

} // namespace GMLIB::FakeListAPI