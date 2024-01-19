#pragma once
#include "GMLIB/GMLIB.h"
#include <ll/api/event/entity/ActorEvent.h>

namespace GMLIB::Event::EntityEvent {

class ItemActorEvent : public ll::event::entity::ActorEvent {
protected:
    constexpr explicit ItemActorEvent(ItemActor& itemActor) : ActorEvent(itemActor) {}

public:
    GMLIB_API ItemActor& getItemActor() const;
};

} // namespace GMLIB::Event::EntityEvent