#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

namespace GMLIB::Event::PlayerEvent {

class PlayerStartSleepBeforeEvent : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    BlockPos& mBlockPos;

public:
    constexpr explicit PlayerStartSleepBeforeEvent(Player& player, BlockPos& pos)
    : Cancellable(player),
      mBlockPos(pos) {}

    GMLIB_API BlockPos& getPosition() const;
};

class PlayerStartSleepAfterEvent : public ll::event::player::PlayerEvent {
protected:
    BlockPos&            mBlockPos;
    ::BedSleepingResult& mResult;

public:
    constexpr explicit PlayerStartSleepAfterEvent(Player& player, BlockPos& pos, BedSleepingResult result)
    : PlayerEvent(player),
      mBlockPos(pos),
      mResult(result) {}

    GMLIB_API BlockPos&            getPosition() const;
    GMLIB_API ::BedSleepingResult& getResult() const;
};

class PlayerStopSleepBeforeEvent : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    bool& mForcefulWakeUp;
    bool& mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepBeforeEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : Cancellable(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_API bool& isForcefulWakeUp() const;
    GMLIB_API bool& isUpdateLevelList() const;
};

class PlayerStopSleepAfterEvent : public ll::event::player::PlayerEvent {
protected:
    bool& mForcefulWakeUp;
    bool& mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepAfterEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : PlayerEvent(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_API bool& isForcefulWakeUp() const;
    GMLIB_API bool& isUpdateLevelList() const;
};

} // namespace GMLIB::Event::PlayerEvent