#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/common/wrapper/optional_ref.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"


namespace GMLIB::Event::LevelEvent {


class SculkSpreadBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    BlockPos const&           mPos;
    optional_ref<BlockSource> mBlockSource;

public:
    constexpr explicit SculkSpreadBeforeEvent(BlockPos const& pos, optional_ref<BlockSource> blockSource)
    : Cancellable(),
      mPos(pos),
      mBlockSource(blockSource) {}

    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI optional_ref<BlockSource> getBlockSource() const;
};


class SculkSpreadAfterEvent final : public ll::event::Event {
protected:
    BlockPos const&           mPos;
    optional_ref<BlockSource> mBlockSource;

public:
    constexpr explicit SculkSpreadAfterEvent(BlockPos const& pos, optional_ref<BlockSource> blockSource)
    : mPos(pos),
      mBlockSource(blockSource) {}

    GMLIB_NDAPI BlockPos const& getPos() const;
    GMLIB_NDAPI optional_ref<BlockSource> getBlockSource() const;
};


} // namespace GMLIB::Event::LevelEvent