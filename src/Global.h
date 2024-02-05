#pragma once
#include "Plugin.h"
#include <GMLIB/include_ll.h>
#include <GMLIB/include_mc.h>

#define S(x) std::to_string(x)

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