#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

namespace GMLIB::Event::PlayerEvent {

class PlayerStartSleepBeforeEvent final : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    BlockPos& mBlockPos;

public:
    constexpr explicit PlayerStartSleepBeforeEvent(Player& player, BlockPos& pos)
    : Cancellable(player),
      mBlockPos(pos) {}

    GMLIB_NDAPI BlockPos& getPosition() const;
};

class PlayerStartSleepAfterEvent final : public ll::event::player::PlayerEvent {
protected:
    BlockPos&            mBlockPos;
    ::BedSleepingResult& mResult;

public:
    constexpr explicit PlayerStartSleepAfterEvent(Player& player, BlockPos& pos, BedSleepingResult result)
    : PlayerEvent(player),
      mBlockPos(pos),
      mResult(result) {}

    GMLIB_NDAPI BlockPos&            getPosition() const;
    GMLIB_NDAPI ::BedSleepingResult& getResult() const;
};

class PlayerStopSleepBeforeEvent final : public ll::event::Cancellable<ll::event::player::PlayerEvent> {
protected:
    bool& mForcefulWakeUp;
    bool& mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepBeforeEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : Cancellable(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_NDAPI bool& isForcefulWakeUp() const;
    GMLIB_NDAPI bool& isUpdateLevelList() const;
};

class PlayerStopSleepAfterEvent final : public ll::event::player::PlayerEvent {
protected:
    bool& mForcefulWakeUp;
    bool& mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepAfterEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : PlayerEvent(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_NDAPI bool& isForcefulWakeUp() const;
    GMLIB_NDAPI bool& isUpdateLevelList() const;
};

} // namespace GMLIB::Event::PlayerEvent