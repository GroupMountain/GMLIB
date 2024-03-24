#pragma once
#include "GMLIB/Event/Entity/ItemActorEvent.h"
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"


namespace GMLIB::Event::EntityEvent {

class ItemActorSpawnBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
protected:
    BlockSource&        mBlockSource;
    Vec3&               mPosition;
    ItemStack&          mItem;
    optional_ref<Actor> mSpawner;
    int&                mThrowTime;

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

    GMLIB_API BlockSource& getBlockSource() const;
    GMLIB_API Vec3&        getPosition() const;
    GMLIB_API ItemStack&   getItem() const;
    GMLIB_API optional_ref<Actor> getSpawner() const;
    GMLIB_API int&                getThrowTime() const;
};

class ItemActorSpawnAfterEvent : public ItemActorEvent {
protected:
    BlockSource&        mBlockSource;
    Vec3&               mPosition;
    ItemStack&          mItem;
    optional_ref<Actor> mSpawner;
    int&                mThrowTime;

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

    GMLIB_API BlockSource& getBlockSource() const;
    GMLIB_API Vec3&        getPosition() const;
    GMLIB_API ItemStack&   getItem() const;
    GMLIB_API optional_ref<Actor> getSpawner() const;
    GMLIB_API int&                getThrowTime() const;
};

} // namespace GMLIB::Event::EntityEvent