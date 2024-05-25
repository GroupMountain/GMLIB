#pragma once
#include "GMLIB/Macros.h"
#include "GMLIB/Server/ActorAPI.h"

class GMLIB_Spawner {
public:
    GMLIB_API static optional_ref<Actor>
    spawnEntity(Vec3 const& pos, DimensionType dimId, std::string_view name, optional_ref<Actor> owner = nullptr);

    GMLIB_API static optional_ref<Mob> spawnMob(
        Vec3 const&         pos,
        DimensionType       dimId,
        std::string_view    name,
        optional_ref<Actor> owner        = nullptr,
        bool                naturalSpawn = false,
        bool                surface      = true,
        bool                fromSpawner  = false
    );

    GMLIB_API static optional_ref<ItemActor>
    spawnItem(Vec3 const& pos, DimensionType dimId, ItemStack& item, optional_ref<Actor> owner = nullptr);

    GMLIB_API static optional_ref<ItemActor> spawnItem(
        Vec3 const&         pos,
        int                 dimId,
        std::string_view    name,
        int                 count = 1,
        int                 aux   = 0,
        optional_ref<Actor> owner = nullptr
    );

    GMLIB_API static optional_ref<Actor>
    spawnProjectile(Actor& owner, std::string_view name, float speed = 2, float offset = 3);

    GMLIB_API static optional_ref<Actor>
    summonEntity(Vec3 const& pos, int dimId, std::string_view name, optional_ref<Actor> summoner = nullptr);
};