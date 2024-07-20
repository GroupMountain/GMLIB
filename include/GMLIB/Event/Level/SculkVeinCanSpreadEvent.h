#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/common/wrapper/optional_ref.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/chunk/LevelChunk.h"


namespace GMLIB::Event::LevelEvent {

class SculkVeinCanSpreadEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    Block const&             mBlock;
    BlockPos const&          mPos;
    uchar                    mFace;
    optional_ref<LevelChunk> mLevelChunk;

public:
    constexpr explicit SculkVeinCanSpreadEvent(
        Block const&             block,
        BlockPos const&          pos,
        uchar                    face,
        optional_ref<LevelChunk> levelChunk
    )
    : Cancellable(),
      mBlock(block),
      mPos(pos),
      mFace(face),
      mLevelChunk(levelChunk) {}


    GMLIB_NDAPI Block const&    getBlock() const;
    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI uchar           getFace() const;
    GMLIB_NDAPI optional_ref<LevelChunk> getLevelChunk() const;
};


} // namespace GMLIB::Event::LevelEvent