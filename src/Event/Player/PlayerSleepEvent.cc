#include "Global.h"
#include <GMLIB/Event/Player/PlayerSleepEvent.h>


namespace GMLIB::Event::PlayerEvent {

Player const&   PlayerStartSleepBeforeEvent::getPlayer() const { return mPlayer; }
BlockPos const& PlayerStartSleepBeforeEvent::getPosition() const { return mBlockPos; }

Player const&   PlayerStartSleepAfterEvent::getPlayer() const { return mPlayer; }
BlockPos const& PlayerStartSleepAfterEvent::getPosition() const { return mBlockPos; }
bool const&     PlayerStartSleepAfterEvent::getResult() const { return mResult; }

Player const& PlayerStopSleepBeforeEvent::getPlayer() const { return mPlayer; }
bool const&   PlayerStopSleepBeforeEvent::isForcefulWakeUp() const { return mForcefulWakeUp; }
bool const&   PlayerStopSleepBeforeEvent::isUpdateLevelList() const { return mUpdateLevelList; }

Player const& PlayerStopSleepAfterEvent::getPlayer() const { return mPlayer; }
bool const&   PlayerStopSleepAfterEvent::isForcefulWakeUp() const { return mForcefulWakeUp; }
bool const&   PlayerStopSleepAfterEvent::isUpdateLevelList() const { return mUpdateLevelList; }

LL_AUTO_TYPED_INSTANCE_HOOK(
    PlayerStartSleepEventHook,
    ll::memory::HookPriority::Normal,
    Player,
    "?startSleepInBed@Player@@UEAA?AW4BedSleepingResult@@AEBVBlockPos@@@Z",
    int,
    BlockPos& pos
) {
    PlayerStartSleepBeforeEvent beforeEvent = PlayerStartSleepBeforeEvent(*this, pos);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return 1;
    }
    auto                       res        = origin(pos);
    PlayerStartSleepAfterEvent afterEvent = PlayerStartSleepAfterEvent(*this, pos, res);
    ll::event::EventBus::getInstance().publish(afterEvent);
    return res;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    PlayerStopSleepEventHook,
    ll::memory::HookPriority::Normal,
    Player,
    "?stopSleepInBed@Player@@UEAAX_N0@Z",
    void,
    bool forcefulWakeUp, bool updateLevelList
) {
    PlayerStopSleepBeforeEvent beforeEvent = PlayerStopSleepBeforeEvent(*this, forcefulWakeUp, updateLevelList);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(forcefulWakeUp, updateLevelList);
    PlayerStopSleepAfterEvent afterEvent = PlayerStopSleepAfterEvent(*this, forcefulWakeUp, updateLevelList);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

} // namespace GMLIB::Event::PlayerEvent