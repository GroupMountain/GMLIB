#include "Global.h"
#include <GMLIB/Event/Level/WeatherUpdateEvent.h>

namespace GMLIB::Event::LevelEvent {

int const& WeatherUpdateBeforeEvent::getRainingLastTick() const { return mRainingLastTick; }
int const& WeatherUpdateBeforeEvent::getLightningLastTick() const { return mLightningLastTick; }
int const& WeatherUpdateBeforeEvent::getRainLevel() const { return mRainLevel; }
int const& WeatherUpdateBeforeEvent::getLightningLevel() const { return mLightningLevel; }

int const& WeatherUpdateAfterEvent::getRainingLastTick() const { return mRainingLastTick; }
int const& WeatherUpdateAfterEvent::getLightningLastTick() const { return mLightningLastTick; }
int const& WeatherUpdateAfterEvent::getRainLevel() const { return mRainLevel; }
int const& WeatherUpdateAfterEvent::getLightningLevel() const { return mLightningLevel; }

LL_AUTO_TYPE_INSTANCE_HOOK(
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

} // namespace GMLIB::Event::LevelEvent
