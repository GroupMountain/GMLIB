#include "GMLIB/Event/Level/SculkSpreadEvent.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EmitterBase.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/IBlockWorldGenAPI.h"
#include "mc/world/level/block/utils/SculkVeinBlockBehavior.h"


namespace GMLIB::Event::LevelEvent {


BlockPos const&           SculkSpreadEvent::getPos() const { return mPos; }
optional_ref<BlockSource> SculkSpreadEvent::getBlockSource() const { return mBlockSource; }


LL_STATIC_HOOK(
    SculkSpreadEventHook,
    ll::memory::HookPriority::Normal,
    "?_attemptPlaceSculk@SculkVeinBlockBehavior@@CA_NAEAVIBlockWorldGenAPI@@PEAVBlockSource@@AEBVBlockPos@@"
    "AEAVSculkSpreader@@AEAVRandom@@@Z",
    bool,
    IBlockWorldGenAPI& target,
    BlockSource*       region,
    BlockPos const&    pos,
    SculkSpreader&     idk,
    Random&            random
) {
    auto ev = SculkSpreadEvent(pos, region);
    ll::event::EventBus::getInstance().publish(ev);
    if (ev.isCancelled()) {
        return false;
    }

    return origin(target, region, pos, idk, random);
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class SculkSpreadEventEmitter : public ll::event::Emitter<emitterFactory, SculkSpreadEvent> {
    ll::memory::HookRegistrar<SculkSpreadEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<SculkSpreadEventEmitter>();
}


} // namespace GMLIB::Event::LevelEvent