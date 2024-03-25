#include "Global.h"
#include <GMLIB/Event/Level/ExplodeEvent.h>

namespace GMLIB::Event::LevelEvent {

BlockSource&        ExplodeBeforeEvent::getBlockSource() const { return mBlockSource; }
optional_ref<Actor> ExplodeBeforeEvent::getSource() const { return mSource; }
Vec3&               ExplodeBeforeEvent::getPosition() const { return mPosition; }
float&              ExplodeBeforeEvent::getPower() const { return mPower; }
bool&               ExplodeBeforeEvent::canCauseFire() const { return mFire; }
bool&               ExplodeBeforeEvent::canBreakBlocks() const { return mBreakBlocks; }
float&              ExplodeBeforeEvent::getMaxResistance() const { return mMaxResistance; }
bool&               ExplodeBeforeEvent::canOverrideUnderwater() const { return mAllowUnderwater; }

BlockSource&        ExplodeAfterEvent::getBlockSource() const { return mBlockSource; }
optional_ref<Actor> ExplodeAfterEvent::getSource() const { return mSource; }
Vec3&               ExplodeAfterEvent::getPosition() const { return mPosition; }
float&              ExplodeAfterEvent::getPower() const { return mPower; }
bool&               ExplodeAfterEvent::canCauseFire() const { return mFire; }
bool&               ExplodeAfterEvent::canBreakBlocks() const { return mBreakBlocks; }
float&              ExplodeAfterEvent::getMaxResistance() const { return mMaxResistance; }
bool&               ExplodeAfterEvent::canOverrideUnderwater() const { return mAllowUnderwater; }

LL_TYPE_INSTANCE_HOOK(
    ExplodeEventHook,
    ll::memory::HookPriority::Normal,
    Level,
    "?explode@Level@@UEAA_NAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
    void,
    class BlockSource& region,
    class Actor*       source,
    class Vec3&        pos,
    float              power,
    bool               fire,
    bool               breaksBlocks,
    float              maxResistance,
    bool               allowUnderwater
) {
    ExplodeBeforeEvent beforeEvent =
        ExplodeBeforeEvent(*this, region, source, pos, power, fire, breaksBlocks, maxResistance, allowUnderwater);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(region, source, pos, power, fire, breaksBlocks, maxResistance, allowUnderwater);
    ExplodeAfterEvent afterEvent =
        ExplodeAfterEvent(*this, region, source, pos, power, fire, breaksBlocks, maxResistance, allowUnderwater);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ExplodeBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, ExplodeBeforeEvent> {
    ll::memory::HookRegistrar<ExplodeEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ExplodeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ExplodeAfterEventEmitter : public ll::event::Emitter<emitterFactory2, ExplodeAfterEvent> {
    ll::memory::HookRegistrar<ExplodeEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ExplodeAfterEventEmitter>();
}

} // namespace GMLIB::Event::LevelEvent
