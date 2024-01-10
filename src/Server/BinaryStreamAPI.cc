#include "GMLIB/Server/BinaryStreamAPI.h"
#include "Global.h"

namespace GMLIB::BinaryStreamHelper {

GMLIB_API inline void writeCompoundTag(BinaryStream& bs, CompoundTag& tag) {
    LL_SYMBOL_CALL("?write@?$serialize@VCompoundTag@@@@SAXAEBVCompoundTag@@AEAVBinaryStream@@@Z", void, CompoundTag&, BinaryStream&)
    (tag, bs);
}

GMLIB_API inline void writeDataItem(BinaryStream& bs, std::vector<std::unique_ptr<class DataItem>> const& dataItems) {
    LL_SYMBOL_CALL("?write@?$serialize@V?$vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$"
                "allocator@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@@@SAXAEBV?$"
                "vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$allocator@V?$unique_ptr@"
                "VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@AEAVBinaryStream@@@Z", void,std::vector<std::unique_ptr<class DataItem>> const&, class BinaryStream&)
    (dataItems, bs);
}

GMLIB_API inline void
writeNetworkItemStackDescriptor(BinaryStream& bs, class NetworkItemStackDescriptor const& netItem) {
    netItem.write(bs);
}

GMLIB_API inline void writeVec3(BinaryStream& bs, Vec3 vec3) {
    bs.writeFloat(vec3.x);
    bs.writeFloat(vec3.y);
    bs.writeFloat(vec3.z);
}

GMLIB_API inline void writeBlockPos(BinaryStream& bs, BlockPos pos) {
    bs.writeVarInt(pos.x);
    bs.writeUnsignedVarInt(pos.y);
    bs.writeVarInt(pos.z);
}

GMLIB_API inline void writeAbilitiesData(BinaryStream& bs, struct SerializedAbilitiesData const& abilitiesData) {
    LL_SYMBOL_CALL("?write@?$serialize@USerializedAbilitiesData@@@@SAXAEBUSerializedAbilitiesData@@AEAVBinaryStream@@@Z", void, struct SerializedAbilitiesData const&, BinaryStream&)
    (abilitiesData, bs);
}

GMLIB_API inline void writeSkin(BinaryStream& bs, class SerializedSkin const& serializedSkin) {
    serializedSkin.write(bs);
}

GMLIB_API inline void writeActorLink(BinaryStream& bs, struct ActorLink const& link) {
    bs.writeVarInt64(link.A.id);
    bs.writeVarInt64(link.B.id);
    bs.writeUnsignedChar((uchar)link.type);
    bs.writeBool(link.mImmediate);
    bs.writeBool(link.mPassengerInitiated);
}

GMLIB_API inline void writeUuid(BinaryStream& bs, mce::UUID const& uuid) {
    bs.writeUnsignedInt64(uuid.a);
    bs.writeUnsignedInt64(uuid.b);
}


// Basic API Export
GMLIB_API inline void writeBool(BinaryStream& bs, bool data) { bs.writeBool(data); }
GMLIB_API inline void writeByte(BinaryStream& bs, uchar data) { bs.writeByte(data); }
GMLIB_API inline void writeDouble(BinaryStream& bs, double data) { bs.writeDouble(data); }
GMLIB_API inline void writeFloat(BinaryStream& bs, float data) { bs.writeFloat(data); }
GMLIB_API inline void writeSignedBigEndianInt(BinaryStream& bs, int data) { bs.writeSignedBigEndianInt(data); }
GMLIB_API inline void writeSignedInt(BinaryStream& bs, int data) { bs.writeSignedInt(data); }
GMLIB_API inline void writeSignedInt64(BinaryStream& bs, int64 data) { bs.writeSignedInt64(data); }
GMLIB_API inline void writeSignedShort(BinaryStream& bs, short data) { bs.writeSignedShort(data); }
GMLIB_API inline void writeString(BinaryStream& bs, std::string_view data) { bs.writeString(data); }
GMLIB_API inline void writeUnsignedChar(BinaryStream& bs, uchar data) { bs.writeUnsignedChar(data); }
GMLIB_API inline void writeUnsignedInt(BinaryStream& bs, uint data) { bs.writeUnsignedInt(data); }
GMLIB_API inline void writeUnsignedInt64(BinaryStream& bs, uint64 data) { bs.writeUnsignedInt64(data); }
GMLIB_API inline void writeUnsignedShort(BinaryStream& bs, ushort data) { bs.writeUnsignedShort(data); }
GMLIB_API inline void writeUnsignedVarInt(BinaryStream& bs, uint data) { bs.writeUnsignedVarInt(data); }
GMLIB_API inline void writeUnsignedVarInt64(BinaryStream& bs, uint64 data) { bs.writeUnsignedVarInt64(data); }
GMLIB_API inline void writeVarInt(BinaryStream& bs, int data) { bs.writeVarInt(data); }
GMLIB_API inline void writeVarInt64(BinaryStream& bs, int64 data) { bs.writeVarInt64(data); }

} // namespace GMLIB::BinaryStreamHelper
