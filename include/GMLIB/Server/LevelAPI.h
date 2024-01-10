#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/enums/AllExperiments.h>
#include <mc/world/level/Level.h>

enum class WeatherType : int {
    Clear   = 0, // Sunny
    Rain    = 1, // Rainy
    Thunder = 2  // Lightning
};

namespace GMLIB::LevelAPI {

GMLIB_API BlockSource* getBlockSource(DimensionType dimid);

GMLIB_API std::vector<Actor*> getAllEntities();

GMLIB_API std::string getLevelName();
GMLIB_API void        setLevelName(std::string newName);
GMLIB_API void        setFakeLevelName(std::string fakeName);

GMLIB_API int64_t getSeed();
GMLIB_API void    setFakeSeed(int64_t fakeSeed);

GMLIB_API void setCoResourcePack(bool enabled = true);

GMLIB_API void setForceTrustSkin(bool enabled = true);

GMLIB_API bool getExperimentEnabled(::AllExperiments experiment);
GMLIB_API void setExperimentEnabled(::AllExperiments experiment, bool enabled = true);
GMLIB_API void addExperimentsRequire(::AllExperiments experiment);

GMLIB_API void setForceAchievementsEnabled();

GMLIB_API void forceEnableAbilityCommand();
GMLIB_API void addEducationEditionRequired();

GMLIB_API int  getTime(int time);
GMLIB_API void setTime(int time);

GMLIB_API WeatherType getWeather();
GMLIB_API void        setWeather(WeatherType weather);
GMLIB_API void        setWeather(WeatherType weather, int lastTick);
GMLIB_API void        setClientWeather(WeatherType weather, Player* pl);


} // namespace GMLIB::LevelAPI
