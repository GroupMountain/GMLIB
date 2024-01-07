#pragma once
#include <mc/enums/AllExperiments.h>
#include <mc/world/level/Level.h>

#ifdef GMLIB_EXPORTS
#define GMLIB_LevelAPI __declspec(dllexport)
#else
#define GMLIB_LevelAPI __declspec(dllimport)
#endif

namespace LevelAPI {

GMLIB_LevelAPI BlockSource* getBlockSource(DimensionType dimid);
GMLIB_LevelAPI std::vector<Actor*> getAllEntities();
GMLIB_LevelAPI std::string getLevelName();
GMLIB_LevelAPI void        setLevelName(std::string newName);
GMLIB_LevelAPI void        setFakeLevelName(std::string fakeName);
GMLIB_LevelAPI int64_t     getSeed();
GMLIB_LevelAPI void        setFakeSeed(int64_t fakeSeed);
GMLIB_LevelAPI void        setCoResourcePack(bool enabled);
GMLIB_LevelAPI void        setForceTrustSkin(bool enabled);
GMLIB_LevelAPI bool        getExperimentEnabled(::AllExperiments experiment, bool enabled);
GMLIB_LevelAPI void        setExperimentEnabled(::AllExperiments experiment, bool enabled);
GMLIB_LevelAPI void        addExperimentsRequire(::AllExperiments experiment);
GMLIB_LevelAPI void        setForceAchievementsEnabled();
GMLIB_LevelAPI void        forceEnableAbilityCommand();
GMLIB_LevelAPI void        addEducationEditionRequired();

} // namespace LevelAPI
