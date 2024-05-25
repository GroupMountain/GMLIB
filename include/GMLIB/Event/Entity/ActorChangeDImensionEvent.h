#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

namespace GMLIB::Event::EntityEvent {

class ActorChangeDimensionBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
protected:
    DimensionType const mFromDimensionType;
    DimensionType&      mToDimensionType;

public:
    constexpr explicit ActorChangeDimensionBeforeEvent(
        Actor&              actor,
        DimensionType const fromDimensionType,
        DimensionType&      toDimensionType
    )
    : Cancellable(actor),
      mFromDimensionType(fromDimensionType),
      mToDimensionType(toDimensionType) {}

    GMLIB_NDAPI DimensionType const getFromDimensionId() const;
    GMLIB_NDAPI DimensionType&      getToDimensionId() const;
};

class ActorChangeDimensionAfterEvent final : public ll::event::entity::ActorEvent {
    DimensionType const mFromDimensionType;
    DimensionType&      mToDimensionType;

public:
    constexpr explicit ActorChangeDimensionAfterEvent(
        Actor&              actor,
        DimensionType const fromDimensionType,
        DimensionType&      toDimensionType
    )
    : ActorEvent(actor),
      mFromDimensionType(fromDimensionType),
      mToDimensionType(toDimensionType) {}

    GMLIB_NDAPI DimensionType const getFromDimensionId() const;
    GMLIB_NDAPI DimensionType&      getToDimensionId() const;
};

} // namespace GMLIB::Event::EntityEvent