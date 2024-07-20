#include "GMLIB/Event/Level/SculkVeinCanSpreadEvent.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EmitterBase.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/ChunkPos.h"
#include "mc/world/level/IBlockWorldGenAPI.h"
#include "mc/world/level/block/utils/SculkVeinMultifaceSpreader.h"


namespace GMLIB::Event::LevelEvent {


Block const&             SculkVeinCanSpreadEvent::getBlock() const { return mBlock; }
BlockPos const&          SculkVeinCanSpreadEvent::getPos() const { return mPos; }
uchar                    SculkVeinCanSpreadEvent::getFace() const { return mFace; }
optional_ref<LevelChunk> SculkVeinCanSpreadEvent::getLevelChunk() const { return mLevelChunk; }

LL_TYPE_INSTANCE_HOOK(
    SculkVeinCanSpreadEventHook,
    ll::memory::HookPriority::Normal,
    SculkVeinMultifaceSpreader,
    "?_canSpreadInto@SculkVeinMultifaceSpreader@@UEBA_NAEAVIBlockWorldGenAPI@@AEBVBlock@@AEBVBlockPos@@E@Z",
    bool,
    IBlockWorldGenAPI& a1,
    Block const&       a2,
    BlockPos const&    a3,
    uchar              a4
) {
    auto levelChunkPtr = a1.getChunk(ChunkPos(a3));
    auto ev            = SculkVeinCanSpreadEvent(a2, a3, a4, levelChunkPtr);
    ll::event::EventBus::getInstance().publish(ev);
    if (ev.isCancelled()) {
        return false;
    }

    return origin(a1, a2, a3, a4);
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class SculkVeinCanSpreadEventEmitter : public ll::event::Emitter<emitterFactory, SculkVeinCanSpreadEvent> {
    ll::memory::HookRegistrar<SculkVeinCanSpreadEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<SculkVeinCanSpreadEventEmitter>();
}


} // namespace GMLIB::Event::LevelEvent