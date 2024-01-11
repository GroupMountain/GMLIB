#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/world/actor/Actor.h>

namespace GMLIB::SpawnerAPI {

GMLIB_API Actor* spawnEntity(Vec3 pos, int dimid, std::string name, Actor* owner = nullptr);
GMLIB_API Mob*   spawnMob(
      Vec3        pos,
      int         dimid,
      std::string name,
      Actor*      owner        = nullptr,
      bool        naturalSpawn = false,
      bool        surface      = true,
      bool        fromSpawner  = false
  );
GMLIB_API ItemActor* spawnItem(Vec3 pos, int dimid, ItemStack& item, Actor* owner = nullptr);
GMLIB_API ItemActor*
                 spawnItem(Vec3 pos, int dimid, std::string name, int count = 1, int aux = 0, Actor* owner = nullptr);
GMLIB_API Actor* spawnProjectile(Actor* owner, std::string name, float speed, float offset = 0);

} // namespace GMLIB::SpawnerAPI
