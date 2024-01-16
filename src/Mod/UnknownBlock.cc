#include "Global.h"
#include <GMLIB/Mod/UnknownBlock.h>
#include <GMLIB/Server/LevelAPI.h>

std::unordered_set<std::string>                       mUnknownBlockLegacyNameList;
bool                                                  mAutoCleanUnknownBlockEnabled = false;
std::unordered_map<int, std::unordered_set<ChunkPos>> mFixedChunksList              = {};

std::unordered_set<std::string> GMLIB_UnknownBlock::getUnknownBlockLegacyNameList() {
    return mUnknownBlockLegacyNameList;
}

GMLIB_API void GMLIB_UnknownBlock::setAutoCleanUnknownBlockEnabled(bool value = true) {
    mAutoCleanUnknownBlockEnabled = value;
}

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
                auto     type = GMLIB_Level::getLevel()->getBlock(bp, dimid)->getTypeName();
                if (mUnknownBlockLegacyNameList.count(type)) {
                    GMLIB_Level::getLevel()->setBlock("minecraft:air", 0, bp, dimid);
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

LL_AUTO_INSTANCE_HOOK(
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

LL_AUTO_TYPE_INSTANCE_HOOK(
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