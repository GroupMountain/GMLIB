#pragma once
#include <LoggerAPI.h>
#include "memory/hook.h"
#include <mc/Json.hpp>
#include <mc/HashedString.hpp>
#include <mc/Actor.hpp>
#include <mc/Mob.hpp>
#include <mc/Level.hpp>
#include <mc/Player.hpp>
#include <mc/ActorUniqueID.hpp>
#include <ScheduleAPI.h>
#include <mc/MobEffect.hpp>
#include <mc/MobEffectInstance.hpp>
#include <mc/ItemStack.hpp>
#include <mc/Item.hpp>
#include <mc/ItemActor.hpp>
#include <mc/ActorDamageSource.hpp>
#include <mc/Block.hpp>
#include <mc/BlockSource.hpp>
#include <mc/BlockPos.hpp>

#define S(x) std::to_string(x)

extern Logger logger;
extern Json::Reader reader;
extern bool CanLoad;
extern void tryCreateResourceDirectory();
extern void ExportAPI();