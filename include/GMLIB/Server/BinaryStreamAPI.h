#pragma once
#include "GMLIB/DllExport.h"
#include <mc/world/actor/Actor.h>
#include <mc/deps/core/utility/BinaryStream.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/world/actor/player/SerializedSkin.h>
#include <mc/network/packet/ActorLink.h>

namespace GMLIB::BinaryStreamHelper {

GMLIB_API void writeCompoundTag(BinaryStream& bs, CompoundTag& tag);
GMLIB_API void writeDataItem(BinaryStream& bs, std::vector<std::unique_ptr<class DataItem>> const& dataItems);
GMLIB_API void writeNetworkItemStackDescriptor(BinaryStream& bs, class NetworkItemStackDescriptor const& netItem);
GMLIB_API void writeVec3(BinaryStream& bs, Vec3 vec3);
GMLIB_API void writeAbilitiesData(BinaryStream& bs, struct SerializedAbilitiesData const& abilitiesData);
GMLIB_API void writeSkin(BinaryStream& bs, class SerializedSkin const& serializedSkin);
GMLIB_API void writeActorLink(BinaryStream& bs, struct ActorLink const& link);

} // namespace GMLIB::BinaryStreamHelper