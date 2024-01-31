#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/entity/ActorEvent.h"

namespace GMLIB::Event::EntityEvent {

class DeathMessageBeforeEvent : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    ActorDamageSource& mDamageSource;

public:
    constexpr explicit DeathMessageBeforeEvent(Actor& actor, ActorDamageSource& damageSource)
    : Cancellable(actor),
      mDamageSource(damageSource) {}

    GMLIB_API ActorDamageSource const& getDamageSource() const;
};

class DeathMessageAfterEvent : public ll::event::entity::ActorEvent {
    ActorDamageSource&                               mDamageSource;
    std::pair<std::string, std::vector<std::string>> mDeathMessage;

public:
    constexpr explicit DeathMessageAfterEvent(
        Actor&                                           actor,
        ActorDamageSource&                               damageSource,
        std::pair<std::string, std::vector<std::string>> deathMessage
    )
    : ActorEvent(actor),
      mDamageSource(damageSource),
      mDeathMessage(deathMessage) {}

    GMLIB_API ActorDamageSource const& getDamageSource() const;
    GMLIB_API std::pair<std::string, std::vector<std::string>> const& getDeathMessage() const;
};

} // namespace GMLIB::Event::EntityEvent