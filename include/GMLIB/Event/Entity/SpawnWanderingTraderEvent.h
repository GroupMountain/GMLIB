#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

namespace GMLIB::Event::EntityEvent {

class SpawnWanderingTraderBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    BlockPos const    mPos;
    BlockSource& mRegion;

public:
    constexpr explicit SpawnWanderingTraderBeforeEvent(BlockPos const& pos, class BlockSource& region)
    : Cancellable(),
      mPos(pos),
      mRegion(region) {}

    GMLIB_NDAPI BlockPos const getPos() const;
    GMLIB_NDAPI BlockSource&    getRegion() const;
};

class SpawnWanderingTraderAfterEvent final : public ll::event::Event {
    BlockPos const     mPos;
    BlockSource& mRegion;

public:
    constexpr explicit SpawnWanderingTraderAfterEvent(BlockPos const& pos, class BlockSource& region)
    : mPos(pos),
      mRegion(region) {}

    GMLIB_NDAPI BlockPos const getPos() const;
    GMLIB_NDAPI BlockSource&    getRegion() const;
};

} // namespace GMLIB::Event::EntityEvent