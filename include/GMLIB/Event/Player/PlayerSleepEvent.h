#pragma once
#include "GMLIB/DllExport.h"
#include <ll/api/event/player/PlayerEvent.h>

namespace GMLIB::Event::PlayerEvent {

class PlayerStartSleepBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    Player&   mPlayer;
    BlockPos& mBlockPos;

public:
    constexpr explicit PlayerStartSleepBeforeEvent(Player& player, BlockPos& pos) : mPlayer(player), mBlockPos(pos) {}

    GMLIB_API Player const&   getPlayer() const;
    GMLIB_API BlockPos const& getPosition() const;
};

class PlayerStartSleepAfterEvent : public ll::event::Event {
    Player&   mPlayer;
    BlockPos& mBlockPos;
    bool      mResult;

public:
    constexpr explicit PlayerStartSleepAfterEvent(Player& player, BlockPos& pos, int result)
    : mPlayer(player),
      mBlockPos(pos) {
        mResult = result == 0 ? true : false;
    }

    GMLIB_API Player const&   getPlayer() const;
    GMLIB_API BlockPos const& getPosition() const;
    GMLIB_API bool const&     getResult() const;
};

class PlayerStopSleepBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    Player& mPlayer;
    bool    mForcefulWakeUp;
    bool    mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepBeforeEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : mPlayer(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_API Player const& getPlayer() const;
    GMLIB_API bool const&   isForcefulWakeUp() const;
    GMLIB_API bool const&   isUpdateLevelList() const;
};

class PlayerStopSleepAfterEvent : public ll::event::Event {
    Player& mPlayer;
    bool    mForcefulWakeUp;
    bool    mUpdateLevelList;

public:
    constexpr explicit PlayerStopSleepAfterEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : mPlayer(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    GMLIB_API Player const& getPlayer() const;
    GMLIB_API bool const&   isForcefulWakeUp() const;
    GMLIB_API bool const&   isUpdateLevelList() const;
};

} // namespace GMLIB::Event::PlayerEvent