#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/enums/AllExperiments.h>
#include <mc/world/level/Level.h>

namespace GMLIB::LevelAPI {

GMLIB_API BlockSource* getBlockSource(DimensionType dimid);

GMLIB_API std::vector<Actor*> getAllEntities();

GMLIB_API std::string getLevelName();
GMLIB_API void        setLevelName(std::string newName);
GMLIB_API void        setFakeLevelName(std::string fakeName);

GMLIB_API int64_t getSeed();
GMLIB_API void    setFakeSeed(int64_t fakeSeed);

GMLIB_API void setCoResourcePack(bool enabled);

GMLIB_API void setForceTrustSkin(bool enabled);

GMLIB_API bool getExperimentEnabled(::AllExperiments experiment, bool enabled);
GMLIB_API void setExperimentEnabled(::AllExperiments experiment, bool enabled);
GMLIB_API void addExperimentsRequire(::AllExperiments experiment);

GMLIB_API void setForceAchievementsEnabled();

GMLIB_API void forceEnableAbilityCommand();
GMLIB_API void addEducationEditionRequired();

} // namespace GMLIB::LevelAPI
