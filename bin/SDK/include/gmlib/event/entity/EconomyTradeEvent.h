#pragma once
#include "Macros.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/actor/player/Player.h"

namespace gmlib::event::entity {

class EconomyTradeBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
protected:
    Player& mPlayer;

public:
    constexpr explicit EconomyTradeBeforeEvent(Actor& actor, Player& player) : Cancellable(actor), mPlayer(player) {}

    GMLIB_NDAPI Player& getPlayer() const;
};

class EconomyTradeAfterEvent final : public ll::event::entity::ActorEvent {
protected:
    Player& mPlayer;

public:
    constexpr explicit EconomyTradeAfterEvent(Actor& actor, Player& player) : ActorEvent(actor), mPlayer(player) {}

    GMLIB_NDAPI Player& getPlayer() const;
};

} // namespace gmlib::event::entity