#pragma once
#include "GMLIB/Event/Entity/ItemActorEvent.h"
#include "GMLIB/GMLIB.h"
#include <ll/api/event/entity/ActorEvent.h>

namespace GMLIB::Event::EntityEvent {

class ItemActorSpawnBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    BlockSource const& mBlockSource;
    Vec3&              mPosition;
    ItemStack&         mItem;
    Actor*             mSpawner = nullptr;
    int                mThrowTime;

public:
    constexpr explicit ItemActorSpawnBeforeEvent(
        BlockSource& blockSource,
        Vec3&        position,
        ItemStack&   item,
        Actor*       spawner,
        int          throwTime
    )
    : Cancellable(),
      mBlockSource(blockSource),
      mPosition(position),
      mItem(item),
      mSpawner(spawner),
      mThrowTime(throwTime) {}

    GMLIB_API BlockSource const& getBlockSource() const;
    GMLIB_API Vec3 const&        getPosition() const;
    GMLIB_API ItemStack const&   getItem() const;
    GMLIB_API Actor* const       getSpawner() const;
    GMLIB_API int const          getThrowTime() const;
};

class ItemActorSpawnAfterEvent : public ItemActorEvent {
    BlockSource const& mBlockSource;
    Vec3&              mPosition;
    ItemStack&         mItem;
    Actor*             mSpawner = nullptr;
    int                mThrowTime;

public:
    constexpr explicit ItemActorSpawnAfterEvent(
        ItemActor&   itemActor,
        BlockSource& blockSource,
        Vec3&        position,
        ItemStack&   item,
        Actor*       spawner,
        int          throwTime
    )
    : ItemActorEvent(itemActor),
      mBlockSource(blockSource),
      mPosition(position),
      mItem(item),
      mSpawner(spawner),
      mThrowTime(throwTime) {}

    GMLIB_API BlockSource const& getBlockSource() const;
    GMLIB_API Vec3 const&        getPosition() const;
    GMLIB_API ItemStack const&   getItem() const;
    GMLIB_API Actor* const       getSpawner() const;
    GMLIB_API int const          getThrowTime() const;
};

} // namespace GMLIB::Event::EntityEvent