#include "Global.h"
#include "mc/world/item/NetworkItemStackDescriptor.h"
#include <GMLIB/Event/Player/PlayerChangeDimensionEvent.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FloatingTextAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/NetworkPacketAPI.h>
#include <GMLIB/Server/PlaceholderAPI.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <mc/network/packet/AddItemActorPacket.h>
#include <mc/network/packet/RemoveActorPacket.h>
#include <mc/network/packet/SetActorDataPacket.h>
#include <mc/util/Random.h>

namespace GMLIB::Server {

std::unordered_map<int64, FloatingText*> mRuntimeFloatingTextList;
ll::schedule::ServerTimeScheduler        mScheduler;

int getNextFloatingTextId() {
    auto id = Random::getThreadLocal().nextInt(0, 2147483647);
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(id))) {
        id = Random::getThreadLocal().nextInt(0, 2147483647);
    }
    return id;
}

FloatingText::FloatingText(std::string text, Vec3 position, DimensionType dimensionId, bool usePapi)
: mText(text),
  mPosition(position),
  mDimensionId(dimensionId),
  mUsePapi(usePapi) {
    mRuntimeId                           = getNextFloatingTextId();
    mRuntimeFloatingTextList[mRuntimeId] = this;
}

FloatingText::~FloatingText() {
    removeFromAllClients();
    mRuntimeFloatingTextList.erase(mRuntimeId);
}

void FloatingText::setPosition(Vec3& pos, DimensionType dimid) {
    mPosition    = pos;
    mDimensionId = dimid;
}

void FloatingText::setUsePapi(bool value) { mUsePapi = value; }

bool FloatingText::shouldUsePapi() const { return mUsePapi; }

GMLIB::Server::NetworkPacket<(int)MinecraftPacketIds::AddItemActor>
createAddFloatingTextPacket(FloatingText* ft, Player* pl) {
    auto item = std::make_unique<ItemStack>(ItemStack{"minecraft:air"});
    auto nisd = NetworkItemStackDescriptor(*item);
    auto text = ft->getText();
    if (ft->shouldUsePapi()) {
        PlaceholderAPI::translate(text, pl);
    }
    GMLIB_BinaryStream bs;
    bs.writeVarInt64(ft->getRuntimeID());
    bs.writeUnsignedVarInt64(ft->getRuntimeID());
    bs.writeType(nisd);
    bs.writeVec3(ft->getPos());
    bs.writeVec3(Vec3{0, 0, 0});
    // DataItem
    bs.writeUnsignedVarInt(2);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeString(text);
    bs.writeUnsignedVarInt((uint)0x51);
    bs.writeUnsignedVarInt((uint)0x0);
    bs.writeBool(true);
    bs.writeBool(false);
    GMLIB::Server::NetworkPacket<(int)MinecraftPacketIds::AddItemActor> pkt(bs.getAndReleaseData());
    return pkt;
}

GMLIB::Server::NetworkPacket<(int)MinecraftPacketIds::SetActorData>
createUpdateFloatingTextPacket(FloatingText* ft, Player* pl) {
    auto text = ft->getText();
    if (ft->shouldUsePapi()) {
        PlaceholderAPI::translate(text, pl);
    }
    GMLIB_BinaryStream bs;
    bs.writeUnsignedVarInt64(ft->getRuntimeID());
    // DataItem
    bs.writeUnsignedVarInt(2);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeUnsignedVarInt((uint)0x4);
    bs.writeString(text);
    bs.writeUnsignedVarInt((uint)0x51);
    bs.writeUnsignedVarInt((uint)0x0);
    bs.writeBool(true);
    // Other
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt64(0);
    GMLIB::Server::NetworkPacket<(int)MinecraftPacketIds::SetActorData> pkt(bs.getAndReleaseData());
    return pkt;
}

void FloatingText::sendToClient(Player* pl) {
    if (!pl->isSimulatedPlayer() && pl->getDimensionId() == getDimensionId()) {
        auto pkt = createAddFloatingTextPacket(this, pl);
        pkt.sendTo(*pl);
    }
}

void FloatingText::sendToAllClients() {
    ll::service::getLevel()->forEachPlayer([this](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer() && pl.getDimensionId() == getDimensionId()) {
            auto pkt = createAddFloatingTextPacket(this, &pl);
            pkt.sendTo(pl);
        }
        return true;
    });
}

GMLIB_API void FloatingText::updateClient(Player* pl) {
    if (!pl->isSimulatedPlayer() && pl->getDimensionId() == getDimensionId()) {
        auto pkt = createUpdateFloatingTextPacket(this, pl);
        pkt.sendTo(*pl);
    }
}

GMLIB_API void FloatingText::updateAllClients() {
    ll::service::getLevel()->forEachPlayer([this](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer() && pl.getDimensionId() == getDimensionId()) {
            auto pkt = createUpdateFloatingTextPacket(this, &pl);
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

void FloatingText::setText(std::string newText) { mText = newText; }

FloatingText* FloatingText::getFloatingText(int64 runtimeId) {
    if (mRuntimeFloatingTextList.count(runtimeId)) {
        return mRuntimeFloatingTextList[runtimeId];
    }
    return nullptr;
}

bool FloatingText::deleteFloatingText(int64 runtimeId) {
    auto ft = getFloatingText(runtimeId);
    if (ft) {
        delete ft;
        return true;
    }
    return false;
}

int64 FloatingText::getRuntimeID() const { return mRuntimeId; }

std::string FloatingText::getText() const { return mText; }

Vec3 FloatingText::getPos() const { return mPosition; }

DimensionType FloatingText::getDimensionId() const { return mDimensionId; }

bool FloatingText::isDynamic() const { return false; }

StaticFloatingText::StaticFloatingText(std::string text, Vec3 position, DimensionType dimensionId, bool usePapi)
: FloatingText(text, position, dimensionId, usePapi) {
    sendToAllClients();
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.emplaceListener<ll::event::player::PlayerJoinEvent>([&](ll::event::player::PlayerJoinEvent& ev) {
        this->sendToClient(&ev.self());
    });
    eventBus.emplaceListener<GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent>(
        [&](GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent& ev) { this->sendToClient(&ev.self()); }
    );
}

DynamicFloatingText::DynamicFloatingText(
    std::string   text,
    Vec3          position,
    DimensionType dimensionId,
    uint          updateRate,
    bool          usePapi
)
: FloatingText(text, position, dimensionId, usePapi) {
    sendToAllClients();
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.emplaceListener<ll::event::player::PlayerJoinEvent>([&](ll::event::player::PlayerJoinEvent& ev) {
        this->sendToClient(&ev.self());
    });
    eventBus.emplaceListener<GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent>(
        [&](GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent& ev) { this->sendToClient(&ev.self()); }
    );
    mUpdateRate = updateRate;
    mTask       = mScheduler.add<ll::schedule::task::RepeatTask>(std::chrono::seconds::duration(mUpdateRate), [this] {
        this->updateAllClients();
    });
}

DynamicFloatingText::~DynamicFloatingText() {
    mTask->cancel();
    mScheduler.remove(mTask);
}

bool DynamicFloatingText::isDynamic() const { return true; }

bool DynamicFloatingText::stopUpdate() {
    if (!mTask->isCancelled()) {
        mTask->cancel();
        mScheduler.remove(mTask);
        return true;
    }
    return false;
}

bool DynamicFloatingText::startUpdate() {
    if (mTask->isCancelled()) {
        this->sendToAllClients();
        mTask = mScheduler.add<ll::schedule::task::RepeatTask>(std::chrono::seconds::duration(mUpdateRate), [this] {
            this->updateAllClients();
        });
        return true;
    }
    return false;
}

uint DynamicFloatingText::getUpdateRate() { return mUpdateRate; }

void DynamicFloatingText::setUpdateRate(uint seconds) {
    stopUpdate();
    mUpdateRate = seconds;
    startUpdate();
}

void StaticFloatingText::updateText(std::string newText) {
    setText(newText);
    updateAllClients();
}

void DynamicFloatingText::updateText(std::string newText) {
    setText(newText);
    updateAllClients();
}

void StaticFloatingText::updatePosition(Vec3& pos, DimensionType dimid) {
    setPosition(pos, dimid);
    sendToAllClients();
}

void DynamicFloatingText::updatePosition(Vec3& pos, DimensionType dimid) {
    setPosition(pos, dimid);
    sendToAllClients();
}


} // namespace GMLIB::Server