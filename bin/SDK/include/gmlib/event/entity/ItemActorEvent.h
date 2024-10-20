#pragma once
#include "Macros.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/actor/item/ItemActor.h"

namespace gmlib::event::entity {

class ItemActorEvent : public ll::event::entity::ActorEvent {
protected:
    constexpr explicit ItemActorEvent(ItemActor& itemActor) : ActorEvent(itemActor) {}

public:
    GMLIB_NDAPI ItemActor& getItemActor() const;
};

} // namespace gmlib::event::entity