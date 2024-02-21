#pragma once
#include <ll/api/Logger.h>
#include <ll/api/event/Cancellable.h>
#include <ll/api/event/Emitter.h>
#include <ll/api/event/EmitterBase.h>
#include <ll/api/event/Event.h>
#include <ll/api/event/ListenerBase.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/schedule/Scheduler.h>
#include <ll/api/schedule/Task.h>
#include <ll/api/service/Bedrock.h>
#include <ll/api/service/Service.h>
#include <ll/api/utils/HashUtils.h>
#include <ll/api/utils/StringUtils.h>
#include <mc/certificates/Certificate.h>
#include <mc/certificates/ExtendedCertificate.h>
#include <mc/deps/core/mce/UUID.h>
#include <mc/deps/core/string/HashedString.h>
#include <mc/deps/core/utility/BinaryStream.h>
#include <mc/math/Vec3.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/network/NetworkIdentifier.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/Packet.h>
#include <mc/world/Minecraft.h>
#include <mc/world/actor/ActorDamageSource.h>
#include <mc/world/actor/Mob.h>
#include <mc/world/actor/item/ItemActor.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/Item.h>
#include <mc/world/item/registry/ItemStack.h>
#include <mc/world/level/BlockPos.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/ChunkPos.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/Spawner.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/BlockLegacy.h>
#include <mc/world/scores/Objective.h>
#include <mc/world/scores/Scoreboard.h>

#define S(x) std::to_string(x)

extern ll::Logger logger;

extern void initExperiments(LevelData* leveldat);
extern void CaculateTPS();

class DBStorage;

namespace GMLIB::Server::UserCache {
extern void initUserCache();
} // namespace GMLIB::Server::UserCache

namespace GMLIB {

template <typename T>
inline T* Global = nullptr;

extern void loadLib();
extern void enableLib();
extern void disableLib();
extern void updatePlaceholder();
extern void regServerPAPI();
extern void regPlayerPAPI();

} // namespace GMLIB