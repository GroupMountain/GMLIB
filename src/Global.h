#pragma once
#include "Plugin.h"
#include "include_all.h"

extern ll::Logger logger;

extern void initExperiments(LevelData* leveldat);
extern void CaculateTPS();

class DBStorage;

namespace GMLIB::Server::UserCache {
extern void initUserCache();
} // namespace GMLIB::Server::UserCache

namespace GMLIB {

template <typename T>
inline T* Global = nullptr;

extern void loadLib();
extern void enableLib();
extern void disableLib();

} // namespace GMLIB