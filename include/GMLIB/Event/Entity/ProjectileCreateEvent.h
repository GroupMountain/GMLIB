#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

namespace GMLIB::Event::EntityEvent {

class ProjectileCreateBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
protected:
    optional_ref<Actor> mShooter;

public:
    constexpr explicit ProjectileCreateBeforeEvent(Actor& actor, optional_ref<Actor> shooter)
    : Cancellable(actor),
      mShooter(shooter) {}

    GMLIB_NDAPI optional_ref<Actor> getShooter() const;
};

class ProjectileCreateAfterEvent final : public ll::event::entity::ActorEvent {
protected:
    optional_ref<Actor> mShooter;

public:
    constexpr explicit ProjectileCreateAfterEvent(Actor& actor, optional_ref<Actor> shooter)
    : ActorEvent(actor),
      mShooter(shooter) {}

    GMLIB_NDAPI optional_ref<Actor> getShooter() const;
};

} // namespace GMLIB::Event::EntityEvent