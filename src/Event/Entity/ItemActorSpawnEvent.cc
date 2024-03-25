#include "Global.h"
#include <GMLIB/Event/entity/ItemActorSpawnEvent.h>

namespace GMLIB::Event::EntityEvent {

BlockSource&        ItemActorSpawnBeforeEvent::getBlockSource() const { return mBlockSource; }
Vec3&               ItemActorSpawnBeforeEvent::getPosition() const { return mPosition; }
ItemStack&          ItemActorSpawnBeforeEvent::getItem() const { return mItem; }
optional_ref<Actor> ItemActorSpawnBeforeEvent::getSpawner() const { return mSpawner; }
int&                ItemActorSpawnBeforeEvent::getThrowTime() const { return mThrowTime; }

BlockSource&        ItemActorSpawnAfterEvent::getBlockSource() const { return mBlockSource; }
Vec3&               ItemActorSpawnAfterEvent::getPosition() const { return mPosition; }
ItemStack&          ItemActorSpawnAfterEvent::getItem() const { return mItem; }
optional_ref<Actor> ItemActorSpawnAfterEvent::getSpawner() const { return mSpawner; }
int&                ItemActorSpawnAfterEvent::getThrowTime() const { return mThrowTime; }

LL_TYPE_INSTANCE_HOOK(
    ItemActorSpawnEventHook,
    ll::memory::HookPriority::Normal,
    Spawner,
    "?spawnItem@Spawner@@QEAAPEAVItemActor@@AEAVBlockSource@@AEBVItemStack@@PEAVActor@@AEBVVec3@@H@Z",
    ItemActor*,
    class BlockSource& region,
    class ItemStack&   item,
    class Actor*       spawner,
    class Vec3&        pos,
    int                throwTime
) {
    auto beforeEvent = ItemActorSpawnBeforeEvent(region, pos, item, spawner, throwTime);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return nullptr;
    }
    auto result = origin(region, item, spawner, pos, throwTime);
    if (result) {
        auto afterEvent = ItemActorSpawnAfterEvent(*result, region, pos, item, spawner, throwTime);
        ll::event::EventBus::getInstance().publish(afterEvent);
    }
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ItemActorSpawnBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, ItemActorSpawnBeforeEvent> {
    ll::memory::HookRegistrar<ItemActorSpawnEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ItemActorSpawnBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ItemActorSpawnAfterEventEmitter : public ll::event::Emitter<emitterFactory2, ItemActorSpawnAfterEvent> {
    ll::memory::HookRegistrar<ItemActorSpawnEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ItemActorSpawnAfterEventEmitter>();
}

} // namespace GMLIB::Event::EntityEvent
