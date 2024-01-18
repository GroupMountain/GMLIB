#include "GMLIB/Server/BinaryStreamAPI.h"
#include "Global.h"

inline std::string GMLIB_BinaryStream::getRaw() { return *ll::memory::dAccess<std::string*>(this, 96); }

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

inline void GMLIB_BinaryStream::writeVec3(Vec3 vec3) {
    writeFloat(vec3.x);
    writeFloat(vec3.y);
    writeFloat(vec3.z);
}

inline void GMLIB_BinaryStream::writeVec2(Vec2 data) {
    writeFloat(data.x);
    writeFloat(data.z);
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

inline void GMLIB_BinaryStream::writePropertySyncData(struct PropertySyncData const& syncdata) {
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

inline void GMLIB_BinaryStream::writeBool(bool data) { ((BinaryStream*)this)->writeBool(data); }

inline void GMLIB_BinaryStream::writeByte(uchar data) { ((BinaryStream*)this)->writeByte(data); }

inline void GMLIB_BinaryStream::writeDouble(double data) { ((BinaryStream*)this)->writeDouble(data); }

inline void GMLIB_BinaryStream::writeFloat(float data) { ((BinaryStream*)this)->writeFloat(data); }

inline void GMLIB_BinaryStream::writeSignedBigEndianInt(int data) {
    ((BinaryStream*)this)->writeSignedBigEndianInt(data);
}

inline void GMLIB_BinaryStream::writeSignedInt(int data) { ((BinaryStream*)this)->writeSignedInt(data); }

inline void GMLIB_BinaryStream::writeSignedInt64(int64 data) { ((BinaryStream*)this)->writeSignedInt64(data); }

inline void GMLIB_BinaryStream::writeSignedShort(short data) { ((BinaryStream*)this)->writeSignedShort(data); }

inline void GMLIB_BinaryStream::writeString(std::string_view data) { ((BinaryStream*)this)->writeString(data); }

inline void GMLIB_BinaryStream::writeUnsignedChar(uchar data) { ((BinaryStream*)this)->writeUnsignedChar(data); }

inline void GMLIB_BinaryStream::writeUnsignedInt(uint data) { ((BinaryStream*)this)->writeUnsignedInt(data); }

inline void GMLIB_BinaryStream::writeUnsignedInt64(uint64 data) { ((BinaryStream*)this)->writeUnsignedInt64(data); }

inline void GMLIB_BinaryStream::writeUnsignedShort(ushort data) { ((BinaryStream*)this)->writeUnsignedShort(data); }

inline void GMLIB_BinaryStream::writeUnsignedVarInt(uint data) { ((BinaryStream*)this)->writeUnsignedVarInt(data); }

inline void GMLIB_BinaryStream::writeUnsignedVarInt64(uint64 data) {
    ((BinaryStream*)this)->writeUnsignedVarInt64(data);
}

inline void GMLIB_BinaryStream::writeVarInt(int data) { ((BinaryStream*)this)->writeVarInt(data); }

inline void GMLIB_BinaryStream::writeVarInt64(int64 data) { ((BinaryStream*)this)->writeVarInt64(data); }
