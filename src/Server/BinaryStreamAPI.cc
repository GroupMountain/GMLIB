#include "Global.h"
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <mc/math/Vec2.h>
#include <mc/network/NetworkConnection.h>
#include <mc/network/NetworkSystem.h>
#include <mc/network/packet/ActorLink.h>
#include <mc/network/packet/SerializedAbilitiesData.h>
#include <mc/world/actor/DataItem.h>
#include <mc/world/actor/player/SerializedSkin.h>
#include <mc/world/actor/state/PropertySyncData.h>

void GMLIB_BinaryStream::writeCompoundTag(CompoundTag& tag) { writeType<CompoundTag>(tag); }

void GMLIB_BinaryStream::writeDataItem(std::vector<std::unique_ptr<class DataItem>> const& dataItems) {
    writeType<std::vector<std::unique_ptr<class DataItem>>>(dataItems);
}

void GMLIB_BinaryStream::writeVec3(Vec3 vec3) {
    writeFloat(vec3.x);
    writeFloat(vec3.y);
    writeFloat(vec3.z);
}

void GMLIB_BinaryStream::writeVec2(Vec2 data) {
    writeFloat(data.x);
    writeFloat(data.z);
}

void GMLIB_BinaryStream::writeBlockPos(BlockPos pos) {
    writeVarInt(pos.x);
    writeUnsignedVarInt(pos.y);
    writeVarInt(pos.z);
}

void GMLIB_BinaryStream::writeAbilitiesData(struct SerializedAbilitiesData const& abilitiesData) {
    writeType<SerializedAbilitiesData>(abilitiesData);
}

void GMLIB_BinaryStream::writeSkin(class SerializedSkin const& serializedSkin) { serializedSkin.write(*this); }

void GMLIB_BinaryStream::writeActorLink(struct ActorLink const& link) {
    writeVarInt64(link.mA.id);
    writeVarInt64(link.mB.id);
    writeUnsignedChar((uchar)link.mType);
    writeBool(link.mImmediate);
    writeBool(link.mPassengerInitiated);
}

void GMLIB_BinaryStream::writeUuid(mce::UUID const& uuid) {
    writeUnsignedInt64(uuid.a);
    writeUnsignedInt64(uuid.b);
}

void GMLIB_BinaryStream::writePropertySyncData(struct PropertySyncData const& syncdata) {
    writeUnsignedVarInt(syncdata.mIntEntries.size());
    for (auto& IntEntry : syncdata.mIntEntries) {
        writeUnsignedVarInt(IntEntry.mPropertyIndex);
        writeVarInt(IntEntry.mData);
    }
    writeUnsignedVarInt(syncdata.mFloatEntries.size());
    for (auto& FloatEntry : syncdata.mIntEntries) {
        writeUnsignedVarInt(FloatEntry.mPropertyIndex);
        writeFloat(FloatEntry.mData);
    }
}

// Send packet api
void GMLIB_BinaryStream::writePacketHeader(MinecraftPacketIds packetId, SubClientId subId) {
    writeUnsignedVarInt(
        (int)packetId & 0x3FF | (((uchar)subId & 3) << 10) | (((uchar)SubClientId::PrimaryClient & 3) << 12)
    );
}

void GMLIB_BinaryStream::sendTo(Player& pl, ::NetworkPeer::Reliability reliability, ::Compressibility compressible) {
    if (auto networksystem = ll::service::getNetworkSystem(); networksystem) {
        if (auto connection = networksystem->_getConnectionFromId(pl.getNetworkIdentifier()); connection) {
            if (auto peer = connection->mPeer; peer) {
                peer->sendPacket(*mBuffer, reliability, compressible);
            }
        }
    }
}

void GMLIB_BinaryStream::sendToClients(::NetworkPeer::Reliability reliability, ::Compressibility compressible) {
    ll::service::getLevel()->forEachPlayer([&](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer()) {
            sendTo(pl, reliability, compressible);
        }
        return true;
    });
}

void GMLIB_BinaryStream::sendToDimension(
    DimensionType              dimId,
    ::NetworkPeer::Reliability reliability,
    ::Compressibility          compressible
) {
    ll::service::getLevel()->getOrCreateDimension(dimId)->forEachPlayer([&](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer()) {
            sendTo(pl, reliability, compressible);
        }
        return true;
    });
}