#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/LevelEvent.h"


namespace GMLIB::Event::LevelEvent {

class WeatherUpdateBeforeEvent : public ll::event::Cancellable<ll::event::world::LevelEvent> {
    int mRainingLastTick;
    int mRainLevel;
    int mLightningLastTick;
    int mLightningLevel;

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

    GMLIB_API int const getRainingLastTick() const;
    GMLIB_API int const getLightningLastTick() const;
    GMLIB_API int const getRainLevel() const;
    GMLIB_API int const getLightningLevel() const;
};

class WeatherUpdateAfterEvent : public ll::event::world::LevelEvent {
    int mRainingLastTick;
    int mRainLevel;
    int mLightningLastTick;
    int mLightningLevel;

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

    GMLIB_API int const getRainingLastTick() const;
    GMLIB_API int const getLightningLastTick() const;
    GMLIB_API int const getRainLevel() const;
    GMLIB_API int const getLightningLevel() const;
};

} // namespace GMLIB::Event::LevelEvent