#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/block/Block.h"


namespace GMLIB::Event::LevelEvent {

class SculkVeinCanSpreadEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    Block const&    mBlock;
    BlockPos const& mPos;
    uchar           mFace;

public:
    constexpr explicit SculkVeinCanSpreadEvent(Block const& block, BlockPos const& pos, uchar face)
    : Cancellable(),
      mBlock(block),
      mPos(pos),
      mFace(face) {}


    GMLIB_NDAPI Block const&    getBlock() const;
    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI uchar           getFace() const;
};


} // namespace GMLIB::Event::LevelEvent