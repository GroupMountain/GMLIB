#pragma once
#include "GMLIB/Macros.h"
#include "GMLIB/Server/ActorAPI.h"

class GMLIB_Spawner {
public:
    GMLIB_API static GMLIB_Actor*
    spawnEntity(Vec3 const& pos, DimensionType dimId, std::string const& name, GMLIB_Actor* owner = nullptr);

    GMLIB_API static Mob* spawnMob(
        Vec3 const&        pos,
        DimensionType      dimId,
        std::string const& name,
        GMLIB_Actor*       owner        = nullptr,
        bool               naturalSpawn = false,
        bool               surface      = true,
        bool               fromSpawner  = false
    );

    GMLIB_API static ItemActor*
    spawnItem(Vec3 const& pos, DimensionType dimId, ItemStack& item, GMLIB_Actor* owner = nullptr);

    GMLIB_API static ItemActor* spawnItem(
        Vec3 const&        pos,
        int                dimid,
        std::string const& name,
        int                count = 1,
        int                aux   = 0,
        GMLIB_Actor*       owner = nullptr
    );

    GMLIB_API static GMLIB_Actor*
    spawnProjectile(GMLIB_Actor* owner, std::string const& name, float speed = 2, float offset = 3);

    GMLIB_API static GMLIB_Actor*
    summonEntity(Vec3 const& pos, int dimId, std::string const& name, GMLIB_Actor* summoner = nullptr);
};