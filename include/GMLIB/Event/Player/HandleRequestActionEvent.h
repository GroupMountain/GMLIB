#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include <mc/world/inventory/network/ItemStackRequestActionHandler.h>

namespace GMLIB::Event::PlayerEvent {

class HandleRequestActionBeforeEvent final : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    ItemStackRequestAction& mRequestAction;

public:
    constexpr explicit HandleRequestActionBeforeEvent(Player& player, ItemStackRequestAction& requestAction)
    : Cancellable(player),
      mRequestAction(requestAction) {}

    GMLIB_NDAPI class ItemStackRequestAction& getRequestAction() const;
};

class HandleRequestActionAfterEvent final : public ll::event::player::PlayerEvent {
protected:
    ItemStackRequestAction const& mRequestAction;

public:
    constexpr explicit HandleRequestActionAfterEvent(Player& player, class ItemStackRequestAction const& requestAction)
    : PlayerEvent(player),
      mRequestAction(requestAction) {}

    GMLIB_NDAPI class ItemStackRequestAction const& getRequestAction() const;
};

} // namespace GMLIB::Event::PlayerEvent