#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/ActorUniqueID.h"
#include "mc/world/actor/monster/EnderDragon.h"

namespace GMLIB::Event::EntityEvent {

class DragonRespawnBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    ActorUniqueID mUniqueId;

public:
    constexpr explicit DragonRespawnBeforeEvent(ActorUniqueID& uniqueId) : Cancellable(), mUniqueId(uniqueId) {}

    GMLIB_NDAPI ActorUniqueID const getEnderDragon() const;
};

class DragonRespawnAfterEvent final : public ll::event::entity::ActorEvent {
protected:
    optional_ref<EnderDragon> mDragon;

public:
    constexpr explicit DragonRespawnAfterEvent(optional_ref<EnderDragon> dragon) : ActorEvent(dragon) {}

    GMLIB_NDAPI optional_ref<EnderDragon> getEnderDragon() const;
};

} // namespace GMLIB::Event::EntityEvent