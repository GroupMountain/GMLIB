#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/SpawnerAPI.h>

namespace GMLIB::SpawnerAPI {

GMLIB_API Actor* spawnEntity(Vec3 pos, int dimid, std::string name, Actor* owner) {
    try {
        ll::utils::string_utils::replaceAll(name, "minecraft:", "");
        ActorDefinitionIdentifier identifier(name);
        auto                      bs = GMLIB::LevelAPI::getBlockSource(dimid);
        Actor* ac = ll::service::getLevel()->getSpawner().spawnProjectile(*bs, identifier, owner, pos, Vec3::ZERO);
        return ac;
    } catch (...) {
        return nullptr;
    }
}

GMLIB_API Mob*
spawnMob(Vec3 pos, int dimid, std::string name, Actor* owner, bool naturalSpawn, bool surface, bool fromSpawner) {
    try {
        ll::utils::string_utils::replaceAll(name, "minecraft:", "");
        ActorDefinitionIdentifier identifier(name);
        auto                      bs = GMLIB::LevelAPI::getBlockSource(dimid);

        Mob* mob = ll::service::getLevel()
                       ->getSpawner()
                       .spawnMob(*bs, identifier, owner, pos, naturalSpawn, surface, fromSpawner);
        return mob;
    } catch (...) {
        return nullptr;
    }
}

GMLIB_API ItemActor* spawnItem(Vec3 pos, int dimid, ItemStack& item, Actor* owner) {
    try {
        auto       bs        = GMLIB::LevelAPI::getBlockSource(dimid);
        ItemActor* itemActor = ll::service::getLevel()->getSpawner().spawnItem(*bs, item, owner, pos, 0);
        return itemActor;
    } catch (...) {
        return nullptr;
    }
}

GMLIB_API ItemActor* spawnItem(Vec3 pos, int dimid, std::string name, int count, int aux, Actor* owner) {
    try {
        auto       bs        = GMLIB::LevelAPI::getBlockSource(dimid);
        auto       item      = ItemStack{name, count, aux};
        ItemActor* itemActor = ll::service::getLevel()->getSpawner().spawnItem(*bs, item, owner, pos, 0);
        return itemActor;
    } catch (...) {
        return nullptr;
    }
}

GMLIB_API Actor* spawnProjectile(Actor* owner, std::string name, float speed, float offset) {
    if (owner) {
        auto proj = SpawnerAPI::spawnEntity(owner->getPosition(), owner->getDimensionId(), name, owner);
        if (proj) {
            GMLIB::ActorAPI::setProjectile(owner, proj, speed, offset);
            return proj;
        }
    }
    return nullptr;
}

} // namespace GMLIB::SpawnerAPI