#pragma once
#include "GMLIB/DllExport.h"
#include <mc/world/actor/Actor.h>

namespace GMLIB::SpawnerAPI {

GMLIB_API Actor* spawnEntity(Vec3 pos, int dimid, std::string name, Actor* owner = nullptr);

GMLIB_API Actor* shootProjectile(Actor* owner, std::string name, float speed, float offset = 0);

GMLIB_API bool throwEntity(Actor* owner, Actor* actor, float speed, float offset = 0);

} // namespace GMLIB::SpawnerAPI
