#include "GMLIB/Event/Level/SculkBlockGrowthEvent.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EmitterBase.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/IBlockWorldGenAPI.h"
#include "mc/world/level/block/utils/SculkBlockBehavior.h"


namespace GMLIB::Event::LevelEvent {


BlockSource*    SculkBlockGrowthBeforeEvent::getSource() const { return mSource; }
BlockPos const& SculkBlockGrowthBeforeEvent::getPos() const { return mPos; }

BlockSource*    SculkBlockGrowthAfterEvent::getSource() const { return mSource; }
BlockPos const& SculkBlockGrowthAfterEvent::getPos() const { return mPos; }


LL_STATIC_HOOK(
    SculkBlockGrowthHook,
    ll::memory::HookPriority::Normal,
    "?_placeGrowthAt@SculkBlockBehavior@@CAXAEAVIBlockWorldGenAPI@@PEAVBlockSource@@AEBVBlockPos@@AEAVRandom@@"
    "AEAVSculkSpreader@@@Z",
    void,
    IBlockWorldGenAPI& target,
    BlockSource*       region,
    BlockPos const&    pos,
    Random&            random,
    SculkSpreader&     a5
) {
    auto ev = SculkBlockGrowthBeforeEvent(region, pos);
    ll::event::EventBus::getInstance().publish(ev);
    if (ev.isCancelled()) {
        return;
    }

    origin(target, region, pos, random, a5);

    auto after = SculkBlockGrowthAfterEvent(region, pos);
    ll::event::EventBus::getInstance().publish(after);
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class SculkBlockGrowthBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, SculkBlockGrowthBeforeEvent> {
    ll::memory::HookRegistrar<SculkBlockGrowthHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<SculkBlockGrowthBeforeEventEmitter>();
}


static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class SculkBlockGrowthAfterEventEmitter : public ll::event::Emitter<emitterFactory2, SculkBlockGrowthAfterEvent> {
    ll::memory::HookRegistrar<SculkBlockGrowthHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<SculkBlockGrowthAfterEventEmitter>();
}

} // namespace GMLIB::Event::LevelEvent