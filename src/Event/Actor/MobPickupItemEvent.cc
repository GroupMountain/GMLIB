#include "Global.h"
#include <GMLIB/Event/entity/MobPickupItemEvent.h>

namespace GMLIB::Event::EntityEvent {

ItemActor const& MobPickupItemBeforeEvent::getItemActor() const { return mItemActor; }

ItemActor const& MobPickupItemAfterEvent::getItemActor() const { return mItemActor; }

LL_AUTO_TYPE_INSTANCE_HOOK(
    MobPickupItemEventHook,
    ll::memory::HookPriority::Normal,
    PickupItemsGoal,
    "?_pickItemUp@PickupItemsGoal@@AEAAXPEAVItemActor@@@Z",
    void,
    class ItemActor& item
) {
    auto mob         = ll::memory::dAccess<Mob*>(this, 112); // IDA: PickupItemsGoal::PickupItemsGoal()
    auto beforeEvent = MobPickupItemBeforeEvent(*mob, item);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(item);
    auto afterEvent = MobPickupItemAfterEvent(*mob, item);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

} // namespace GMLIB::Event::EntityEvent
