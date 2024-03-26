#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/LevelEvent.h"

namespace GMLIB::Event::LevelEvent {

class WeatherUpdateBeforeEvent : public ll::event::Cancellable<ll::event::world::LevelEvent> {
protected:
    int& mRainingLastTick;
    int& mRainLevel;
    int& mLightningLastTick;
    int& mLightningLevel;

public:
    constexpr explicit WeatherUpdateBeforeEvent(
        Level& level,
        int    rainingLastTick,
        int    rainLevel,
        int    lightningLastTick,
        int    lightningLevel
    )
    : Cancellable(level),
      mRainingLastTick(rainingLastTick),
      mRainLevel(rainLevel),
      mLightningLastTick(lightningLastTick),
      mLightningLevel(lightningLevel) {}

    GMLIB_API int& getRainingLastTick() const;
    GMLIB_API int& getLightningLastTick() const;
    GMLIB_API int& getRainLevel() const;
    GMLIB_API int& getLightningLevel() const;
};

class WeatherUpdateAfterEvent : public ll::event::world::LevelEvent {
protected:
    int& mRainingLastTick;
    int& mRainLevel;
    int& mLightningLastTick;
    int& mLightningLevel;

public:
    constexpr explicit WeatherUpdateAfterEvent(
        Level& level,
        int    rainingLastTick,
        int    rainLevel,
        int    lightningLastTick,
        int    lightningLevel
    )
    : LevelEvent(level),
      mRainingLastTick(rainingLastTick),
      mRainLevel(rainLevel),
      mLightningLastTick(lightningLastTick),
      mLightningLevel(lightningLevel) {}

    GMLIB_API int& getRainingLastTick() const;
    GMLIB_API int& getLightningLastTick() const;
    GMLIB_API int& getRainLevel() const;
    GMLIB_API int& getLightningLevel() const;
};

} // namespace GMLIB::Event::LevelEvent