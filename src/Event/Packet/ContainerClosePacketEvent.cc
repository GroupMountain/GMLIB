#include "Global.h"
#include <GMLIB/Event/Packet/ContainerClosePacketEvent.h>

namespace GMLIB::Event::PacketEvent {

ServerNetworkHandler& ContainerClosePacketSendBeforeEvent::getServerNetworkHandler() const {
    return mServerNetworkHandler;
}
NetworkIdentifier&    ContainerClosePacketSendBeforeEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
ContainerClosePacket& ContainerClosePacketSendBeforeEvent::getPacket() const { return mPacket; }

ServerNetworkHandler& ContainerClosePacketSendAfterEvent::getServerNetworkHandler() const {
    return mServerNetworkHandler;
}
NetworkIdentifier&    ContainerClosePacketSendAfterEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
ContainerClosePacket& ContainerClosePacketSendAfterEvent::getPacket() const { return mPacket; }

LL_TYPE_INSTANCE_HOOK(
    ContainerClosePacketSendHook,
    HookPriority::Low,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVContainerClosePacket@@@Z",
    void,
    NetworkIdentifier&    identifier,
    ContainerClosePacket& packet
) {
    auto beforeEvent = ContainerClosePacketSendBeforeEvent(*this, identifier, packet);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(identifier, packet);
    auto afterEvent = ContainerClosePacketSendAfterEvent(*this, identifier, packet);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ContainerClosePacketSendBeforeEventEmitter
: public ll::event::Emitter<emitterFactory1, ContainerClosePacketSendBeforeEvent> {
    ll::memory::HookRegistrar<ContainerClosePacketSendHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ContainerClosePacketSendBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ContainerClosePacketSendAfterEventEmitter
: public ll::event::Emitter<emitterFactory2, ContainerClosePacketSendAfterEvent> {
    ll::memory::HookRegistrar<ContainerClosePacketSendHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ContainerClosePacketSendAfterEventEmitter>();
}

ContainerClosePacket& ContainerClosePacketWriteBeforeEvent::getPacket() const { return mPacket; }

ContainerClosePacket& ContainerClosePacketWriteAfterEvent::getPacket() const { return mPacket; }

LL_TYPE_INSTANCE_HOOK(
    ContainerClosePacketWriteHook,
    HookPriority::Low,
    ContainerClosePacket,
    "?write@ContainerClosePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    auto beforeEvent = ContainerClosePacketWriteBeforeEvent(*this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(stream);
    auto afterEvent = ContainerClosePacketWriteBeforeEvent(*this);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory3(ll::event::ListenerBase&);
class ContainerClosePacketWriteBeforeEventEmitter
: public ll::event::Emitter<emitterFactory3, ContainerClosePacketWriteBeforeEvent> {
    ll::memory::HookRegistrar<ContainerClosePacketWriteHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory3(ll::event::ListenerBase&) {
    return std::make_unique<ContainerClosePacketWriteBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory4(ll::event::ListenerBase&);
class ContainerClosePacketWriteAfterEventEmitter
: public ll::event::Emitter<emitterFactory4, ContainerClosePacketWriteAfterEvent> {
    ll::memory::HookRegistrar<ContainerClosePacketWriteHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory4(ll::event::ListenerBase&) {
    return std::make_unique<ContainerClosePacketWriteAfterEventEmitter>();
}

} // namespace GMLIB::Event::PacketEvent
