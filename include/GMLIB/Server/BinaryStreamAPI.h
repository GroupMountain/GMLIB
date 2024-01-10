#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/deps/core/utility/BinaryStream.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/network/packet/ActorLink.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/SerializedSkin.h>

namespace GMLIB::BinaryStreamHelper {

// Rapid Write API
GMLIB_API void writeCompoundTag(BinaryStream& bs, CompoundTag& data);
GMLIB_API void writeDataItem(BinaryStream& bs, std::vector<std::unique_ptr<class DataItem>> const& data);
GMLIB_API void writeNetworkItemStackDescriptor(BinaryStream& bs, class NetworkItemStackDescriptor const& data);
GMLIB_API void writeVec3(BinaryStream& bs, Vec3 data);
GMLIB_API void writeBlockPos(BinaryStream& bs, BlockPos data);
GMLIB_API void writeAbilitiesData(BinaryStream& bs, struct SerializedAbilitiesData const& data);
GMLIB_API void writeSkin(BinaryStream& bs, class SerializedSkin const& data);
GMLIB_API void writeActorLink(BinaryStream& bs, struct ActorLink const& data);
GMLIB_API void writeUuid(BinaryStream& bs, mce::UUID const& data);

// Basic API Export
GMLIB_API void writeBool(BinaryStream& bs, bool data);
GMLIB_API void writeByte(BinaryStream& bs, uchar data);
GMLIB_API void writeDouble(BinaryStream& bs, double data);
GMLIB_API void writeFloat(BinaryStream& bs, float data);
GMLIB_API void writeSignedBigEndianInt(BinaryStream& bs, int data);
GMLIB_API void writeSignedInt(BinaryStream& bs, int data);
GMLIB_API void writeSignedInt64(BinaryStream& bs, int64 data);
GMLIB_API void writeSignedShort(BinaryStream& bs, short data);
GMLIB_API void writeString(BinaryStream& bs, std::string_view data);
GMLIB_API void writeUnsignedChar(BinaryStream& bs, uchar data);
GMLIB_API void writeUnsignedInt(BinaryStream& bs, uint data);
GMLIB_API void writeUnsignedInt64(BinaryStream& bs, uint64 data);
GMLIB_API void writeUnsignedShort(BinaryStream& bs, ushort data);
GMLIB_API void writeUnsignedVarInt(BinaryStream& bs, uint data);
GMLIB_API void writeUnsignedVarInt64(BinaryStream& bs, uint64 data);
GMLIB_API void writeVarInt(BinaryStream& bs, int data);
GMLIB_API void writeVarInt64(BinaryStream& bs, int64 data);

} // namespace GMLIB::BinaryStreamHelper