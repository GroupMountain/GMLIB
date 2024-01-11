#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/deps/core/utility/BinaryStream.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/network/packet/ActorLink.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/SerializedSkin.h>

class GMLIB_BinaryStream : public BinaryStream {
public:
    GMLIB_API void writeCompoundTag(CompoundTag& data);

    GMLIB_API void writeDataItem(std::vector<std::unique_ptr<class DataItem>> const& data);

    GMLIB_API void writeNetworkItemStackDescriptor(class NetworkItemStackDescriptor const& data);

    GMLIB_API void writeVec3(Vec3 data);

    GMLIB_API void writeBlockPos(BlockPos data);

    GMLIB_API void writeAbilitiesData(struct SerializedAbilitiesData const& data);

    GMLIB_API void writeSkin(class SerializedSkin const& data);

    GMLIB_API void writeActorLink(struct ActorLink const& data);

    GMLIB_API void writeUuid(mce::UUID const& data);
};