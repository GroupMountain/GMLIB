#include "GMLIB/Event/RedStone/BlockMovedByPistonEvent.h"
#include "Global.h"
#include "mc/world/level/block/actor/PistonBlockActor.h"
#include <string>

namespace GMLIB::Event::RedStoneEvent {

GMLIB_API Block&          BlockMovedByPistonAfterEvent::getBlock() const { return mBlock; }
GMLIB_API BlockPos const& BlockMovedByPistonAfterEvent::getPosition() const { return mMovedBlockPos; }

LL_TYPE_INSTANCE_HOOK(
    BlockMovedByPistonEventHook,
    ll::memory::HookPriority::Normal,
    Block,
    "?movedByPiston@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@@Z",
    void,
    BlockSource&    region,
    const BlockPos& pos
) {
    auto event = BlockMovedByPistonAfterEvent(*this, region, pos);
    ll::event::EventBus::getInstance().publish(event);
    return origin(region, pos);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class BlockMovedByPistonAfterEventEmitter : public ll::event::Emitter<emitterFactory, BlockMovedByPistonAfterEvent> {
    ll::memory::HookRegistrar<BlockMovedByPistonEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<BlockMovedByPistonAfterEventEmitter>();
}

} // namespace GMLIB::Event::RedStoneEvent
