#include "Global.h"
#include <GMLIB/Event/Player/PlayerChangeDimensionEvent.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FloatingTextAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/PlaceholderAPI.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <mc/network/packet/AddItemActorPacket.h>
#include <mc/network/packet/RemoveActorPacket.h>
#include <mc/network/packet/SetActorDataPacket.h>
#include <mc/util/Random.h>

namespace GMLIB::Server {

ll::schedule::ServerTimeScheduler mScheduler;

int getNextFloatingTextRuntimeId() {
    auto id = Random::getThreadLocal().nextInt(0, 2147483647);
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(id))) {
        id = Random::getThreadLocal().nextInt(0, 2147483647);
    }
    return id;
}

FloatingTextBase::FloatingTextBase(
    std::string const& text,
    Vec3 const&        position,
    DimensionType      dimensionId,
    bool               usePapi
)
: mText(text),
  mPosition(position),
  mDimensionId(dimensionId),
  mTranslatePlaceholderApi(usePapi) {
    mRuntimeId = getNextFloatingTextRuntimeId();
}

FloatingTextBase::~FloatingTextBase() { removeFromClients(); }

void FloatingTextBase::setPosition(Vec3 const& pos) { mPosition = pos; }

void FloatingTextBase::setDimensionId(DimensionType dimId) { mDimensionId = dimId; }

void FloatingTextBase::setTranslatePlaceholderApi(bool value) { mTranslatePlaceholderApi = value; }

bool FloatingTextBase::shouldTranslatePlaceholderApi() const { return mTranslatePlaceholderApi; }


void sendUpdateFloatingTextPacket(FloatingTextBase& ft, Player& pl) {
    auto text = ft.getText();
    if (ft.shouldTranslatePlaceholderApi()) {
        PlaceholderAPI::translate(text, &pl);
    }
    GMLIB_BinaryStream bs;
    bs.writePacketHeader(MinecraftPacketIds::SetActorData);
    bs.writeUnsignedVarInt64(ft.getRuntimeID());
    // DataItem
    bs.writeUnsignedVarInt(2);
    bs.writeUnsignedVarInt((uint)ActorDataIDs::Name);
    bs.writeUnsignedVarInt((uint)DataItemType::String);
    bs.writeString(text);
    bs.writeUnsignedVarInt((uint)ActorDataIDs::NametagAlwaysShow);
    bs.writeUnsignedVarInt((uint)DataItemType::Byte);
    bs.writeBool(true);
    // Other
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt64(0);
    bs.sendTo(pl);
}


void sendAddFloatingTextPacket(FloatingTextBase& ft, Player& pl) {
    auto text = ft.getText();
    auto pos  = ft.getPosition();
    if (ft.shouldTranslatePlaceholderApi()) {
        PlaceholderAPI::translate(text, &pl);
    }
    GMLIB_BinaryStream bs;
    bs.writePacketHeader(MinecraftPacketIds::AddActor);
    bs.writeVarInt64(ft.getRuntimeID());
    bs.writeUnsignedVarInt64(ft.getRuntimeID());
    bs.writeString("player");
    bs.writeVec3({pos.x, pos.y - 2, pos.z});
    bs.writeVec3(Vec3{0, 0, 0});
    bs.writeVec2(Vec2{0, 0});
    bs.writeFloat(0.0f);
    bs.writeFloat(0.0f);
    bs.writeUnsignedVarInt(0);
    // DataItem
    bs.writeUnsignedVarInt(2);
    bs.writeUnsignedVarInt((uint)ActorDataIDs::Name);
    bs.writeUnsignedVarInt((uint)DataItemType::String);
    bs.writeString(text);
    bs.writeUnsignedVarInt((uint)ActorDataIDs::NametagAlwaysShow);
    bs.writeUnsignedVarInt((uint)DataItemType::Byte);
    bs.writeBool(true);
    // Others
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt(0);
    // send
    bs.sendTo(pl);
}

void FloatingTextBase::sendTo(Player& pl) {
    if (!pl.isSimulatedPlayer() && pl.getDimensionId() == getDimensionId()) {
        sendAddFloatingTextPacket(*this, pl);
    }
}

void FloatingTextBase::sendToClients() {
    ll::service::getLevel()->getOrCreateDimension(getDimensionId())->forEachPlayer([this](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer()) {
            sendAddFloatingTextPacket(*this, pl);
        }
        return true;
    });
}

GMLIB_API void FloatingTextBase::update(Player& pl) {
    if (!pl.isSimulatedPlayer() && pl.getDimensionId() == getDimensionId()) {
        sendUpdateFloatingTextPacket(*this, pl);
    }
}

GMLIB_API void FloatingTextBase::updateClients() {
    ll::service::getLevel()->getOrCreateDimension(getDimensionId())->forEachPlayer([this](Player& pl) -> bool {
        if (!pl.isSimulatedPlayer()) {
            sendUpdateFloatingTextPacket(*this, pl);
        }
        return true;
    });
}

void FloatingTextBase::removeFromClients() { RemoveActorPacket(ActorUniqueID(getRuntimeID())).sendToClients(); }

void FloatingTextBase::removeFrom(Player& pl) {
    if (!pl.isSimulatedPlayer()) {
        RemoveActorPacket(ActorUniqueID(getRuntimeID())).sendTo(pl);
    }
}

void FloatingTextBase::setText(std::string const& newText) { mText = newText; }

int FloatingTextBase::getRuntimeID() const { return mRuntimeId; }

std::string FloatingTextBase::getText() const { return mText; }

Vec3 FloatingTextBase::getPosition() const { return mPosition; }

DimensionType FloatingTextBase::getDimensionId() const { return mDimensionId; }

bool FloatingTextBase::isDynamic() const { return false; }

StaticFloatingText::StaticFloatingText(
    std::string const& text,
    Vec3 const&        position,
    DimensionType      dimensionId,
    bool               usePapi
)
: FloatingTextBase(text, position, dimensionId, usePapi) {
    sendToClients();
}

DynamicFloatingText::DynamicFloatingText(
    std::string const& text,
    Vec3 const&        position,
    DimensionType      dimensionId,
    uint               updateRate,
    bool               usePapi
)
: StaticFloatingText(text, position, dimensionId, usePapi) {
    sendToClients();
    mUpdateInterval = updateRate;
    mTask = mScheduler.add<ll::schedule::task::RepeatTask>(std::chrono::seconds::duration(mUpdateInterval), [this] {
        this->updateClients();
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
        this->sendToClients();
        mTask = mScheduler.add<ll::schedule::task::RepeatTask>(std::chrono::seconds::duration(mUpdateInterval), [this] {
            this->updateClients();
        });
        return true;
    }
    return false;
}

uint DynamicFloatingText::getUpdateInterval() { return mUpdateInterval; }

void DynamicFloatingText::setUpdateInterval(uint seconds) {
    stopUpdate();
    mUpdateInterval = seconds;
    startUpdate();
}

void StaticFloatingText::setText(std::string const& newText) {
    mText = newText;
    updateClients();
}

void StaticFloatingText::setPosition(Vec3 const& pos) {
    mPosition = pos;
    sendToClients();
}

void StaticFloatingText::setDimensionId(DimensionType dimId) {
    mDimensionId = dimId;
    sendToClients();
}

FloatingTextManager::FloatingTextManager() {
    auto& eventBus = ll::event::EventBus::getInstance();
    auto  event1 =
        eventBus.emplaceListener<ll::event::player::PlayerJoinEvent>([&](ll::event::player::PlayerJoinEvent& ev) {
            for (auto& [runtimeId, floatingText] : mRuntimeFloatingTexts) {
                floatingText->sendTo(ev.self());
            }
        });
    mEventListener.push_back(event1->getId());
    auto event2 = eventBus.emplaceListener<GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent>(
        [&](GMLIB::Event::PlayerEvent::PlayerChangeDimensionAfterEvent& ev) {
            for (auto& [runtimeId, floatingText] : mRuntimeFloatingTexts) {
                floatingText->removeFrom(ev.self());
                floatingText->sendTo(ev.self());
            }
        }
    );
    mEventListener.push_back(event2->getId());
}
FloatingTextManager::~FloatingTextManager() {
    auto& eventBus = ll::event::EventBus::getInstance();
    for (auto& id : mEventListener) {
        eventBus.removeListener(id);
    }
}

FloatingTextManager& FloatingTextManager::getInstance() {
    static std::unique_ptr<FloatingTextManager> instance;
    if (!instance) {
        instance = std::make_unique<FloatingTextManager>();
    }
    return *instance;
}

bool FloatingTextManager::add(std::shared_ptr<FloatingTextBase> floatingText) {
    auto runtimeId = floatingText->getRuntimeID();
    if (!mRuntimeFloatingTexts.contains(runtimeId)) {
        mRuntimeFloatingTexts[runtimeId] = floatingText;
        return true;
    }
    return false;
}

bool FloatingTextManager::remove(std::shared_ptr<FloatingTextBase> floatingText) {
    auto runtimeId = floatingText->getRuntimeID();
    return remove(runtimeId);
}

bool FloatingTextManager::remove(int runtimeId) {
    if (mRuntimeFloatingTexts.contains(runtimeId)) {
        mRuntimeFloatingTexts.erase(runtimeId);
        return true;
    }
    return false;
}

optional_ref<FloatingTextBase> FloatingTextManager::getFloatingText(int runtimeId) {
    if (mRuntimeFloatingTexts.contains(runtimeId)) {
        return mRuntimeFloatingTexts[runtimeId].get();
    }
    return {};
}

std::vector<FloatingTextBase*> FloatingTextManager::getAllFloatingTexts() {
    std::vector<FloatingTextBase*> result;
    for (auto& [runtimeId, floatingText] : mRuntimeFloatingTexts) {
        result.push_back(floatingText.get());
    }
    return result;
}

std::vector<FloatingTextBase*> FloatingTextManager::getAllFloatingTexts(DimensionType dimId) {
    std::vector<FloatingTextBase*> result;
    for (auto& [runtimeId, floatingText] : mRuntimeFloatingTexts) {
        if (floatingText->getDimensionId() == dimId) {
            result.push_back(floatingText.get());
        }
    }
    return result;
}

std::shared_ptr<StaticFloatingText> FloatingTextManager::createStatic(
    std::string const& text,
    Vec3 const&        position,
    DimensionType      dimensionId,
    bool               translatePlaceholderApi
) {
    return std::make_shared<StaticFloatingText>(text, position, dimensionId, translatePlaceholderApi);
}

std::shared_ptr<DynamicFloatingText> FloatingTextManager::createDynamic(
    std::string const& text,
    Vec3 const&        position,
    DimensionType      dimensionId,
    uint               updateInterval,
    bool               translatePlaceholderApi
) {
    return std::make_shared<DynamicFloatingText>(text, position, dimensionId, updateInterval, translatePlaceholderApi);
}

} // namespace GMLIB::Server