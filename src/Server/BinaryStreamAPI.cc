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

std::string GMLIB_BinaryStream::getRaw() { return *ll::memory::dAccess<std::string*>(this, 96); }

void GMLIB_BinaryStream::writeCompoundTag(CompoundTag& tag) {
    LL_SYMBOL_CALL("?write@?$serialize@VCompoundTag@@@@SAXAEBVCompoundTag@@AEAVBinaryStream@@@Z", void, CompoundTag&, BinaryStream&)
    (tag, *this);
}

void GMLIB_BinaryStream::writeDataItem(std::vector<std::unique_ptr<class DataItem>> const& dataItems) {
    LL_SYMBOL_CALL("?write@?$serialize@V?$vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$"
                "allocator@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@@@SAXAEBV?$"
                "vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$allocator@V?$unique_ptr@"
                "VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@AEAVBinaryStream@@@Z", void,std::vector<std::unique_ptr<class DataItem>> const&, class BinaryStream&)
    (dataItems, *this);
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
    LL_SYMBOL_CALL("?write@?$serialize@USerializedAbilitiesData@@@@SAXAEBUSerializedAbilitiesData@@AEAVBinaryStream@@@Z", void, struct SerializedAbilitiesData const&, BinaryStream&)
    (abilitiesData, *this);
}

void GMLIB_BinaryStream::writeSkin(class SerializedSkin const& serializedSkin) { serializedSkin.write(*this); }

void GMLIB_BinaryStream::writeActorLink(struct ActorLink const& link) {
    writeVarInt64(link.A.id);
    writeVarInt64(link.B.id);
    writeUnsignedChar((uchar)link.type);
    writeBool(link.mImmediate);
    writeBool(link.mPassengerInitiated);
}

void GMLIB_BinaryStream::writeUuid(mce::UUID const& uuid) {
    writeUnsignedInt64(uuid.a);
    writeUnsignedInt64(uuid.b);
}

void GMLIB_BinaryStream::writePropertySyncData(struct PropertySyncData const& syncdata) {
    writeUnsignedVarInt(syncdata.mIntEntries.size());
    for (auto IntEntry : syncdata.mIntEntries) {
        writeUnsignedVarInt(IntEntry.mPropertyIndex);
        writeVarInt(IntEntry.mData);
    }
    writeUnsignedVarInt(syncdata.mFloatEntries.size());
    for (auto FloatEntry : syncdata.mIntEntries) {
        writeUnsignedVarInt(FloatEntry.mPropertyIndex);
        writeFloat(FloatEntry.mData);
    }
}
// Basic API Export

void GMLIB_BinaryStream::writeBool(bool data) { ((BinaryStream*)this)->writeBool(data); }

void GMLIB_BinaryStream::writeByte(uchar data) { ((BinaryStream*)this)->writeByte(data); }

void GMLIB_BinaryStream::writeDouble(double data) { ((BinaryStream*)this)->writeDouble(data); }

void GMLIB_BinaryStream::writeFloat(float data) { ((BinaryStream*)this)->writeFloat(data); }

void GMLIB_BinaryStream::writeSignedBigEndianInt(int data) { ((BinaryStream*)this)->writeSignedBigEndianInt(data); }

void GMLIB_BinaryStream::writeSignedInt(int data) { ((BinaryStream*)this)->writeSignedInt(data); }

void GMLIB_BinaryStream::writeSignedInt64(int64 data) { ((BinaryStream*)this)->writeSignedInt64(data); }

void GMLIB_BinaryStream::writeSignedShort(short data) { ((BinaryStream*)this)->writeSignedShort(data); }

void GMLIB_BinaryStream::writeString(std::string_view data) { ((BinaryStream*)this)->writeString(data); }

void GMLIB_BinaryStream::writeUnsignedChar(uchar data) { ((BinaryStream*)this)->writeUnsignedChar(data); }

void GMLIB_BinaryStream::writeUnsignedInt(uint data) { ((BinaryStream*)this)->writeUnsignedInt(data); }

void GMLIB_BinaryStream::writeUnsignedInt64(uint64 data) { ((BinaryStream*)this)->writeUnsignedInt64(data); }

void GMLIB_BinaryStream::writeUnsignedShort(ushort data) { ((BinaryStream*)this)->writeUnsignedShort(data); }

void GMLIB_BinaryStream::writeUnsignedVarInt(uint data) { ((BinaryStream*)this)->writeUnsignedVarInt(data); }

void GMLIB_BinaryStream::writeUnsignedVarInt64(uint64 data) { ((BinaryStream*)this)->writeUnsignedVarInt64(data); }

void GMLIB_BinaryStream::writeVarInt(int data) { ((BinaryStream*)this)->writeVarInt(data); }

void GMLIB_BinaryStream::writeVarInt64(int64 data) { ((BinaryStream*)this)->writeVarInt64(data); }

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

void GMLIB_BinaryStream::sendToClients() {
    ll::service::getLevel()->forEachPlayer([&](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer()) {
            sendTo(pl);
        }
        return true;
    });
}

void GMLIB_BinaryStream::sendToClients(DimensionType dimId) {
    ll::service::getLevel()->forEachPlayer([&](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer() && pl.getDimensionId() == dimId) {
            sendTo(pl);
        }
        return true;
    });
}