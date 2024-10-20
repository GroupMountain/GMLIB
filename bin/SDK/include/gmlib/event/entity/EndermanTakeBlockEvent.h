#pragma once
#include "Macros.h"
#include "ll/api/event/entity/MobEvent.h"
#include "mc/world/level/block/Block.h"

namespace gmlib::event::entity {

class EndermanTakeBlockBeforeEvent final : public ll::event::Cancellable<ll::event::entity::MobEvent> {
public:
    constexpr explicit EndermanTakeBlockBeforeEvent(Mob& mob) : Cancellable(mob) {}
};

class EndermanTakeBlockAfterEvent final : public ll::event::entity::MobEvent {
protected:
    Block const& mBlock;

public:
    constexpr explicit EndermanTakeBlockAfterEvent(Mob& mob, Block const& block) : MobEvent(mob), mBlock(block) {}

    GMLIB_NDAPI Block const& getBlock() const;
};

} // namespace gmlib::event::entity