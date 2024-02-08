#include "Global.h"
#include <GMLIB/Event/entity/ItemActorSpawnEvent.h>

namespace GMLIB::Event::EntityEvent {

BlockSource const& ItemActorSpawnBeforeEvent::getBlockSource() const { return mBlockSource; }
Vec3 const&        ItemActorSpawnBeforeEvent::getPosition() const { return mPosition; }
ItemStack const&   ItemActorSpawnBeforeEvent::getItem() const { return mItem; }
Actor* const       ItemActorSpawnBeforeEvent::getSpawner() const { return mSpawner; }
int const&         ItemActorSpawnBeforeEvent::getThrowTime() const { return mThrowTime; }

BlockSource const& ItemActorSpawnAfterEvent::getBlockSource() const { return mBlockSource; }
Vec3 const&        ItemActorSpawnAfterEvent::getPosition() const { return mPosition; }
ItemStack const&   ItemActorSpawnAfterEvent::getItem() const { return mItem; }
Actor* const       ItemActorSpawnAfterEvent::getSpawner() const { return mSpawner; }
int const&         ItemActorSpawnAfterEvent::getThrowTime() const { return mThrowTime; }

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
