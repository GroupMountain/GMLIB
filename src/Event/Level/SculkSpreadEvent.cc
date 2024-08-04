#include "GMLIB/Event/Level/SculkSpreadEvent.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EmitterBase.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/IBlockWorldGenAPI.h"
#include "mc/world/level/block/utils/SculkVeinBlockBehavior.h"


namespace GMLIB::Event::LevelEvent {


BlockPos const&           SculkSpreadBeforeEvent::getPos() const { return mPos; }
optional_ref<BlockSource> SculkSpreadBeforeEvent::getBlockSource() const { return mBlockSource; }

BlockPos const&           SculkSpreadAfterEvent::getPos() const { return mPos; }
optional_ref<BlockSource> SculkSpreadAfterEvent::getBlockSource() const { return mBlockSource; }


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
    auto ev = SculkSpreadBeforeEvent(pos, region);
    ll::event::EventBus::getInstance().publish(ev);
    if (ev.isCancelled()) {
        return false;
    }

    return origin(target, region, pos, idk, random);

    auto after = SculkSpreadAfterEvent(pos, region);
    ll::event::EventBus::getInstance().publish(after);
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class SculkSpreadEventBeforeEmitter : public ll::event::Emitter<emitterFactory1, SculkSpreadBeforeEvent> {
    ll::memory::HookRegistrar<SculkSpreadEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<SculkSpreadEventBeforeEmitter>();
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class SculkSpreadEventAfterEmitter : public ll::event::Emitter<emitterFactory2, SculkSpreadAfterEvent> {
    ll::memory::HookRegistrar<SculkSpreadEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<SculkSpreadEventAfterEmitter>();
}


} // namespace GMLIB::Event::LevelEvent