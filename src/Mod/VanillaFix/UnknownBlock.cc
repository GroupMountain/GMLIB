#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/world/level/block/UnknownBlock.h>

namespace GMLIB::Mod {

std::unordered_set<std::string>                       mUnknownBlockLegacyNameList;
bool                                                  mAutoCleanUnknownBlockEnabled = false;
std::unordered_map<int, std::unordered_set<ChunkPos>> mFixedChunksList              = {};

std::unordered_set<std::string> VanillaFix::getUnknownBlockLegacyNameList() { return mUnknownBlockLegacyNameList; }

std::pair<int, int> getHeightInfo(int dimid) {
    switch (dimid) {
    case 0:
        return {-64, 384};
    case 1:
        return {0, 128};
    case 2:
        return {0, 256};
    }
    return {-64, 384};
}

void fixLevelChunk(ChunkPos cp, int dimid) {
    auto ht = getHeightInfo(dimid);
    for (int x = 0; x <= 15; x++) {
        for (int z = 0; z <= 15; z++) {
            for (int y = ht.first; y <= ht.second; y++) {
                BlockPos bp   = {16 * (cp.x) + x, y, 16 * (cp.z) + z};
                auto     type = GMLIB_Level::getInstance()->getBlock(bp, dimid)->getTypeName();
                if (mUnknownBlockLegacyNameList.count(type)) {
                    GMLIB_Level::getInstance()->setBlock(bp, dimid, "minecraft:air", 0);
                }
            }
        }
    }
}

bool isChunkFixed(ChunkPos cp, int dimid) { return mFixedChunksList[dimid].count(cp) ? true : false; }

void setChunkFixed(ChunkPos cp, int dimid) {
    fixLevelChunk(cp, dimid);
    mFixedChunksList[dimid].insert(cp);
}

LL_INSTANCE_HOOK(
    LoadUnknownBlock,
    HookPriority::Normal,
    "??0UnknownBlock@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HAEBVMaterial@@@Z",
    UnknownBlock*,
    std::string& a1,
    int          a2,
    void*        a3
) {
    mUnknownBlockLegacyNameList.insert(a1);
    return origin(a1, a2, a3);
}

LL_TYPE_INSTANCE_HOOK(
    ChunkLoadEvent,
    HookPriority::Normal,
    LevelChunk,
    "?_setOnChunkLoadedCalled@LevelChunk@@QEAA_NXZ",
    bool
) {
    if (mAutoCleanUnknownBlockEnabled) {
        auto cp    = this->getPosition();
        auto dimid = this->getDimension().getDimensionId();
        if (!isChunkFixed(cp, dimid)) {
            setChunkFixed(cp, dimid);
        }
    }
    return origin();
}

GMLIB_API void VanillaFix::setAutoCleanUnknownBlockEnabled() {
    if (!mAutoCleanUnknownBlockEnabled) {
        ll::memory::HookRegistrar<LoadUnknownBlock>().hook();
        ll::memory::HookRegistrar<ChunkLoadEvent>().hook();
        mAutoCleanUnknownBlockEnabled = true;
    }
}

} // namespace GMLIB::Mod