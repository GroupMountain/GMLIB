#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/world/WorldEvent.h"

namespace GMLIB::Event::RedStoneEvent {


class BlockMovedByPistonAfterEvent final : public ll::event::world::WorldEvent {
protected:
    BlockPos const& mMovedBlockPos;
    Block&          mBlock;

public:
    constexpr explicit BlockMovedByPistonAfterEvent(
        Block&          block,
        BlockSource&    blockSource,
        BlockPos const& movedBlockPos
    )
    : WorldEvent(blockSource),
      mMovedBlockPos(movedBlockPos),
      mBlock(block) {}

    GMLIB_API BlockPos const& getPosition() const;
    GMLIB_API Block&          getBlock() const;
};

} // namespace GMLIB::Event::RedStoneEvent