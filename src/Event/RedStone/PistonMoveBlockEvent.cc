#include "GMLIB/Event/RedStone/PistonMoveBlockEvent.h"
#include "Global.h"
#include "mc/world/level/block/actor/PistonBlockActor.h"
#include <string>


namespace GMLIB::Event::RedStoneEvent {
static bool         isRetract = false;

GMLIB_API Block&    PistonMoveBlockEvent::getBlock() const { return mBlock; }
GMLIB_API BlockPos& PistonMoveBlockEvent::getPosition() const { return mMovedBlockPos; }

LL_AUTO_TYPE_INSTANCE_HOOK(
    BlockMovedHook,
    ll::memory::HookPriority::Normal,
    Block,
    "?movedByPiston@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@@Z",
    void,
    BlockSource& a2,
    BlockPos&    a3
) {
    if (isRetract) {
        auto bevent = PistonPullBlockEvent(a3, *this, a2);
        ll::event::EventBus::getInstance().publish(bevent);
    } else {
        auto bevent = PistonPushBlockEvent(a3, *this, a2);
        ll::event::EventBus::getInstance().publish(bevent);
    }
    return origin(a2, a3);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    PistonActorMovedHook,
    ll::memory::HookPriority::Normal,
    PistonBlockActor,
    "?_spawnBlocks@PistonBlockActor@@AEAAXAEAVBlockSource@@@Z",
    void,
    BlockSource* a2
) {
    isRetract = this->isRetracting();
    return origin(a2);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class PistonPullBlockEventEmitter : public ll::event::Emitter<emitterFactory1, PistonPullBlockEvent> {
    ll::memory::HookRegistrar<PistonActorMovedHook,BlockMovedHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<PistonPullBlockEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class PistonPushBlockEventEmitter : public ll::event::Emitter<emitterFactory2, PistonPushBlockEvent> {
    ll::memory::HookRegistrar<PistonActorMovedHook,BlockMovedHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<PistonPushBlockEventEmitter>();
}
} // namespace GMLIB::Event::RedStoneEvent
