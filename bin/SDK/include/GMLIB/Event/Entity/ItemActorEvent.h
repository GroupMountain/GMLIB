#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/actor/item/ItemActor.h"

namespace GMLIB::Event::EntityEvent {

class ItemActorEvent : public ll::event::entity::ActorEvent {
protected:
    constexpr explicit ItemActorEvent(ItemActor& itemActor) : ActorEvent(itemActor) {}

public:
    GMLIB_NDAPI ItemActor& getItemActor() const;
};

} // namespace GMLIB::Event::EntityEvent