#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/world/WorldEvent.h"

namespace GMLIB::Event::RedStoneEvent {

class PistonPushBeforeEvent final : public ll::event::Cancellable<ll::event::WorldEvent> {
protected:
    BlockPos const& mBlockPos;
    uchar const&    mCurrentBranchFacing;
    uchar const&    mPistonMoveFacing;
    bool const&     mIsRetracting;

public:
    constexpr explicit PistonPushBeforeEvent(
        BlockSource&    region,
        BlockPos const& currentPos,
        uchar const&    currentBranchFacing,
        uchar const&    pistonMoveFacing,
        bool const&     isRetracting
    )
    : Cancellable(region),
      mBlockPos(currentPos),
      mCurrentBranchFacing(currentBranchFacing),
      mPistonMoveFacing(pistonMoveFacing),
      mIsRetracting(isRetracting) {}

    GMLIB_NDAPI BlockPos const& getPosition() const;
    GMLIB_NDAPI uchar const&    getBranchFacing() const;
    GMLIB_NDAPI uchar const&    getPistonMoveFacing() const;
    GMLIB_NDAPI bool const&     isRetracting() const;
};

class PistonPushAfterEvent final : public ll::event::WorldEvent {
protected:
    BlockPos const& mBlockPos;
    uchar const&    mCurrentBranchFacing;
    uchar const&    mPistonMoveFacing;
    bool const&     mIsRetracting;

public:
    constexpr explicit PistonPushAfterEvent(
        BlockSource&    region,
        BlockPos const& currentPos,
        uchar const&    currentBranchFacing,
        uchar const&    pistonMoveFacing,
        bool const&     isRetracting
    )
    : WorldEvent(region),
      mBlockPos(currentPos),
      mCurrentBranchFacing(currentBranchFacing),
      mPistonMoveFacing(pistonMoveFacing),
      mIsRetracting(isRetracting) {}

    GMLIB_NDAPI BlockPos const& getPosition() const;
    GMLIB_NDAPI uchar const&    getBranchFacing() const;
    GMLIB_NDAPI uchar const&    getPistonMoveFacing() const;
    GMLIB_NDAPI bool const&     isRetracting() const;
};

} // namespace GMLIB::Event::RedStoneEvent