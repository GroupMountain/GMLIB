#include "GMLIB/Server/SpawnerAPI.h"
#include "GMLIB/Server/LevelAPI.h"
#include "Global.h"
#include <corecrt_math_defines.h>

namespace GMLIB::SpawnerAPI {

bool setProjectile(Actor* owner, Actor* proj, float speed, float offset) {
    try {
        proj->setOwner(owner->getOrCreateUniqueID());
        proj->teleport(owner->getPosition(), owner->getDimensionId());
        if (speed > 0) {
            auto berot = owner->getRotation();
            if (offset > 0) { // 偏移
                // 初始化随机数生成器
                std::srand(static_cast<unsigned>(std::time(nullptr)));
                double x_offset = (std::rand() % 200 - 100) / 100.0 * offset; // 在-1到1之间生成随机偏移
                double y_offset = (std::rand() % 200 - 100) / 100.0 * offset;
                // 调整角度
                berot.x += x_offset;
                berot.y += y_offset;
            }
            auto dx = -speed * sin(berot.y * M_PI / 180) * cos(berot.x * M_PI / 180);
            auto dz = speed * cos(berot.y * M_PI / 180) * cos(berot.x * M_PI / 180);
            auto dy = speed * sin(berot.x * M_PI / 180);
            proj->setVelocity({dx, -dy, dz});
        }
        return true;
    } catch (...) {
        return false;
    }
}

Actor* spawnEntity(Vec3 pos, int dimid, std::string name, Actor* owner) {
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

bool throwEntity(Actor* owner, Actor* proj, float speed, float offset) {
    if (owner && proj && speed > 0) {
        return setProjectile(owner, proj, speed, offset);
    }
    return false;
}

Actor* shootProjectile(Actor* owner, std::string name, float speed, float offset) {
    if (owner) {
        auto proj = SpawnerAPI::spawnEntity(owner->getPosition(), owner->getDimensionId(), name, owner);
        if (proj) {
            setProjectile(owner, proj, speed, offset);
            return proj;
        }
    }
    return nullptr;
}

} // namespace GMLIB::SpawnerAPI