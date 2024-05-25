#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/world/level/block/UnknownBlock.h>

namespace GMLIB::Mod {

std::unordered_set<std::string>                           mUnknownBlockLegacyNameList;
phmap::flat_hash_map<int, phmap::flat_hash_set<ChunkPos>> mFixedChunksList = {};

std::unordered_set<std::string> VanillaFix::getUnknownBlockLegacyNameList() { return mUnknownBlockLegacyNameList; }

std::pair<int, int> getHeightInfo(int dimId) {
    switch (dimId) {
    case 0:
        return {-64, 384};
    case 1:
        return {0, 128};
    case 2:
        return {0, 256};
    }
    return {-64, 384};
}

void fixLevelChunk(ChunkPos cp, int dimId) {
    auto ht = getHeightInfo(dimId);
    for (int x = 0; x <= 15; x++) {
        for (int z = 0; z <= 15; z++) {
            for (int y = ht.first; y <= ht.second; y++) {
                BlockPos bp   = {16 * (cp.x) + x, y, 16 * (cp.z) + z};
                auto     type = GMLIB_Level::getInstance()->getBlock(bp, dimId).getTypeName();
                if (mUnknownBlockLegacyNameList.count(type)) {
                    GMLIB_Level::getInstance()->setBlock(bp, dimId, "minecraft:air", 0);
                }
            }
        }
    }
}

bool isChunkFixed(ChunkPos cp, int dimId) { return mFixedChunksList[dimId].count(cp) ? true : false; }

void setChunkFixed(ChunkPos cp, int dimId) {
    fixLevelChunk(cp, dimId);
    mFixedChunksList[dimId].insert(cp);
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
    auto cp    = this->getPosition();
    auto dimId = this->getDimension().getDimensionId();
    if (!isChunkFixed(cp, dimId)) {
        setChunkFixed(cp, dimId);
    }
    return origin();
}

struct UnknownBlock_Impl {
    ll::memory::HookRegistrar<LoadUnknownBlock, ChunkLoadEvent> r;
};

std::unique_ptr<UnknownBlock_Impl> impl;

GMLIB_API void VanillaFix::setAutoCleanUnknownBlockEnabled() {
    if (!impl) {
        impl = std::make_unique<UnknownBlock_Impl>();
    }
}

} // namespace GMLIB::Mod