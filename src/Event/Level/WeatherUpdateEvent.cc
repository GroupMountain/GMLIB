#include "Global.h"
#include <GMLIB/Event/Level/WeatherUpdateEvent.h>

namespace GMLIB::Event::LevelEvent {

int& WeatherUpdateBeforeEvent::getRainingLastTick() const { return mRainingLastTick; }
int& WeatherUpdateBeforeEvent::getLightningLastTick() const { return mLightningLastTick; }
int& WeatherUpdateBeforeEvent::getRainLevel() const { return mRainLevel; }
int& WeatherUpdateBeforeEvent::getLightningLevel() const { return mLightningLevel; }

int& WeatherUpdateAfterEvent::getRainingLastTick() const { return mRainingLastTick; }
int& WeatherUpdateAfterEvent::getLightningLastTick() const { return mLightningLastTick; }
int& WeatherUpdateAfterEvent::getRainLevel() const { return mRainLevel; }
int& WeatherUpdateAfterEvent::getLightningLevel() const { return mLightningLevel; }

LL_TYPE_INSTANCE_HOOK(
    WeatherUpdateEventHook,
    ll::memory::HookPriority::Normal,
    Level,
    "?updateWeather@Level@@UEAAXMHMH@Z",
    void,
    float rainLevel,
    int   rainTime,
    float lightningLevel,
    int   lightningTime
) {
    WeatherUpdateBeforeEvent beforeEvent =
        WeatherUpdateBeforeEvent(*this, rainLevel, rainTime, lightningLevel, lightningTime);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(rainLevel, rainTime, lightningLevel, lightningTime);
    WeatherUpdateAfterEvent afterEvent =
        WeatherUpdateAfterEvent(*this, rainLevel, rainTime, lightningLevel, lightningTime);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class WeatherUpdateBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, WeatherUpdateBeforeEvent> {
    ll::memory::HookRegistrar<WeatherUpdateEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<WeatherUpdateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class WeatherUpdateAfterEventEmitter : public ll::event::Emitter<emitterFactory2, WeatherUpdateAfterEvent> {
    ll::memory::HookRegistrar<WeatherUpdateEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<WeatherUpdateAfterEventEmitter>();
}

} // namespace GMLIB::Event::LevelEvent
