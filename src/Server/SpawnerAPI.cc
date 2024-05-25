#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>
#include <mc/server/commands/CommandUtils.h>
#include <mc/world/actor/ActorDefinitionIdentifier.h>

optional_ref<Actor>
GMLIB_Spawner::spawnEntity(Vec3 const& pos, DimensionType dimId, std::string_view id, optional_ref<Actor> owner) {
    auto name = std::string(id);
    try {
        ll::utils::string_utils::replaceAll(name, "minecraft:", "");
        ActorDefinitionIdentifier identifier(name);

        auto& bs = GMLIB_Level::getInstance()->getBlockSource(dimId);
        auto  result =
            (GMLIB_Actor*)ll::service::getLevel()->getSpawner().spawnProjectile(bs, identifier, owner, pos, Vec3::ZERO);
        return result;
    } catch (...) {
        return nullptr;
    }
}

optional_ref<Mob> GMLIB_Spawner::spawnMob(
    Vec3 const&         pos,
    DimensionType       dimId,
    std::string_view    id,
    optional_ref<Actor> owner,
    bool                naturalSpawn,
    bool                surface,
    bool                fromSpawner
) {
    auto name = std::string(id);
    try {
        ll::utils::string_utils::replaceAll(name, "minecraft:", "");
        ActorDefinitionIdentifier identifier(name);
        auto&                     bs = GMLIB_Level::getInstance()->getBlockSource(dimId);

        Mob* mob = ll::service::getLevel()
                       ->getSpawner()
                       .spawnMob(bs, identifier, owner, pos, naturalSpawn, surface, fromSpawner);
        return mob;
    } catch (...) {
        return nullptr;
    }
}

optional_ref<ItemActor>
GMLIB_Spawner::spawnItem(Vec3 const& pos, DimensionType dimId, ItemStack& item, optional_ref<Actor> owner) {
    try {
        auto&      bs        = GMLIB_Level::getInstance()->getBlockSource(dimId);
        ItemActor* itemActor = ll::service::getLevel()->getSpawner().spawnItem(bs, item, owner, pos, 0);
        return itemActor;
    } catch (...) {
        return nullptr;
    }
}

optional_ref<ItemActor> GMLIB_Spawner::spawnItem(
    Vec3 const&         pos,
    int                 dimId,
    std::string_view    name,
    int                 count,
    int                 aux,
    optional_ref<Actor> owner
) {
    try {
        auto&      bs        = GMLIB_Level::getInstance()->getBlockSource(dimId);
        auto       item      = ItemStack{name, count, aux};
        ItemActor* itemActor = ll::service::getLevel()->getSpawner().spawnItem(bs, item, owner, pos, 0);
        return itemActor;
    } catch (...) {
        return nullptr;
    }
}

optional_ref<Actor> GMLIB_Spawner::spawnProjectile(Actor& owner, std::string_view name, float speed, float offset) {
    auto proj = spawnEntity(owner.getPosition(), owner.getDimensionId(), name, owner);
    if (proj) {
        static_cast<GMLIB_Actor&>(owner).setProjectile(proj, speed, offset);
        return proj;
    }
    return nullptr;
}

optional_ref<Actor>
GMLIB_Spawner::summonEntity(Vec3 const& pos, int dimId, std::string_view id, optional_ref<Actor> summoner) {
    auto name = std::string(id);
    ll::utils::string_utils::replaceAll(name, "minecraft:", "");
    auto  identifider = ActorDefinitionIdentifier(name);
    auto  uniqueId    = ActorUniqueID(GMLIB_Actor::getNextActorUniqueID());
    auto& bs          = GMLIB_Level::getInstance()->getBlockSource(dimId);
    return CommandUtils::spawnEntityAt(bs, pos, identifider, uniqueId, summoner);
}