#pragma once
#include "GMLIB/Macros.h"
#include "mc/deps/core/utility/BinaryStream.h"
#include "mc/enums/SubClientId.h"
#include "mc/nbt/CompoundTag.h"
#include "mc/network/MinecraftPackets.h"
#include "mc/network/packet/ActorLink.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/actor/player/SerializedSkin.h"

class GMLIB_BinaryStream : public BinaryStream {
public:
    GMLIB_API void writePacketHeader(MinecraftPacketIds packetId, SubClientId subId = SubClientId::PrimaryClient);

    GMLIB_API void sendTo(
        Player&                    pl,
        ::NetworkPeer::Reliability reliability  = ::NetworkPeer::Reliability::ReliableOrdered,
        ::Compressibility          compressible = ::Compressibility::Compressible
    );

    GMLIB_API void sendToClients(
        ::NetworkPeer::Reliability reliability  = ::NetworkPeer::Reliability::ReliableOrdered,
        ::Compressibility          compressible = ::Compressibility::Compressible
    );

    GMLIB_API void sendToDimension(
        DimensionType              dimId,
        ::NetworkPeer::Reliability reliability  = ::NetworkPeer::Reliability::ReliableOrdered,
        ::Compressibility          compressible = ::Compressibility::Compressible
    );

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

    GMLIB_API void writePropertySyncData(struct PropertySyncData const& data);
};