#include "Global.h"
#include <GMLIB/Event/Player/PlayerChangeDimensionEvent.h>
#include <mc/world/level/ChangeDimensionRequest.h>

namespace GMLIB::Event::PlayerEvent {

DimensionType& PlayerChangeDimensionBeforeEvent::getFromDimensionId() const { return mFromDimensionId; }
DimensionType& PlayerChangeDimensionBeforeEvent::getToDimensionId() const { return mToDimensionId; }
Vec3&          PlayerChangeDimensionBeforeEvent::getFromPosition() const { return mFromPosition; }
Vec3&          PlayerChangeDimensionBeforeEvent::getToPosition() const { return mToPosition; }
bool&          PlayerChangeDimensionBeforeEvent::isUsingPortal() const { return mUsePortal; }
bool&          PlayerChangeDimensionBeforeEvent::isRespawn() const { return mRespawn; }


DimensionType& PlayerChangeDimensionAfterEvent::getFromDimensionId() const { return mFromDimensionId; }
DimensionType& PlayerChangeDimensionAfterEvent::getToDimensionId() const { return mToDimensionId; }
Vec3&          PlayerChangeDimensionAfterEvent::getFromPosition() const { return mFromPosition; }
Vec3&          PlayerChangeDimensionAfterEvent::getToPosition() const { return mToPosition; }
bool&          PlayerChangeDimensionAfterEvent::isUsingPortal() const { return mUsePortal; }
bool&          PlayerChangeDimensionAfterEvent::isRespawn() const { return mRespawn; }

LL_TYPE_INSTANCE_HOOK(
    PlayerChangeDimensionEventHook,
    HookPriority::Normal,
    Level,
    "?requestPlayerChangeDimension@Level@@UEAAXAEAVPlayer@@$$QEAVChangeDimensionRequest@@@Z",
    void,
    class Player&                  player,
    class ChangeDimensionRequest&& changeRequest
) {
    PlayerChangeDimensionBeforeEvent beforeEvent = PlayerChangeDimensionBeforeEvent(
        player,
        changeRequest.mFromDimensionId,
        changeRequest.mToDimensionId,
        changeRequest.mFormPosition,
        changeRequest.mToPosition,
        changeRequest.mUsePortal,
        changeRequest.mRespawn
    );
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(player, std::move(changeRequest));
    PlayerChangeDimensionAfterEvent afterEvent = PlayerChangeDimensionAfterEvent(
        player,
        changeRequest.mFromDimensionId,
        changeRequest.mToDimensionId,
        changeRequest.mFormPosition,
        changeRequest.mToPosition,
        changeRequest.mUsePortal,
        changeRequest.mRespawn
    );
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class PlayerChangeDimensionBeforeEventEmitter
: public ll::event::Emitter<emitterFactory1, PlayerChangeDimensionBeforeEvent> {
    ll::memory::HookRegistrar<PlayerChangeDimensionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<PlayerChangeDimensionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class PlayerChangeDimensionAfterEventEmitter
: public ll::event::Emitter<emitterFactory2, PlayerChangeDimensionAfterEvent> {
    ll::memory::HookRegistrar<PlayerChangeDimensionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<PlayerChangeDimensionAfterEventEmitter>();
}

} // namespace GMLIB::Event::PlayerEvent