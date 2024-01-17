#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FloatingTextAPI.h>
#include <GMLIB/Server/NetworkPacketAPI.h>

std::unordered_map<int64, FloatingText*> RuntimeFloatingTextList;

FloatingText::FloatingText(std::string text, Vec3 position, DimensionType dimensionId)
: mText(text),
  mPosition(position),
  mDimensionId(dimensionId) {
    mRuntimeId = GMLIB_Actor::getNextActorUniqueID();
    RuntimeFloatingTextList.insert({mRuntimeId, this});
    logger.warn("create {} {} {} {}", mText, mRuntimeId, mDimensionId.id, mPosition.toString());
}

FloatingText::~FloatingText() {
    removeFromAllClients();
    RuntimeFloatingTextList.erase(mRuntimeId);
    delete this;
}

int64_t FloatingText::getFloatingTextRuntimeId() { return mRuntimeId; }

GMLIB_NetworkPacket<15> createFloatingTextPacket(FloatingText* ft) {
    std::vector<std::unique_ptr<DataItem>> dataItemList;
    dataItemList.emplace_back(DataItem::create(ActorDataIDs::Name, ft->mText));
    dataItemList.emplace_back(DataItem::create<schar>(ActorDataIDs::NametagAlwaysShow, true));
    ItemStackDescriptor isd(*ll::service::getLevel()->getItemRegistry().lookupByName("minecraft:air"), 0, 1, nullptr);
    NetworkItemStackDescriptor nisd(isd);
    GMLIB_BinaryStream         bs;
    bs.writeVarInt64(ft->mRuntimeId);
    bs.writeUnsignedVarInt64(ft->mRuntimeId);
    bs.writeNetworkItemStackDescriptor(nisd);
    bs.writeVec3(ft->mPosition);
    bs.writeVec3(Vec3{0, 0, 0});

    bs.writeDataItem(dataItemList);

    bs.writeBool(false);
    GMLIB_NetworkPacket<(int)MinecraftPacketIds::AddItemActor> pkt(bs.getAndReleaseData());
    return pkt;
}

void FloatingText::sendToClient(Player* pl) {
    if (!pl->isSimulatedPlayer()) {
        auto pkt = createFloatingTextPacket(this);
        pkt.sendTo(*pl);
    }
}

void FloatingText::sendToAllClients() {
    auto pkt = createFloatingTextPacket(this);
    pkt.sendToClients();
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

#include <GMLIB/Server/PlayerAPI.h>
LL_AUTO_INSTANCE_HOOK(
    Test2,
    ll::memory::HookPriority::Normal,
    "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@AEAVCommandContext@@_N@Z",
    void,
    void* a1,
    void* a2,
    bool  a3
) {
    origin(a1, a2, a3);

    auto ft = new FloatingText("傻逼", {0, 120, 0}, 0);

    std::vector<std::unique_ptr<DataItem>> dataItemList;
    dataItemList.emplace_back(DataItem::create(ActorDataIDs::Name, ft->mText));
    dataItemList.emplace_back(DataItem::create<schar>(ActorDataIDs::NametagAlwaysShow, true));
    auto item = std::make_unique<ItemStack>(ItemStack{"minecraft:bedrock"});
    // ItemStackDescriptor        isd(*item->getItem(), item->getAuxValue(), 1, nullptr);
    // NetworkItemStackDescriptor nisd(isd);
    auto               nisd = NetworkItemStackDescriptor(*item);
    GMLIB_BinaryStream bs;
    bs.writeVarInt64(ft->mRuntimeId);
    bs.writeUnsignedVarInt64(ft->mRuntimeId);
    bs.writeNetworkItemStackDescriptor(nisd);
    bs.writeVec3(ft->mPosition);
    bs.writeVec3(Vec3{0, 0, 0});
    bs.writeDataItem(dataItemList);
    bs.writeBool(false);
    GMLIB_NetworkPacket<(int)MinecraftPacketIds::AddItemActor> pkt(bs.getAndReleaseData());
    pkt.sendToClients();
}