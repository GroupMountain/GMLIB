#include "Global.h"
#include <GMLIB/Event/RedStone/PistonPushEvent.h>
#include <mc/world/level/block/actor/PistonBlockActor.h>


namespace GMLIB::Event::RedStoneEvent {

LL_TYPE_INSTANCE_HOOK(
    PistonPushEventHook,
    ll::memory::HookPriority::Normal,
    PistonBlockActor,
    "?_attachedBlockWalker@PistonBlockActor@@AEAA_NAEAVBlockSource@@AEBVBlockPos@@EE@Z",
    bool,
    BlockSource&    region,
    const BlockPos& curPos,
    uchar           curBranchFacing,
    uchar           pistonMoveFacing
) {
    PistonPushBeforeEvent beforeEvent =
        PistonPushBeforeEvent(region, curPos, curBranchFacing, pistonMoveFacing, isRetracting());
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }
    auto                 result = origin(region, curPos, curBranchFacing, pistonMoveFacing);
    PistonPushAfterEvent afterEvent =
        PistonPushAfterEvent(region, curPos, curBranchFacing, pistonMoveFacing, isRetracting());
    ll::event::EventBus::getInstance().publish(afterEvent);
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class PistonPullBlockEventEmitter : public ll::event::Emitter<emitterFactory1, PistonPushBeforeEvent> {
    ll::memory::HookRegistrar<PistonPushEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<PistonPullBlockEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class PistonPushBlockEventEmitter : public ll::event::Emitter<emitterFactory2, PistonPushAfterEvent> {
    ll::memory::HookRegistrar<PistonPushEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<PistonPushBlockEventEmitter>();
}

} // namespace GMLIB::Event::RedStoneEvent
