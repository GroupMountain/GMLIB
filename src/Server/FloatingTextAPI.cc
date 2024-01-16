#include "Global.h"
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FloatingTextAPI.h>

std::unordered_map<int64, FloatingText*> RuntimeFloatingTextList;

FloatingText::FloatingText(std::string text, Vec3 position, DimensionType dimensionId)
: mText(text),
  mPosition(position),
  mDimensionId(dimensionId) {
    mRuntimeId = ll::service::getLevel()->getRandom().nextLong();
    RuntimeFloatingTextList.insert({mRuntimeId, this});
}

FloatingText::~FloatingText() {
    removeFromAllClients();
    RuntimeFloatingTextList.erase(mRuntimeId);
}

template <typename T>
inline std::unique_ptr<DataItem2<T>> makeDataItem2(ActorDataIDs id, T flag) {
    return std::make_unique<DataItem2<T>>((unsigned short)id, flag);
}

std::shared_ptr<Packet> createFloatingTextPacket(FloatingText ft) {
    std::vector<std::unique_ptr<DataItem>> dataItemList;
    // auto data1 = makeDataItem2<std::string>(ActorDataIDs::Name, ft.mText);
    // dataItemList.emplace_back(data1);
    // auto data2 = makeDataItem2<signed char>(ActorDataIDs::NametagAlwaysShow, true);
    // dataItemList.emplace_back(data2);
    ItemStackDescriptor
        isd(*ll::service::bedrock::getLevel()->getItemRegistry().lookupByName("minecraft:air"), 0, 1, nullptr);
    NetworkItemStackDescriptor nisd(isd);
    GMLIB_BinaryStream         bs;
    bs.writeVarInt64(ft.mRuntimeId);
    bs.writeUnsignedVarInt64(ft.mRuntimeId);
    bs.writeNetworkItemStackDescriptor(nisd);
    bs.writeVec3(ft.mPosition);
    bs.writeVec3(Vec3::ZERO);
    bs.writeDataItem(dataItemList);
    bs.writeBool(false);
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::AddItemActor);
    return pkt;
}

void FloatingText::sendToClient(Player* pl) {
    if (!pl->isSimulatedPlayer()) {
        auto pkt = createFloatingTextPacket(*this);
        pkt->sendTo(*pl);
    }
}

void FloatingText::sendToAllClients() {
    auto pkt = createFloatingTextPacket(*this);
    pkt->sendToClients();
}

void FloatingText::removeFromAllClients() { RemoveActorPacket(ActorUniqueID(this->mRuntimeId)).sendToClients(); }

void FloatingText::removeFromClient(Player* pl) {
    if (!pl->isSimulatedPlayer()) {
        RemoveActorPacket(ActorUniqueID(this->mRuntimeId)).sendTo(*pl);
    }
}

void FloatingText::updateText(std::string newText) { mText = newText; }

FloatingText* FloatingText::getFloatingText(int64 runtimeId) {
    if (RuntimeFloatingTextList.count(runtimeId)) {
        return RuntimeFloatingTextList[runtimeId];
    }
    return nullptr;
}