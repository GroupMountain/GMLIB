#include "Global.h"
#include "mc/world/item/NetworkItemStackDescriptor.h"
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
}

FloatingText::~FloatingText() {
    removeFromAllClients();
    RuntimeFloatingTextList.erase(mRuntimeId);
}

int64_t FloatingText::getFloatingTextRuntimeId() { return mRuntimeId; }

GMLIB_NetworkPacket<15> createFloatingTextPacket(FloatingText* ft) {
    auto               item = std::make_unique<ItemStack>(ItemStack{"minecraft:air"});
    auto               nisd = NetworkItemStackDescriptor(*item);
    GMLIB_BinaryStream bs;
    bs.writeVarInt64(ft->mRuntimeId);
    bs.writeUnsignedVarInt64(ft->mRuntimeId);
    bs.writeType(nisd);
    bs.writeVec3(ft->mPosition);
    bs.writeVec3(Vec3{0, 0, 0});
    // DataItem
    bs.writeUnsignedVarInt(2);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeString(ft->mText);
    bs.writeUnsignedVarInt((uint)0x51);
    bs.writeUnsignedVarInt((uint)0x0);
    bs.writeBool(true);
    bs.writeBool(false);
    GMLIB_NetworkPacket<(int)MinecraftPacketIds::AddItemActor> pkt(bs.getAndReleaseData());
    return pkt;
}

void FloatingText::sendToClient(Player* pl) {
    if (!pl->isSimulatedPlayer() && pl->getDimensionId() == mDimensionId) {
        auto pkt = createFloatingTextPacket(this);
        pkt.sendTo(*pl);
    }
}

void FloatingText::sendToAllClients() {
    auto pkt = createFloatingTextPacket(this);
    ll::service::getLevel()->forEachPlayer([&](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer() && pl.getDimensionId() == mDimensionId) {
            pkt.sendTo(pl);
        }
        return true;
    });
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

void FloatingText::removeFromServer() {
    removeFromAllClients();
    RuntimeFloatingTextList.erase(mRuntimeId);
    delete this;
}

bool FloatingText::deleteFloatingText(int64 runtimeId) {
    auto ft = getFloatingText(runtimeId);
    if (ft) {
        ft->removeFromServer();
        return true;
    }
    return false;
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
    ft->sendToAllClients();
    ft->removeFromServer();
    // delete ft;
}