#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/world/WorldEvent.h"

namespace GMLIB::Event::RedStoneEvent {


class PistonMoveBlockEvent : public ll::event::world::WorldEvent {
protected:
    BlockPos& mMovedBlockPos; // 被推动方块坐标
    Block&    mBlock;         // 被推动方块

public:
    constexpr explicit PistonMoveBlockEvent(BlockPos& MovedBlockPos, Block& block, BlockSource& BlockSource)
    : mMovedBlockPos(MovedBlockPos),
      mBlock(block),
      WorldEvent(BlockSource) {}

    GMLIB_API BlockPos& getPosition() const;
    GMLIB_API Block&    getBlock() const;
};

//推
class PistonPushBlockEvent final : public PistonMoveBlockEvent {
public:
    constexpr explicit PistonPushBlockEvent(BlockPos& MovedBlockPos, Block& Block, BlockSource& BlockSource)
    : PistonMoveBlockEvent(MovedBlockPos, Block, BlockSource) {}

};

//拉
class PistonPullBlockEvent final : public PistonMoveBlockEvent {
public:
    constexpr explicit PistonPullBlockEvent(BlockPos& MovedBlockPos, Block& Block, BlockSource& BlockSource)
    : PistonMoveBlockEvent(MovedBlockPos, Block, BlockSource) {}

};
} // namespace GMLIB::Event::RedStoneEvent