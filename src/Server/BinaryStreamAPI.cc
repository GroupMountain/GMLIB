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

GMLIB_API inline void writeNetworkItemStackDescriptor(BinaryStream& bs, class NetworkItemStackDescriptor const& netItem) {
    netItem.write(bs);
}

GMLIB_API inline void writeVec3(BinaryStream& bs, Vec3 vec3) {
    bs.writeFloat(vec3.x, 0, 0);
    bs.writeFloat(vec3.y, 0, 0);
    bs.writeFloat(vec3.z, 0, 0);
}

GMLIB_API inline void writeAbilitiesData(BinaryStream& bs, struct SerializedAbilitiesData const& abilitiesData) {
    LL_SYMBOL_CALL("?write@?$serialize@USerializedAbilitiesData@@@@SAXAEBUSerializedAbilitiesData@@AEAVBinaryStream@@@Z", void, struct SerializedAbilitiesData const&, BinaryStream&)
    (abilitiesData, bs);
}

GMLIB_API inline void writeSkin(BinaryStream& bs, class SerializedSkin const& serializedSkin) { serializedSkin.write(bs); }

GMLIB_API inline void writeActorLink(BinaryStream& bs, struct ActorLink const& link) {
    bs.writeVarInt64(link.A.id, 0, 0);
    bs.writeVarInt64(link.B.id, 0, 0);
    bs.writeUnsignedChar((uchar)link.type, 0, 0);
    bs.writeBool(link.mImmediate, 0, 0);
    bs.writeBool(link.mPassengerInitiated, 0, 0);
}

} // namespace GMLIB::BinaryStreamHelper
