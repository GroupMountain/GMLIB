#pragma once
#include "GMLIB/Event/Entity/ItemActorEvent.h"
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/actor/ActorDamageSource.h"

namespace GMLIB::Event::EntityEvent {

class ActorHurtBeforeEvent : public ll::event::Cancellable<ll::event::ActorEvent> {
    ActorDamageSource& mSource;
    float&             mDamage;
    bool&              mKnock;
    bool&              mIgnite;

public:
    constexpr explicit ActorHurtBeforeEvent(
        Actor&             actor,
        ActorDamageSource& source,
        float&             damage,
        bool&              knock,
        bool&              ignite
    )
    : Cancellable(actor),
      mSource(source),
      mDamage(damage),
      mKnock(knock),
      mIgnite(ignite) {}

    LLAPI void serialize(CompoundTag&) const override;
    LLAPI void deserialize(CompoundTag const&) override;

    LLNDAPI ActorDamageSource& source() const;
    LLNDAPI float&             damage() const;
    LLNDAPI bool&              knock() const;
    LLNDAPI bool&              ignite() const;
};
} // namespace GMLIB::Event::EntityEvent