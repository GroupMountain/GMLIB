#pragma once
#include "Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"


namespace gmlib::event::entity {

class MobPickupItemBeforeEvent final : public ll::event::Cancellable<ll::event::entity::MobEvent> {
protected:
    ItemActor& mItemActor;

public:
    constexpr explicit MobPickupItemBeforeEvent(Mob& mob, ItemActor& itemActor)
    : Cancellable(mob),
      mItemActor(itemActor) {}

    GMLIB_NDAPI ItemActor& getItemActor() const;
};

class MobPickupItemAfterEvent final : public ll::event::entity::MobEvent {
protected:
    ItemActor& mItemActor;

public:
    constexpr explicit MobPickupItemAfterEvent(Mob& mob, ItemActor& itemActor) : MobEvent(mob), mItemActor(itemActor) {}

    GMLIB_NDAPI ItemActor& getItemActor() const;
};

} // namespace gmlib::event::entity