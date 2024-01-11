#include "GMLIB/Server/BinaryStreamAPI.h"
#include "Global.h"


inline void GMLIB_BinaryStream::writeCompoundTag(CompoundTag& tag) {
    LL_SYMBOL_CALL("?write@?$serialize@VCompoundTag@@@@SAXAEBVCompoundTag@@AEAVBinaryStream@@@Z", void, CompoundTag&, BinaryStream&)
    (tag, *this);
}

inline void GMLIB_BinaryStream::writeDataItem(std::vector<std::unique_ptr<class DataItem>> const& dataItems) {
    LL_SYMBOL_CALL("?write@?$serialize@V?$vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$"
                "allocator@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@@@SAXAEBV?$"
                "vector@V?$unique_ptr@VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@V?$allocator@V?$unique_ptr@"
                "VDataItem@@U?$default_delete@VDataItem@@@std@@@std@@@2@@std@@AEAVBinaryStream@@@Z", void,std::vector<std::unique_ptr<class DataItem>> const&, class BinaryStream&)
    (dataItems, *this);
}

inline void GMLIB_BinaryStream::writeNetworkItemStackDescriptor(class NetworkItemStackDescriptor const& netItem) {
    netItem.write(*this);
}

inline void GMLIB_BinaryStream::writeVec3(Vec3 vec3) {
    writeFloat(vec3.x);
    writeFloat(vec3.y);
    writeFloat(vec3.z);
}

inline void GMLIB_BinaryStream::writeBlockPos(BlockPos pos) {
    writeVarInt(pos.x);
    writeUnsignedVarInt(pos.y);
    writeVarInt(pos.z);
}

inline void GMLIB_BinaryStream::writeAbilitiesData(struct SerializedAbilitiesData const& abilitiesData) {
    LL_SYMBOL_CALL("?write@?$serialize@USerializedAbilitiesData@@@@SAXAEBUSerializedAbilitiesData@@AEAVBinaryStream@@@Z", void, struct SerializedAbilitiesData const&, BinaryStream&)
    (abilitiesData, *this);
}

inline void GMLIB_BinaryStream::writeSkin(class SerializedSkin const& serializedSkin) { serializedSkin.write(*this); }

inline void GMLIB_BinaryStream::writeActorLink(struct ActorLink const& link) {
    writeVarInt64(link.A.id);
    writeVarInt64(link.B.id);
    writeUnsignedChar((uchar)link.type);
    writeBool(link.mImmediate);
    writeBool(link.mPassengerInitiated);
}

inline void GMLIB_BinaryStream::writeUuid(mce::UUID const& uuid) {
    writeUnsignedInt64(uuid.a);
    writeUnsignedInt64(uuid.b);
}
