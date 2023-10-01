/**
 * @file ThrowEntity.cpp
 * @author Zhongzi8972
 * @brief 实体投掷相关API
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../GlobalHeader.h"
#include <mc/Vec2.hpp>
#include <mc/Spawner.hpp>
#include <mc/ActorDefinitionIdentifier.hpp>
#include <GMLib/GMLib_ServerAPI.h>

//此函数仅内部调用，不要传入空指针！
bool setProjectile(Actor* owner, Actor* proj, float speed, float offset) {
	try {
		proj->setOwner(owner->getActorUniqueId());
		proj->teleport(owner->getPos(), owner->getDimensionId());
		if (speed > 0) {
			auto berot = owner->getRotation();
		    if (offset > 0) {									//偏移
		        // 初始化随机数生成器
		        std::srand(static_cast<unsigned>(std::time(nullptr)));
		        double x_offset = (std::rand() % 200 - 100) / 100.0 * offset; // 在-1到1之间生成随机偏移
		        double y_offset = (std::rand() % 200 - 100) / 100.0 * offset;
		        // 调整角度
		        berot.x += x_offset;
		        berot.y += y_offset;
		    }
		    auto dx = -speed * sin(berot.y * PI / 180) * cos(berot.x * PI / 180);
		    auto dz = speed * cos(berot.y * PI / 180) * cos(berot.x * PI / 180);
		    auto dy = speed * sin(berot.x * PI / 180);
		    proj->setVelocity({ dx, -dy, dz });
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

bool GMLib_Server::throwEntity(Actor* owner, Actor* proj, float speed, float offset) {
	if (owner && proj && speed > 0) {
		return setProjectile(owner, proj, speed, offset);
	}
	return false;
}

Actor* GMLib_Server::shootProjectile(Actor* owner, std::string name, float speed, float offset) {
	if (owner) {
		auto proj = GMLib_Server::spawnActor(owner->getPos(), owner->getDimensionId(), name, owner);
	    if (proj) {
		    setProjectile(owner, proj, speed, offset);
			return proj;
	    }
	}
	return nullptr;
}

Actor* GMLib_Server::spawnActor(Vec3 pos, int dimid, std::string name, Actor* owner) {
	try {
		ReplaceStr(name, "minecraft:", "");
		ActorDefinitionIdentifier identifier(name);
        Actor* ac = Global<Level>->getSpawner().spawnProjectile(*Global<Level>->getBlockSource(dimid), identifier, owner, pos, Vec3());
        return ac;
    } 
	catch (...) { 
		return nullptr;
	}
}