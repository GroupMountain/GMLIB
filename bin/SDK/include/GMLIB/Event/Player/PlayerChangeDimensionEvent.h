#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

namespace GMLIB::Event::PlayerEvent {

class PlayerChangeDimensionBeforeEvent final : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    DimensionType& mFromDimensionId;
    DimensionType& mToDimensionId;
    Vec3&          mFromPosition;
    Vec3&          mToPosition;
    bool&          mUsePortal;
    bool&          mRespawn;

public:
    constexpr explicit PlayerChangeDimensionBeforeEvent(
        Player&        player,
        DimensionType& fromDimId,
        DimensionType& toDimId,
        Vec3&          fromPosition,
        Vec3&          toPosition,
        bool&          usePortal,
        bool&          respawn
    )
    : Cancellable(player),
      mFromDimensionId(fromDimId),
      mToDimensionId(toDimId),
      mFromPosition(fromPosition),
      mToPosition(fromPosition),
      mUsePortal(usePortal),
      mRespawn(respawn) {}

    GMLIB_NDAPI DimensionType& getFromDimensionId() const;
    GMLIB_NDAPI DimensionType& getToDimensionId() const;
    GMLIB_NDAPI Vec3&          getFromPosition() const;
    GMLIB_NDAPI Vec3&          getToPosition() const;
    GMLIB_NDAPI bool&          isUsingPortal() const;
    GMLIB_NDAPI bool&          isRespawn() const;
};

class PlayerChangeDimensionAfterEvent final : public ll::event::player::PlayerEvent {
protected:
    DimensionType& mFromDimensionId;
    DimensionType& mToDimensionId;
    Vec3&          mFromPosition;
    Vec3&          mToPosition;
    bool&          mUsePortal;
    bool&          mRespawn;

public:
    constexpr explicit PlayerChangeDimensionAfterEvent(
        Player&        player,
        DimensionType& fromDimId,
        DimensionType& toDimId,
        Vec3&          fromPosition,
        Vec3&          toPosition,
        bool&          usePortal,
        bool&          respawn
    )
    : PlayerEvent(player),
      mFromDimensionId(fromDimId),
      mToDimensionId(toDimId),
      mFromPosition(fromPosition),
      mToPosition(fromPosition),
      mUsePortal(usePortal),
      mRespawn(respawn) {}

    GMLIB_NDAPI DimensionType& getFromDimensionId() const;
    GMLIB_NDAPI DimensionType& getToDimensionId() const;
    GMLIB_NDAPI Vec3&          getFromPosition() const;
    GMLIB_NDAPI Vec3&          getToPosition() const;
    GMLIB_NDAPI bool&          isUsingPortal() const;
    GMLIB_NDAPI bool&          isRespawn() const;
};

} // namespace GMLIB::Event::PlayerEvent