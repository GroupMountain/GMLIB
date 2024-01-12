#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Server/ActorAPI.h"

class GMLIB_Spawner {
public:
    GMLIB_API static GMLIB_Actor* spawnEntity(Vec3 pos, int dimid, std::string name, GMLIB_Actor* owner = nullptr);

    GMLIB_API static Mob* spawnMob(
        Vec3         pos,
        int          dimid,
        std::string  name,
        GMLIB_Actor* owner        = nullptr,
        bool         naturalSpawn = false,
        bool         surface      = true,
        bool         fromSpawner  = false
    );

    GMLIB_API static ItemActor* spawnItem(Vec3 pos, int dimid, ItemStack& item, GMLIB_Actor* owner = nullptr);

    GMLIB_API static ItemActor*
    spawnItem(Vec3 pos, int dimid, std::string name, int count = 1, int aux = 0, GMLIB_Actor* owner = nullptr);

    GMLIB_API static GMLIB_Actor* spawnProjectile(GMLIB_Actor* owner, std::string name, float speed = 2, float offset = 3);
};