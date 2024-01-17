#pragma once
#include "GMLIB/GMLIB.h"
#include <mc/deps/core/utility/BinaryStream.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/network/packet/ActorLink.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/SerializedSkin.h>

class GMLIB_BinaryStream : public BinaryStream {
public:
    GMLIB_API std::string getRaw();

public:
    GMLIB_API void writeCompoundTag(CompoundTag& data);

    GMLIB_API void writeDataItem(std::vector<std::unique_ptr<class DataItem>> const& data);

    GMLIB_API void writeVec3(Vec3 data);

    GMLIB_API void writeVec2(Vec2 data);

    GMLIB_API void writeBlockPos(BlockPos data);

    GMLIB_API void writeAbilitiesData(struct SerializedAbilitiesData const& data);

    GMLIB_API void writeSkin(class SerializedSkin const& data);

    GMLIB_API void writeActorLink(struct ActorLink const& data);

    GMLIB_API void writeUuid(mce::UUID const& data);

    GMLIB_API void writePropertySyncData(struct PropertySyncData const& syncdata);

public:
    // Basic API Export
    GMLIB_API void writeBool(bool data);

    GMLIB_API void writeByte(uchar data);

    GMLIB_API void writeDouble(double data);

    GMLIB_API void writeFloat(float data);

    GMLIB_API void writeSignedBigEndianInt(int data);

    GMLIB_API void writeSignedInt(int data);

    GMLIB_API void writeSignedInt64(int64 data);

    GMLIB_API void writeSignedShort(short data);

    GMLIB_API void writeString(std::string_view data);

    GMLIB_API void writeUnsignedChar(uchar data);

    GMLIB_API void writeUnsignedInt(uint data);

    GMLIB_API void writeUnsignedInt64(uint64 data);

    GMLIB_API void writeUnsignedShort(ushort data);

    GMLIB_API void writeUnsignedVarInt(uint data);

    GMLIB_API void writeUnsignedVarInt64(uint64 data);

    GMLIB_API void writeVarInt(int data);

    GMLIB_API void writeVarInt64(int64 data);
};