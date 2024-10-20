#pragma once
#include "Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

namespace gmlib::event::entity {

class DeathMessageBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
protected:
    ActorDamageSource& mDamageSource;

public:
    constexpr explicit DeathMessageBeforeEvent(Actor& actor, ActorDamageSource& damageSource)
    : Cancellable(actor),
      mDamageSource(damageSource) {}

    GMLIB_NDAPI ActorDamageSource& getDamageSource() const;
};

class DeathMessageAfterEvent final : public ll::event::entity::ActorEvent {
    ActorDamageSource&                                mDamageSource;
    std::pair<std::string, std::vector<std::string>>& mDeathMessage;

public:
    constexpr explicit DeathMessageAfterEvent(
        Actor&                                            actor,
        ActorDamageSource&                                damageSource,
        std::pair<std::string, std::vector<std::string>>& deathMessage
    )
    : ActorEvent(actor),
      mDamageSource(damageSource),
      mDeathMessage(deathMessage) {}

    GMLIB_NDAPI ActorDamageSource& getDamageSource() const;
    GMLIB_NDAPI std::pair<std::string, std::vector<std::string>>& getDeathMessage() const;
};

} // namespace gmlib::event::entity