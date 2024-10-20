#pragma once
#include "Macros.h"
#include "ll/api/event/entity/MobEvent.h"
#include "mc/world/actor/ActorDamageSource.h"

namespace gmlib::event::entity {

class MobHurtAfterEvent final : public ll::event::MobEvent {
protected:
    ActorDamageSource& mSource;
    float&             mDamage;

public:
    constexpr explicit MobHurtAfterEvent(Mob& mob, ActorDamageSource& source, float& damage)
    : MobEvent(mob),
      mSource(source),
      mDamage(damage) {}

    GMLIB_NDAPI ActorDamageSource& getSource() const;
    GMLIB_NDAPI float&             getDamage() const;
};

} // namespace gmlib::event::entity