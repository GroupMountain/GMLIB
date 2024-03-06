#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

namespace GMLIB::Event::PlayerEvent {

class PlayerChangeDimensionBeforeEvent : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
    DimensionType& mFromDimensionId;
    DimensionType& mToDimensionId;
    Vec3&          mFromPosition;
    Vec3&          mToPosition;
    bool           mUsePortal;
    bool           mRespawn;

public:
    constexpr explicit PlayerChangeDimensionBeforeEvent(
        Player&        player,
        DimensionType& fromDimId,
        DimensionType& toDimId,
        Vec3&          fromPosition,
        Vec3&          toPosition,
        bool           usePortal,
        bool           respawn
    )
    : Cancellable(player),
      mFromDimensionId(fromDimId),
      mToDimensionId(toDimId),
      mFromPosition(fromPosition),
      mToPosition(fromPosition),
      mUsePortal(usePortal),
      mRespawn(respawn) {}

    GMLIB_API DimensionType const& getFromDimensionId() const;
    GMLIB_API DimensionType const& getToDimensionId() const;
    GMLIB_API DimensionType&       getToDimensionId();
    GMLIB_API Vec3 const&          getFromPosition() const;
    GMLIB_API Vec3 const&          getToPosition() const;
    GMLIB_API Vec3&                getToPosition();
    GMLIB_API bool                 isUsingPortal() const;
    GMLIB_API bool                 isRespawn() const;
};

class PlayerChangeDimensionAfterEvent : public ll::event::player::PlayerEvent {
    DimensionType& mFromDimensionId;
    DimensionType& mToDimensionId;
    Vec3&          mFromPosition;
    Vec3&          mToPosition;
    bool           mUsePortal;
    bool           mRespawn;

public:
    constexpr explicit PlayerChangeDimensionAfterEvent(
        Player&        player,
        DimensionType& fromDimId,
        DimensionType& toDimId,
        Vec3&          fromPosition,
        Vec3&          toPosition,
        bool           usePortal,
        bool           respawn
    )
    : PlayerEvent(player),
      mFromDimensionId(fromDimId),
      mToDimensionId(toDimId),
      mFromPosition(fromPosition),
      mToPosition(fromPosition),
      mUsePortal(usePortal),
      mRespawn(respawn) {}

    GMLIB_API DimensionType const& getFromDimensionId() const;
    GMLIB_API DimensionType const& getToDimensionId() const;
    GMLIB_API Vec3 const&          getFromPosition() const;
    GMLIB_API Vec3 const&          getToPosition() const;
    GMLIB_API bool                 isUsingPortal() const;
    GMLIB_API bool                 isRespawn() const;
};

} // namespace GMLIB::Event::PlayerEvent