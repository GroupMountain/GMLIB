#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/ActorUniqueID.h"
#include "mc/world/actor/monster/EnderDragon.h"

namespace GMLIB::Event::EntityEvent {

class DragonRespawnBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
protected:
    ActorUniqueID mUniqueId;

public:
    constexpr explicit DragonRespawnBeforeEvent(ActorUniqueID& uniqueId) : Cancellable(), mUniqueId(uniqueId) {}

    GMLIB_API ActorUniqueID const getEnderDragon() const;
};

class DragonRespawnAfterEvent : public ll::event::entity::ActorEvent {
protected:
    optional_ref<EnderDragon> mDragon;

public:
    constexpr explicit DragonRespawnAfterEvent(optional_ref<EnderDragon> dragon) : ActorEvent(dragon) {}

    GMLIB_API optional_ref<EnderDragon> getEnderDragon() const;
};

} // namespace GMLIB::Event::EntityEvent