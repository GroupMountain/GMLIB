#pragma once
#include "Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"


namespace gmlib::event::level {


class SculkBlockGrowthBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    BlockSource*    mSource;
    BlockPos const& mPos;

public:
    constexpr explicit SculkBlockGrowthBeforeEvent(BlockSource* source, BlockPos const& pos)
    : Cancellable(),
      mSource(source),
      mPos(pos) {}

    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI BlockSource*    getSource() const;
};


class SculkBlockGrowthAfterEvent final : public ll::event::Event {
protected:
    BlockSource*    mSource;
    BlockPos const& mPos;

public:
    constexpr explicit SculkBlockGrowthAfterEvent(BlockSource* source, BlockPos const& pos)
    : mSource(source),
      mPos(pos) {}

    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI BlockSource*    getSource() const;
};


} // namespace gmlib::event::level