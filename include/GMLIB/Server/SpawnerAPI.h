#pragma once
#include <mc/world/actor/Actor.h>

#ifdef GMLIB_EXPORTS
#define GMLIB_SpawnerAPI __declspec(dllexport)
#else
#define GMLIB_SpawnerAPI __declspec(dllimport)
#endif

namespace SpawnerAPI
{
    GMLIB_SpawnerAPI Actor* spawnEntity(Vec3 pos, int dimid, std::string name, Actor* owner = nullptr);
    GMLIB_SpawnerAPI Actor* shootProjectile(Actor* owner, std::string name, float speed, float offset = 0);
    GMLIB_SpawnerAPI bool throwEntity(Actor* owner, Actor* actor, float speed, float offset = 0);
} // namespace SpawnerAPI
