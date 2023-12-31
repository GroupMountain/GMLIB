#pragma once
#include "Plugin.h"
#include "include_all.h"

extern ll::Logger logger;
extern void       initExperiments(LevelData* leveldat);

class DBStorage;

namespace GMLIB {

template <typename T>
inline T* Global = nullptr;

} // namespace GMLIB